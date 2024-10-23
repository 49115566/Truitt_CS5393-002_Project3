#ifndef SENTIMENT_ANALYZER_H
#define SENTIMENT_ANALYZER_H

#include "Trie.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>

class SentimentAnalyzer {
public:
    SentimentAnalyzer(const std::string& saveFile, const std::string& trainFile);
    double analyzeSentiment(const std::string& text) const;
    void analyzeFile(const std::string& input, const std::string& output) const;
    double accuracy(const std::string& analyzedFile, const std::string& answersFile) const;

private:
    Trie trie;
    std::vector<std::string> tokenize(const std::string& text) const;
};

#endif // SENTIMENT_ANALYZER_H