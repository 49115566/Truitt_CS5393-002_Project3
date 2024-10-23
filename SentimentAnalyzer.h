#ifndef SENTIMENT_ANALYZER_H
#define SENTIMENT_ANALYZER_H

#include "Trie.h"
#include <string>
#include <vector>

class SentimentAnalyzer {
public:
    SentimentAnalyzer(const std::string& trieFile);
    double analyzeSentiment(const std::string& text) const;

private:
    Trie trie;
    std::vector<std::string> tokenize(const std::string& text) const;
};

#endif // SENTIMENT_ANALYZER_H