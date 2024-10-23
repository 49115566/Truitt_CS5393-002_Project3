#include <iostream>
#include <exception>
#include "Trie.h"
#include "SentimentAnalyzer.h"

int main() {
    try {
        SentimentAnalyzer analyzer("trie.dat", "data/train_dataset_20k.csv");

        std::string text = "Sitting in the office forced to look at the sunshine from my window  At least I have a window to stare through. Only 2 wks to my run :-&amp;";

        double sentiment = analyzer.analyzeSentiment(text);
        std::cout << "Sentiment score: " << sentiment << std::endl;

        text = "Doing absolutely nothing alll weekend. How fun.";

        sentiment = analyzer.analyzeSentiment(text);
        std::cout << "Sentiment score: " << sentiment << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return -1;
    }

    return 0;
}