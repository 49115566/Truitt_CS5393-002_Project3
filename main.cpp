#include <iostream>
#include <exception>
#include "Trie.h"
#include "SentimentAnalyzer.h"

int main() {
    try {
        std::cout << "Testing the Sentiment Analyzer!" << std::endl;
        SentimentAnalyzer SA("trie.dat");
        
        std::string text = "Jeremy is not a DS!";
        double score = SA.analyzeSentiment(text);

        std::cout << "Sentiment score for \"" << text << "\": " << score << std::endl;

        text = "Jeremy is a DS!";
        score = SA.analyzeSentiment(text);

        std::cout << "Sentiment score for \"" << text << "\": " << score << std::endl;

        text = "Jeremy is a DS! Jeremy is not a DS!";
        score = SA.analyzeSentiment(text);

        std::cout << "Sentiment score for \"" << text << "\": " << score << std::endl;

        text = "Jeremy is a DS! Jeremy: is not a DS! Jeremy has a DS!";
        score = SA.analyzeSentiment(text);

        std::cout << "Sentiment score for \"" << text << "\": " << score << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return -1;
    }

    return 0;
}