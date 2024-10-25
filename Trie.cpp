#include "Trie.h"
#include "DSString.h"

std::mutex mtx;

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
    }
    condition.notify_one();
}

void ThreadPool::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        try {
            task();
        } catch (const std::exception& e) {
            std::cerr << "Task threw an exception: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Task threw an unknown exception" << std::endl;
        }
    }
}

TrieNode::TrieNode() : totalTweets(0), positiveSentiments(0) {}

Trie::Trie() {
    root = new TrieNode();
}

void Trie::train(const DSString& file) {
    std::ifstream infile(file.c_str());
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }

    DSString line;
    while (getline(infile, line)) {
        std::istringstream ss(line.c_str());
        DSString sentimentStr, id, date, query, user, tweet;

        if (!getline(ss, sentimentStr, ',')) continue;
        if (!getline(ss, id, ',')) continue;
        if (!getline(ss, date, ',')) continue;
        if (!getline(ss, query, ',')) continue;
        if (!getline(ss, user, ',')) continue;
        if (!getline(ss, tweet)) continue;

        bool isPositive = (sentimentStr == "4");

        std::istringstream tweetStream(tweet.c_str());

        std::vector<DSString> words = tokenize(tweet);
        for (const DSString& word : words) {
            insert(word, isPositive);
        }
    }

    infile.close();
}

void Trie::insert(const DSString& word, bool isPositive) {
    TrieNode* current = root;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }
    current->totalTweets++;
    if (isPositive) {
        current->positiveSentiments++;
    }
}

double Trie::getSentimentScore(const DSString& word) const {
    TrieNode* current = root;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            return 0.0; // Word not found
        }
        current = current->children[c];
    }
    if (current->totalTweets == 0) {
        return 0.0;
    }
    return static_cast<double>(current->positiveSentiments - (current->totalTweets - current->positiveSentiments)) / current->totalTweets;
}

double Trie::getLogOddsRatio(const DSString& word) const {
    TrieNode* current = root;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            return 0.0; // Word not found
        }
        current = current->children[c];
    }
    if (current->totalTweets == 0) {
        return 0.0;
    }
    double positiveRatio = static_cast<double>(current->positiveSentiments + 1); // Laplace smoothing * (current->totalTweets + 2)
    double negativeRatio = static_cast<double>(current->totalTweets - current->positiveSentiments + 1); // Laplace smoothing * (current->totalTweets + 2)
    return std::log(positiveRatio / negativeRatio);
}

Trie::~Trie() {
    deleteTrie(root);
}

void Trie::deleteTrie(TrieNode* node) {
    if (node == nullptr) return;
    for (auto& pair : node->children) {
        deleteTrie(pair.second);
    }
    delete node;
}

void Trie::save(const DSString& filename) const {
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }
    std::vector<std::pair<DSString, TrieNode*>> batch;
    ThreadPool* pool = new ThreadPool(std::thread::hardware_concurrency());
    saveNode(file, root, "", batch, *pool);
    delete pool;
    if (!batch.empty()) {
        writeBatch(file, batch);
    }
    file.close();
}

void Trie::saveNode(std::ofstream& file, TrieNode* node, const DSString& prefix, std::vector<std::pair<DSString, TrieNode*>>& batch, ThreadPool& pool) const {
    if (node->totalTweets > 0) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            batch.emplace_back(prefix, node);
        }
        if (batch.size() >= 1000) { // Batch size of 1000
            std::vector<std::pair<DSString, TrieNode*>> batchCopy;
            {
                std::lock_guard<std::mutex> lock(mtx);
                batchCopy.swap(batch);
            }
            pool.enqueue([this, &file, batchCopy] {
                writeBatch(file, batchCopy);
            });
        }
    }
    for (const auto& pair : node->children) {
        pool.enqueue([this, &file, pair, prefix, &batch, &pool] {
            saveNode(file, pair.second, prefix + pair.first, batch, pool);
        });
    }
}

void Trie::writeBatch(std::ofstream& file, const std::vector<std::pair<DSString, TrieNode*>>& batch) const {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& pair : batch) {
        const DSString& prefix = pair.first;
        TrieNode* node = pair.second;
        size_t prefixSize = prefix.length();
        file.write(reinterpret_cast<const char*>(&prefixSize), sizeof(prefixSize));
        file.write(prefix.c_str(), prefixSize);
        file.write(reinterpret_cast<const char*>(&node->totalTweets), sizeof(node->totalTweets));
        file.write(reinterpret_cast<const char*>(&node->positiveSentiments), sizeof(node->positiveSentiments));
    }
}

void Trie::load(const DSString& filename) {
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }

    while (file) {
        size_t prefixSize;
        if (!file.read(reinterpret_cast<char*>(&prefixSize), sizeof(prefixSize))) {
            break; // Exit loop if reading fails (end of file or error)
        }

        DSString prefix(prefixSize, '\0');
        if (!file.read(&prefix[0], prefixSize)) {
            throw std::runtime_error("Error reading prefix from file");
        }

        int totalTweets, positiveSentiments;
        if (!file.read(reinterpret_cast<char*>(&totalTweets), sizeof(totalTweets)) ||
            !file.read(reinterpret_cast<char*>(&positiveSentiments), sizeof(positiveSentiments))) {
            throw std::runtime_error("Error reading tweet data from file");
        }

        TrieNode* current = root;
        for (char c : prefix) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->totalTweets = totalTweets;
        current->positiveSentiments = positiveSentiments;
    }
}

std::vector<DSString> Trie::tokenize(const DSString& text) const {
    std::vector<DSString> tokens;
    std::istringstream stream(text.c_str());
    DSString word;
    while (stream >> word) {
        // Remove punctuation and convert to lowercase
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (word == "not" || word == "no" || word == "nor" || word == "neither") {
            DSString nextWord;
            stream >> nextWord;
            nextWord.erase(std::remove_if(nextWord.begin(), nextWord.end(), ::ispunct), nextWord.end());
            std::transform(nextWord.begin(), nextWord.end(), nextWord.begin(), ::tolower);
            word += " ";
            word += nextWord;
        }
        tokens.push_back(word);
    }
    return tokens;
}