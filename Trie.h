#ifndef TRIE_H // Include guard to prevent multiple inclusions
#define TRIE_H // Define the include guard

#include "DSString.h" // Include DSString header
#include <unordered_map> // Include unordered_map for TrieNode children
#include <fstream> // Include fstream for file operations
#include <sstream> // Include sstream for string stream operations
#include <thread> // Include thread for multithreading
#include <mutex> // Include mutex for thread synchronization
#include <vector> // Include vector for dynamic array
#include <queue> // Include queue for task queue in ThreadPool
#include <condition_variable> // Include condition_variable for thread synchronization
#include <functional> // Include functional for std::function
#include <iostream> // Include iostream for input/output operations
#include <cmath> // Include cmath for mathematical operations
#include <chrono> // Include the chrono library for timing

/**
 * @class ThreadPool
 * @brief A simple thread pool implementation to manage and execute tasks using multiple threads.
 *
 * The ThreadPool class allows for the creation of a pool of worker threads that can be used to
 * execute tasks concurrently. Tasks are added to a queue and worker threads pick up tasks from
 * the queue and execute them.
 */
class ThreadPool {
public:
    /**
     * @brief Constructs a ThreadPool with a specified number of threads.
     * @param numThreads The number of threads to create in the pool.
     */
    ThreadPool(size_t numThreads);

    /**
     * @brief Destructor to clean up resources used by the ThreadPool.
     */
    ~ThreadPool();

    /**
     * @brief Adds a task to the task queue to be executed by the thread pool.
     * @param task A function object representing the task to be executed.
     */
    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers; ///< Vector to hold worker threads.
    std::queue<std::function<void()>> tasks; ///< Queue to hold tasks.
    std::mutex queueMutex; ///< Mutex to protect task queue.
    std::condition_variable condition; ///< Condition variable for task synchronization.
    bool stop; ///< Flag to stop the ThreadPool.

    /**
     * @brief Method for worker thread to execute tasks from the task queue.
     */
    void workerThread();
};

class TrieNode { // Define TrieNode class
public: // Public members
    std::unordered_map<char, TrieNode*> children; // Map to hold children nodes
    int positiveSentiments; // Counter for positive sentiments
    int totalTweets; // Counter for total tweets

    TrieNode(); // Constructor to initialize TrieNode
};

/**
 * @class Trie
 * @brief A class representing a Trie (prefix tree) for storing and analyzing words with sentiment scores.
 */
class Trie { // Define Trie class
private: // Private members
    TrieNode* root; // Root node of the Trie

public: // Public members
    /**
     * @brief Constructor to initialize the Trie.
     */
    Trie();

    /**
     * @brief Trains the Trie with words from a file.
     * @param file The file containing words to train the Trie.
     */
    void train(const DSString& file);

    /**
     * @brief Inserts a word into the Trie with its sentiment.
     * @param word The word to insert.
     * @param isPositive Boolean indicating if the word has a positive sentiment.
     */
    void insert(const DSString& word, bool isPositive);

    /**
     * @brief Gets the sentiment score of a word.
     * @param word The word to get the sentiment score for.
     * @return The sentiment score of the word.
     */
    double getSentimentScore(const DSString& word) const;

    /**
     * @brief Gets the log odds ratio of a word.
     * @param word The word to get the log odds ratio for.
     * @return The log odds ratio of the word.
     */
    double getLogOddsRatio(const DSString& word) const;

    /**
     * @brief Destructor to clean up resources.
     */
    ~Trie();

    /**
     * @brief Saves the Trie to a file.
     * @param filename The name of the file to save the Trie to.
     */
    void save(const DSString& filename) const;

    /**
     * @brief Loads the Trie from a file.
     * @param filename The name of the file to load the Trie from.
     */
    void load(const DSString& filename);

    /**
     * @brief Tokenizes a text into words.
     * @param text The text to tokenize.
     * @return A vector of tokenized words.
     */
    std::vector<DSString> tokenize(const DSString& text) const;

private: // Private members
    /**
     * @brief Saves a node and its children to a file.
     * @param file The file stream to write to.
     * @param node The node to save.
     * @param prefix The prefix of the current node.
     * @param batch A batch of nodes to save.
     * @param pool The thread pool for parallel processing.
     */
    void saveNode(std::ofstream& file, TrieNode* node, const DSString& prefix, std::vector<std::pair<DSString, TrieNode*>>& batch, ThreadPool& pool) const;

    /**
     * @brief Writes a batch of nodes to a file.
     * @param file The file stream to write to.
     * @param batch The batch of nodes to write.
     */
    void writeBatch(std::ofstream& file, const std::vector<std::pair<DSString, TrieNode*>>& batch) const;

    /**
     * @brief Deletes the Trie and frees memory.
     * @param node The root node of the Trie to delete.
     */
    void deleteTrie(TrieNode* node);
};

#endif // TRIE_H // End of include guard