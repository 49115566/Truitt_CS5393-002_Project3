#include <iostream>
#include <exception>
#include "Trie.h"
#include "SentimentAnalyzer.h"

int main() {
    try {
        SentimentAnalyzer analyzer("trie.dat", "data/train_dataset_20k.csv");

        analyzer.analyzeFile("data/test_dataset_10k.csv", "output.csv");

        double acc = analyzer.accuracy("output.csv", "data/test_dataset_sentiment_10k.csv");
        std::cout << "Accuracy: " << acc << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return -1;
    }

    return 0;
}