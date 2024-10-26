#include "Trie.h" // Include the Trie header file
#include "DSString.h" // Include the DSString header file

std::mutex mtx; // Declare a mutex for thread synchronization

ThreadPool::ThreadPool(size_t numThreads) : stop(false) { // Constructor for ThreadPool, initializes stop to false
    for (size_t i = 0; i < numThreads; ++i) { // Loop to create worker threads
        workers.emplace_back(&ThreadPool::workerThread, this); // Add a worker thread to the pool
    }
}

ThreadPool::~ThreadPool() { // Destructor for ThreadPool
    {
        std::unique_lock<std::mutex> lock(queueMutex); // Lock the queue mutex
        stop = true; // Set stop to true to signal threads to stop
    }
    condition.notify_all(); // Notify all threads
    for (std::thread &worker : workers) { // Loop through all worker threads
        worker.join(); // Join each worker thread
    }
}

void ThreadPool::enqueue(std::function<void()> task) { // Enqueue a task to the thread pool
    {
        std::unique_lock<std::mutex> lock(queueMutex); // Lock the queue mutex
        tasks.push(std::move(task)); // Add the task to the queue
    }
    condition.notify_one(); // Notify one thread
}

void ThreadPool::workerThread() { // Worker thread function
    while (true) { // Infinite loop
        std::function<void()> task; // Declare a task
        {
            std::unique_lock<std::mutex> lock(queueMutex); // Lock the queue mutex
            condition.wait(lock, [this] { return stop || !tasks.empty(); }); // Wait for a task or stop signal
            if (stop && tasks.empty()) { // If stop is true and no tasks are left
                return; // Exit the loop
            }
            task = std::move(tasks.front()); // Get the task from the queue
            tasks.pop(); // Remove the task from the queue
        }
        try {
            task(); // Execute the task
        } catch (const std::exception& e) { // Catch standard exceptions
            std::cerr << "Task threw an exception: " << e.what() << std::endl; // Print the exception message
        } catch (...) { // Catch all other exceptions
            std::cerr << "Task threw an unknown exception" << std::endl; // Print a generic error message
        }
    }
}

TrieNode::TrieNode() : totalTweets(0), positiveSentiments(0) {} // Constructor for TrieNode, initializes totalTweets and positiveSentiments to 0

Trie::Trie() { // Constructor for Trie
    root = new TrieNode(); // Create a new TrieNode for the root
}

void Trie::train(const DSString& file) { // Train the Trie with data from a file
    std::ifstream infile(file.c_str()); // Open the file for reading
    if (!infile.is_open()) { // Check if the file is open
        throw std::runtime_error("Could not open file for reading"); // Throw an error if the file could not be opened
    }

    DSString line; // Declare a string to hold each line
    while (getline(infile, line)) { // Read each line from the file
        std::istringstream ss(line.c_str()); // Create a string stream from the line
        DSString sentimentStr, id, date, query, user, tweet; // Declare strings for each field

        if (!getline(ss, sentimentStr, ',')) continue; // Read the sentiment field
        if (!getline(ss, id, ',')) continue; // Read the id field
        if (!getline(ss, date, ',')) continue; // Read the date field
        if (!getline(ss, query, ',')) continue; // Read the query field
        if (!getline(ss, user, ',')) continue; // Read the user field
        if (!getline(ss, tweet)) continue; // Read the tweet field

        bool isPositive = (sentimentStr == "4"); // Determine if the sentiment is positive

        std::istringstream tweetStream(tweet.c_str()); // Create a string stream from the tweet

        std::vector<DSString> words = tokenize(tweet); // Tokenize the tweet into words
        for (const DSString& word : words) { // Loop through each word
            insert(word, isPositive); // Insert the word into the Trie
        }
    }

    infile.close(); // Close the file
}

