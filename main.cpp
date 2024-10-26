#include <iostream> // Include the iostream library for input and output
#include <exception> // Include the exception library for handling exceptions
#include "Trie.h" // Include the Trie header file
#include "SentimentAnalyzer.h" // Include the SentimentAnalyzer header file

int main(int argc, char* argv[]) { // Main function with command-line arguments
    if (argc != 6) { // Check if the correct number of arguments is provided
        std::cerr << "Usage: " << argv[0] << " <train_dataset> <test_dataset> <test_sentiment> <output_file> <accuracy_file>" << std::endl;
        return -1; // Return error code -1
    }

    try { // Try block to catch exceptions
        SentimentAnalyzer analyzer("trie.dat", argv[1]); // Create a SentimentAnalyzer object with the specified files

        analyzer.analyzeFile(argv[2], argv[4]); // Analyze the test dataset and output the results to a file

        double acc = analyzer.accuracy(argv[4], argv[3], argv[5]); // Calculate the accuracy of the analysis
        std::cout << "Accuracy: " << std::fixed << std::setprecision(5) << acc << std::endl; // Output the accuracy
    } catch (const std::exception& e) { // Catch block for standard exceptions
        std::cerr << "Exception: " << e.what() << std::endl; // Output the exception message
        return -1; // Return error code -1
    } catch (...) { // Catch block for any other exceptions
        std::cerr << "Unknown exception occurred" << std::endl; // Output a generic error message
        return -1; // Return error code -1
    }

    return 0; // Return 0 to indicate successful execution
}