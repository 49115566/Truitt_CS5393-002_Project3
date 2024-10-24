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
    double analyzeSentimentLO(const DSString& text) const;
    double analyzeSentimentSS(const DSString& text) const;
    void analyzeFile(const DSString& input, const DSString& output) const;
    double accuracy(const DSString& analyzedFile, const DSString& answersFile) const;

private:
    Trie trie;
};

#endif // SENTIMENT_ANALYZER_H