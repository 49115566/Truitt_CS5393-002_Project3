#include "SentimentAnalyzer.h"
#include <sstream>
#include <algorithm>
#include <cctype>

SentimentAnalyzer::SentimentAnalyzer(const std::string& trieFile) {
    trie.load(trieFile);
}

double SentimentAnalyzer::analyzeSentiment(const std::string& text) const {
    std::vector<std::string> words = tokenize(text);
    double totalScore = 0.0;
    for (const std::string& word : words) {
        totalScore += trie.getSentimentScore(word);
    }
    return totalScore / words.size();
}

std::vector<std::string> SentimentAnalyzer::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::istringstream stream(text);
    std::string word;
    while (stream >> word) {
        // Remove punctuation and convert to lowercase
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        tokens.push_back(word);
    }
    return tokens;
}