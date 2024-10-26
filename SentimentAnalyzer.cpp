#include "SentimentAnalyzer.h" // Include the header file for the SentimentAnalyzer class

SentimentAnalyzer::SentimentAnalyzer(const DSString& saveFile, const DSString& trainFile) { // Constructor for SentimentAnalyzer
    std::ifstream file(saveFile.c_str()); // Open the save file
    if (file.good() && file.peek() != std::ifstream::traits_type::eof()) { // Check if the file is good and not empty
        std::cout << "Loading trie from file..." << std::endl; // Print loading message
        trie.load(saveFile); // Load the trie from the save file
        std::cout << "Trie loaded!" << std::endl; // Print loaded message
    } else { // If the file is not good or is empty
        std::cout << "Training the trie..." << std::endl; // Print training message
        trie.train(trainFile); // Train the trie using the training file
        trie.save(saveFile); // Save the trained trie to the save file
        std::cout << "Trie trained and saved!" << std::endl; // Print trained and saved message
    }
}

double SentimentAnalyzer::analyzeSentimentLO(const DSString& text) const { // Analyze sentiment using log-odds ratio
    std::vector<DSString> words = trie.tokenize(text); // Tokenize the input text
    double logOddsSum = 0.0; // Initialize log-odds sum
    for (const DSString& word : words) { // Iterate over each word
        logOddsSum += trie.getLogOddsRatio(word); // Add the log-odds ratio of the word to the sum
    }
    return logOddsSum; // Return the log-odds sum
}

double SentimentAnalyzer::analyzeSentimentSS(const DSString& text) const { // Analyze sentiment using sentiment score
    std::vector<DSString> words = trie.tokenize(text); // Tokenize the input text
    double sentimentSum = 0.0; // Initialize sentiment sum
    for (const DSString& word : words) { // Iterate over each word
        sentimentSum += trie.getSentimentScore(word); // Add the sentiment score of the word to the sum
    }
    return sentimentSum; // Return the sentiment sum
}

void SentimentAnalyzer::analyzeFile(const DSString& input, const DSString& output) const { // Analyze sentiment of a file
    std::ifstream inputFile(input.c_str()); // Open the input file
    std::ofstream outputFile(output.c_str()); // Open the output file

    if (!inputFile.is_open()) { // Check if the input file is open
        throw std::runtime_error("Could not open input file"); // Throw an error if the input file could not be opened
    }

    if (!outputFile.is_open()) { // Check if the output file is open
        throw std::runtime_error("Could not open output file"); // Throw an error if the output file could not be opened
    }

    std::cout << "Analyzing file..." << std::endl; // Print analyzing message
    auto start = std::chrono::high_resolution_clock::now(); // Start the timer

    DSString line; // Declare a string to hold each line
    // Skip the header line
    getline(inputFile, line); // Read the header line
    outputFile << "Sentiment,id" << std::endl; // Write the header to the output file

    while (getline(inputFile, line)) { // Read each line from the input file
        std::istringstream stream(line.c_str()); // Create a string stream from the line
        DSString id, date, query, user, tweet; // Declare strings to hold the CSV fields

        // Read the CSV fields
        getline(stream, id, ','); // Read the id field
        getline(stream, date, ','); // Read the date field
        getline(stream, query, ','); // Read the query field
        getline(stream, user, ','); // Read the user field
        getline(stream, tweet, ','); // Read the tweet field

        double sentimentScore = analyzeSentimentLO(tweet) + 0.2; // Analyze the sentiment using log-odds ratio and add 0.2
        int sentiment; // Declare an integer to hold the sentiment
        if(sentimentScore) // Check if the sentiment score is not zero
            sentiment = sentimentScore > 0 ? 4 : 0; // Set sentiment to 4 if positive, otherwise 0
        else { // If the sentiment score is zero
            double sentimentScore = analyzeSentimentSS(tweet) + 0.2; // Analyze the sentiment using sentiment score and add 0.2
            sentiment = sentimentScore > 0 ? 4 : (sentimentScore == 0 ? 2 : 0); // Set sentiment based on the sentiment score
        }

        outputFile << sentiment << "," << id << std::endl; // Write the sentiment, id, and sentiment score to the output file
    }

    inputFile.close(); // Close the input file
    outputFile.close(); // Close the output file

    auto end = std::chrono::high_resolution_clock::now(); // End the timer
    std::chrono::duration<double> duration = end - start; // Calculate the duration
    std::cout << "Analysis complete! Time taken: " << duration.count() << " seconds" << std::endl; // Print analysis complete message with time taken
}