void Trie::insert(const DSString& word, bool isPositive) { // Insert a word into the Trie
    TrieNode* current = root; // Start at the root node
    for (char c : word) { // Loop through each character in the word
        if (current->children.find(c) == current->children.end()) { // If the character is not in the children map
            current->children[c] = new TrieNode(); // Create a new TrieNode for the character
        }
        current = current->children[c]; // Move to the child node
    }
    current->totalTweets++; // Increment the totalTweets count
    if (isPositive) { // If the sentiment is positive
        current->positiveSentiments++; // Increment the positiveSentiments count
    }
}

double Trie::getSentimentScore(const DSString& word) const { // Get the sentiment score for a word
    TrieNode* current = root; // Start at the root node
    for (char c : word) { // Loop through each character in the word
        if (current->children.find(c) == current->children.end()) { // If the character is not in the children map
            return 0.0; // Return 0.0 if the word is not found
        }
        current = current->children[c]; // Move to the child node
    }
    if (current->totalTweets == 0) { // If the totalTweets count is 0
        return 0.0; // Return 0.0
    }
    return static_cast<double>(current->positiveSentiments - (current->totalTweets - current->positiveSentiments)) / current->totalTweets; // Calculate and return the sentiment score
}

double Trie::getLogOddsRatio(const DSString& word) const { // Get the log odds ratio for a word
    TrieNode* current = root; // Start at the root node
    for (char c : word) { // Loop through each character in the word
        if (current->children.find(c) == current->children.end()) { // If the character is not in the children map
            return 0.0; // Return 0.0 if the word is not found
        }
        current = current->children[c]; // Move to the child node
    }
    if (current->totalTweets == 0) { // If the totalTweets count is 0
        return 0.0; // Return 0.0
    }
    double positiveRatio = static_cast<double>(current->positiveSentiments + 1); // Calculate the positive ratio with Laplace smoothing
    double negativeRatio = static_cast<double>(current->totalTweets - current->positiveSentiments + 1); // Calculate the negative ratio with Laplace smoothing
    return std::log(positiveRatio / negativeRatio); // Calculate and return the log odds ratio
}

Trie::~Trie() { // Destructor for Trie
    deleteTrie(root); // Delete the Trie starting from the root
}

void Trie::deleteTrie(TrieNode* node) { // Delete the Trie recursively
    if (node == nullptr) return; // Return if the node is null
    for (auto& pair : node->children) { // Loop through each child node
        deleteTrie(pair.second); // Recursively delete the child node
    }
    delete node; // Delete the current node
}

void Trie::save(const DSString& filename) const { // Save the Trie to a file
    std::ofstream file(filename.c_str(), std::ios::binary); // Open the file for writing in binary mode
    if (!file.is_open()) { // Check if the file is open
        throw std::runtime_error("Could not open file for writing"); // Throw an error if the file could not be opened
    }
    std::vector<std::pair<DSString, TrieNode*>> batch; // Declare a batch vector to hold nodes
    ThreadPool* pool = new ThreadPool(std::thread::hardware_concurrency()); // Create a thread pool with hardware concurrency
    saveNode(file, root, "", batch, *pool); // Save the root node
    delete pool; // Delete the thread pool
    if (!batch.empty()) { // If the batch is not empty
        writeBatch(file, batch); // Write the batch to the file
    }
    file.close(); // Close the file
}

void Trie::saveNode(std::ofstream& file, TrieNode* node, const DSString& prefix, std::vector<std::pair<DSString, TrieNode*>>& batch, ThreadPool& pool) const { // Save a node to the file
    if (node->totalTweets > 0) { // If the node has tweets
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock the mutex
            batch.emplace_back(prefix, node); // Add the node to the batch
        }
        if (batch.size() >= 1000) { // If the batch size is 1000 or more
            std::vector<std::pair<DSString, TrieNode*>> batchCopy; // Declare a batch copy vector
            {
                std::lock_guard<std::mutex> lock(mtx); // Lock the mutex
                batchCopy.swap(batch); // Swap the batch with the batch copy
            }
            pool.enqueue([this, &file, batchCopy] { // Enqueue a task to write the batch to the file
                writeBatch(file, batchCopy); // Write the batch to the file
            });
        }
    }
    for (const auto& pair : node->children) { // Loop through each child node
        pool.enqueue([this, &file, pair, prefix, &batch, &pool] { // Enqueue a task to save the child node
            saveNode(file, pair.second, prefix + pair.first, batch, pool); // Save the child node
        });
    }
}

