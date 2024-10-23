#include "SentimentAnalyzer.h"

SentimentAnalyzer::SentimentAnalyzer(const std::string& saveFile, const std::string& trainFile) {
    std::ifstream file(saveFile);
    if (file.good() && file.peek() != std::ifstream::traits_type::eof()) {
        std::cout << "Loading trie from file..." << std::endl;
        trie.load(saveFile);
        std::cout << "Trie loaded!" << std::endl;
    } else {
        std::cout << "Training the trie..." << std::endl;
        trie.train(trainFile);
        trie.save(saveFile);
        std::cout << "Trie trained and saved!" << std::endl;
    }
}

double SentimentAnalyzer::analyzeSentiment(const std::string& text) const {
    std::vector<std::string> words = tokenize(text);
    double logOddsSum = 0.0;
    for (const std::string& word : words) {
        logOddsSum += trie.getLogOddsRatio(word);
    }
    return logOddsSum;
}

void SentimentAnalyzer::analyzeFile(const std::string& input, const std::string& output) const {
    std::ifstream inputFile(input);
    std::ofstream outputFile(output);

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open input file");
    }

    if (!outputFile.is_open()) {
        throw std::runtime_error("Could not open output file");
    }

    std::cout << "Analyzing file..." << std::endl;
    std::string line;
    // Skip the header line
    std::getline(inputFile, line);
    outputFile << "Sentiment,id" << std::endl;

    while (std::getline(inputFile, line)) {
        std::istringstream stream(line);
        std::string id, date, query, user, tweet;

        // Read the CSV fields
        std::getline(stream, id, ',');
        std::getline(stream, date, ',');
        std::getline(stream, query, ',');
        std::getline(stream, user, ',');
        std::getline(stream, tweet, ',');

        double sentimentScore = analyzeSentiment(tweet);
        int sentiment = sentimentScore > 0 ? 4 : 0;

        outputFile << sentiment << "," << id << std::endl;
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Analysis complete!" << std::endl;
}

double SentimentAnalyzer::accuracy(const std::string& analyzedFile, const std::string& answersFile) const {
    std::ifstream analyzed(analyzedFile);
    std::ifstream answers(answersFile);

    if (!analyzed.is_open()) {
        throw std::runtime_error("Could not open analyzed file");
    }

    if (!answers.is_open()) {
        throw std::runtime_error("Could not open answers file");
    }

    std::string analyzedLine, answersLine;
    int matchingLines = 0;
    int totalLines = 0;

    // Skip the header lines
    std::getline(analyzed, analyzedLine);
    std::getline(answers, answersLine);

    while (std::getline(analyzed, analyzedLine) && std::getline(answers, answersLine)) {
        std::istringstream analyzedStream(analyzedLine);
        std::istringstream answersStream(answersLine);

        int analyzedSentiment, answersSentiment;
        std::string analyzedId, answersId;

        // Read the CSV fields
        analyzedStream >> analyzedSentiment;
        analyzedStream.ignore(1, ',');
        std::getline(analyzedStream, analyzedId);

        answersStream >> answersSentiment;
        answersStream.ignore(1, ',');
        std::getline(answersStream, answersId);

        if (analyzedId != answersId) {
            std::cerr << "ID mismatch at line " << totalLines + 1 << std::endl;
            break;
        }

        if (analyzedSentiment == answersSentiment) {
            matchingLines++;
        }

        totalLines++;
    }

    analyzed.close();
    answers.close();

    if (totalLines == 0) {
        return 0.0;
    }

    return (static_cast<double>(matchingLines) / totalLines) * 100.0;
}

std::vector<std::string> SentimentAnalyzer::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::istringstream stream(text);
    std::string word;
    while (stream >> word) {
        // Remove punctuation and convert to lowercase
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        tokens.push_back(word);
    }
    return tokens;
}