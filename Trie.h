#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>
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
    void train(const std::string& file);
    void insert(const std::string& word, bool isPositive);
    double getSentimentScore(const std::string& word) const;
    double getLogOddsRatio(const std::string& word) const;
    ~Trie();
    
    void save(const std::string& filename) const;
    void load(const std::string& filename);
private:
    void saveNode(std::ofstream& file, TrieNode* node, const std::string& prefix, std::vector<std::pair<std::string, TrieNode*>>& batch, ThreadPool& pool) const;
    void writeBatch(std::ofstream& file, const std::vector<std::pair<std::string, TrieNode*>>& batch) const;

    void deleteTrie(TrieNode* node);
};

#endif // TRIE_H