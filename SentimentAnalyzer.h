#ifndef SENTIMENT_ANALYZER_H // Include guard to prevent multiple inclusions
#define SENTIMENT_ANALYZER_H // Define the include guard

#include "DSString.h" // Include custom DSString class
#include "Trie.h" // Include custom Trie class
#include <string> // Include standard string library
#include <vector> // Include standard vector library
#include <sstream> // Include string stream library
#include <algorithm> // Include algorithm library
#include <cctype> // Include character handling functions
#include <fstream> // Include file stream library
#include <chrono> // Include the chrono library for measuring time
#include <iomanip> // Include iomanip for output formatting

/**
 * @class SentimentAnalyzer
 * @brief A class for performing sentiment analysis on text data.
 * 
 * The SentimentAnalyzer class provides methods to analyze the sentiment of text using different methods,
 * analyze sentiment from files, and calculate the accuracy of the sentiment analysis.
 * It utilizes a Trie data structure for efficient sentiment analysis.
 */
class SentimentAnalyzer { // Define SentimentAnalyzer class
public: // Public members
    /**
     * @brief Constructs a new SentimentAnalyzer object.
     * 
     * @param saveFile The file where the analysis results will be saved.
     * @param trainFile The file used for training the sentiment analysis model.
     */
    SentimentAnalyzer(const DSString& saveFile, const DSString& trainFile); // Constructor

    /**
     * @brief Analyzes the sentiment of the given text using the LO method.
     * 
     * @param text The text to be analyzed.
     * @return double The sentiment score of the text.
     */
    double analyzeSentimentLO(const DSString& text) const; // Analyze sentiment using LO method

    /**
     * @brief Analyzes the sentiment of the given text using the SS method.
     * 
     * @param text The text to be analyzed.
     * @return double The sentiment score of the text.
     */
    double analyzeSentimentSS(const DSString& text) const; // Analyze sentiment using SS method

    /**
     * @brief Analyzes the sentiment of the text in the input file and writes the results to the output file.
     * 
     * @param input The input file containing text to be analyzed.
     * @param output The output file where the analysis results will be saved.
     */
    void analyzeFile(const DSString& input, const DSString& output) const; // Analyze sentiment of a file

    /**
     * @brief Calculates the accuracy of the sentiment analysis by comparing the analyzed file with the answers file.
     * 
     * @param analyzedFile The file containing the analyzed sentiment results.
     * @param answersFile The file containing the correct sentiment answers.
     * @return double The accuracy of the sentiment analysis.
     */
    double accuracy(const DSString& analyzedFile, const DSString& answersFile) const; // Calculate accuracy

private: // Private members
    Trie trie; // Trie data structure for sentiment analysis
};

#endif // SENTIMENT_ANALYZER_H // End of include guard