double SentimentAnalyzer::accuracy(const DSString& analyzedFile, const DSString& answersFile, const DSString& mistakesFile) const { // Calculate accuracy of sentiment analysis
    std::ifstream analyzed(analyzedFile.c_str()); // Open the analyzed file
    std::ifstream answers(answersFile.c_str()); // Open the answers file
    std::ofstream mistakes(mistakesFile.c_str()); // Open the mistakes file

    if (!analyzed.is_open()) { // Check if the analyzed file is open
        throw std::runtime_error("Could not open analyzed file"); // Throw an error if the analyzed file could not be opened
    }

    if (!answers.is_open()) { // Check if the answers file is open
        throw std::runtime_error("Could not open answers file"); // Throw an error if the answers file could not be opened
    }

    DSString analyzedLine, answersLine; // Declare strings to hold each line
    int matchingLines = 0; // Initialize matching lines count
    int totalLines = 0; // Initialize total lines count

    // Skip the header lines
    getline(analyzed, analyzedLine); // Read the header line from the analyzed file
    getline(answers, answersLine); // Read the header line from the answers file

    //Analyze percent accuracy
    while (getline(analyzed, analyzedLine) && getline(answers, answersLine)) { // Read each line from both files
        std::istringstream analyzedStream(analyzedLine.c_str()); // Create a string stream from the analyzed line
        std::istringstream answersStream(answersLine.c_str()); // Create a string stream from the answers line

        int analyzedSentiment, answersSentiment, analyzedId, answersId; // Declare integers to hold the CSV fields

        // Read the CSV fields
        analyzedStream >> analyzedSentiment; // Read the analyzed sentiment
        analyzedStream.ignore(1, ','); // Ignore the comma
        analyzedStream >> analyzedId; // Read the analyzed id

        answersStream >> answersSentiment; // Read the answers sentiment
        answersStream.ignore(1, ','); // Ignore the comma
        answersStream >> answersId; // Read the answers id

        if (analyzedId != answersId) { // Check if the ids do not match
            std::cerr << "ID mismatch at line " << totalLines + 1 << std::endl; // Print id mismatch message
            break; // Break the loop
        }

        if (analyzedSentiment == answersSentiment) { // Check if the sentiments match
            matchingLines++; // Increment matching lines count
        }

        totalLines++; // Increment total lines count
    }

    analyzed.close(); // Close the analyzed file
    answers.close(); // Close the answers file

    if(totalLines != 0){
        double accuracy = (static_cast<double>(matchingLines) / totalLines); // Calculate the accuracy as a percentage
        mistakes << std::fixed << std::setprecision(3) << accuracy << std::endl; // Write the accuracy to the mistakes file with 3 significant figures
    } else{
        mistakes << std::fixed << std::setprecision(3) << 0.0 << std::endl; // Write 0.0 accuracy to the mistakes file with 3 significant figures
        return 0.0; // Return 0.0 accuracy
    }

    analyzed.open(analyzedFile.c_str()); // Open the analyzed file
    answers.open(answersFile.c_str()); // Open the answers file
    
    if (!analyzed.is_open()) { // Check if the analyzed file is open
        throw std::runtime_error("Could not open analyzed file"); // Throw an error if the analyzed file could not be opened
    }

    if (!answers.is_open()) { // Check if the answers file is open
        throw std::runtime_error("Could not open answers file"); // Throw an error if the answers file could not be opened
    }

    // Skip the header lines
    getline(analyzed, analyzedLine); // Read the header line from the analyzed file
    getline(answers, answersLine); // Read the header line from the answers file

    while (getline(analyzed, analyzedLine) && getline(answers, answersLine)) { // Read each line from both files
        std::istringstream analyzedStream(analyzedLine.c_str()); // Create a string stream from the analyzed line
        std::istringstream answersStream(answersLine.c_str()); // Create a string stream from the answers line

        int analyzedSentiment, answersSentiment, analyzedId, answersId; // Declare integers to hold the CSV fields

        // Read the CSV fields
        analyzedStream >> analyzedSentiment; // Read the analyzed sentiment
        analyzedStream.ignore(1, ','); // Ignore the comma
        analyzedStream >> analyzedId; // Read the analyzed id

        answersStream >> answersSentiment; // Read the answers sentiment
        answersStream.ignore(1, ','); // Ignore the comma
        answersStream >> answersId; // Read the answers id

        if (analyzedId != answersId) { // Check if the ids do not match
            std::cerr << "ID mismatch at line " << totalLines + 1 << std::endl; // Print id mismatch message
            break; // Break the loop
        }

        if (analyzedSentiment != answersSentiment) { // If the sentiments do not match
            mistakes << analyzedSentiment << "," << answersSentiment << "," << analyzedId << std::endl; // Write the mistake to the mistakes file
        }
    }

    analyzed.close(); // Close the analyzed file
    answers.close(); // Close the answers file

    return (static_cast<double>(matchingLines) / totalLines); // Return the accuracy as a percentage
}
