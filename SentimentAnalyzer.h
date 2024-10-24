#ifndef SENTIMENT_ANALYZER_H
#define SENTIMENT_ANALYZER_H

#include "DSString.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>

class SentimentAnalyzer {
public:
    SentimentAnalyzer(const DSString& saveFile, const DSString& trainFile);
    double analyzeSentiment(const DSString& text) const;
    void analyzeFile(const DSString& input, const DSString& output) const;
    double accuracy(const DSString& analyzedFile, const DSString& answersFile) const;

private:
    Trie trie;
    std::vector<DSString> tokenize(const DSString& text) const;
};

#endif // SENTIMENT_ANALYZER_H