void Trie::writeBatch(std::ofstream& file, const std::vector<std::pair<DSString, TrieNode*>>& batch) const { // Write a batch of nodes to the file
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex
    for (const auto& pair : batch) { // Loop through each pair in the batch
        const DSString& prefix = pair.first; // Get the prefix
        TrieNode* node = pair.second; // Get the node
        size_t prefixSize = prefix.length(); // Get the length of the prefix
        file.write(reinterpret_cast<const char*>(&prefixSize), sizeof(prefixSize)); // Write the prefix size to the file
        file.write(prefix.c_str(), prefixSize); // Write the prefix to the file
        file.write(reinterpret_cast<const char*>(&node->totalTweets), sizeof(node->totalTweets)); // Write the totalTweets count to the file
        file.write(reinterpret_cast<const char*>(&node->positiveSentiments), sizeof(node->positiveSentiments)); // Write the positiveSentiments count to the file
    }
}

void Trie::load(const DSString& filename) { // Load the Trie from a file
    std::ifstream file(filename.c_str(), std::ios::binary); // Open the file for reading in binary mode
    if (!file.is_open()) { // Check if the file is open
        throw std::runtime_error("Could not open file for reading"); // Throw an error if the file could not be opened
    }

    while (file) { // Loop while the file is open
        size_t prefixSize; // Declare a variable for the prefix size
        if (!file.read(reinterpret_cast<char*>(&prefixSize), sizeof(prefixSize))) { // Read the prefix size from the file
            break; // Exit the loop if reading fails
        }

        DSString prefix(prefixSize, '\0'); // Declare a string for the prefix
        if (!file.read(&prefix[0], prefixSize)) { // Read the prefix from the file
            throw std::runtime_error("Error reading prefix from file"); // Throw an error if reading fails
        }

        int totalTweets, positiveSentiments; // Declare variables for the tweet data
        if (!file.read(reinterpret_cast<char*>(&totalTweets), sizeof(totalTweets)) || // Read the totalTweets count from the file
            !file.read(reinterpret_cast<char*>(&positiveSentiments), sizeof(positiveSentiments))) { // Read the positiveSentiments count from the file
            throw std::runtime_error("Error reading tweet data from file"); // Throw an error if reading fails
        }

        TrieNode* current = root; // Start at the root node
        for (char c : prefix) { // Loop through each character in the prefix
            if (current->children.find(c) == current->children.end()) { // If the character is not in the children map
                current->children[c] = new TrieNode(); // Create a new TrieNode for the character
            }
            current = current->children[c]; // Move to the child node
        }
        current->totalTweets = totalTweets; // Set the totalTweets count
        current->positiveSentiments = positiveSentiments; // Set the positiveSentiments count
    }
}

std::vector<DSString> Trie::tokenize(const DSString& text) const { // Tokenize a string into words
    std::vector<DSString> tokens; // Declare a vector to hold the tokens
    std::istringstream stream(text.c_str()); // Create a string stream from the text
    DSString word; // Declare a string for each word
    while (stream >> word) { // Loop through each word in the stream
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end()); // Remove punctuation from the word
        std::transform(word.begin(), word.end(), word.begin(), ::tolower); // Convert the word to lowercase
        if (word == "not" || word == "no" || word == "nor" || word == "neither") { // Check for negation words
            DSString nextWord; // Declare a string for the next word
            stream >> nextWord; // Read the next word
            nextWord.erase(std::remove_if(nextWord.begin(), nextWord.end(), ::ispunct), nextWord.end()); // Remove punctuation from the next word
            std::transform(nextWord.begin(), nextWord.end(), nextWord.begin(), ::tolower); // Convert the next word to lowercase
            word += " "; // Add a space to the word
            word += nextWord; // Add the next word to the word
        }
        tokens.push_back(word); // Add the word to the tokens vector
    }
    return tokens; // Return the tokens vector
}
