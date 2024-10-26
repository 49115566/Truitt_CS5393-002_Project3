#include <iostream> // Include the iostream library for input and output
#include <exception> // Include the exception library for handling exceptions
#include "Trie.h" // Include the Trie header file
#include "SentimentAnalyzer.h" // Include the SentimentAnalyzer header file

int main() { // Main function
    try { // Try block to catch exceptions
        SentimentAnalyzer analyzer("trie.dat", "data/train_dataset_20k.csv"); // Create a SentimentAnalyzer object with the specified files

        analyzer.analyzeFile("data/test_dataset_10k.csv", "output.csv"); // Analyze the test dataset and output the results to a file

        double acc = analyzer.accuracy("output.csv", "data/test_dataset_sentiment_10k.csv"); // Calculate the accuracy of the analysis
        std::cout << "Accuracy: " << acc << std::endl; // Output the accuracy
    } catch (const std::exception& e) { // Catch block for standard exceptions
        std::cerr << "Exception: " << e.what() << std::endl; // Output the exception message
        return -1; // Return error code -1
    } catch (...) { // Catch block for any other exceptions
        std::cerr << "Unknown exception occurred" << std::endl; // Output a generic error message
        return -1; // Return error code -1
    }

    return 0; // Return 0 to indicate successful execution
}