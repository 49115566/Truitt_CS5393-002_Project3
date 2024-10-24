#ifndef TRIE_H
#define TRIE_H

#include "DSString.h" // Include DSString header
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <cmath>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    void enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;

    void workerThread();
};

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    int positiveSentiments;
    int totalTweets;

    TrieNode();
};

class Trie {
private:
    TrieNode* root;

public:
    Trie();
    void train(const DSString& file);
    void insert(const DSString& word, bool isPositive);
    double getSentimentScore(const DSString& word) const;
    double getLogOddsRatio(const DSString& word) const;
    ~Trie();
    
    void save(const DSString& filename) const;
    void load(const DSString& filename);
private:
    void saveNode(std::ofstream& file, TrieNode* node, const DSString& prefix, std::vector<std::pair<DSString, TrieNode*>>& batch, ThreadPool& pool) const;
    void writeBatch(std::ofstream& file, const std::vector<std::pair<DSString, TrieNode*>>& batch) const;

    void deleteTrie(TrieNode* node);
};

#endif // TRIE_H