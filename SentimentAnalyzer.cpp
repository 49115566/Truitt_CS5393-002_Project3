#include "SentimentAnalyzer.h"

SentimentAnalyzer::SentimentAnalyzer(const DSString& saveFile, const DSString& trainFile) {
    std::ifstream file(saveFile.c_str());
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

double SentimentAnalyzer::analyzeSentiment(const DSString& text) const {
    std::vector<DSString> words = trie.tokenize(text);
    double logOddsSum = 0.0;
    for (const DSString& word : words) {
        logOddsSum += trie.getLogOddsRatio(word);
    }
    return logOddsSum;
}

void SentimentAnalyzer::analyzeFile(const DSString& input, const DSString& output) const {
    std::ifstream inputFile(input.c_str());
    std::ofstream outputFile(output.c_str());

    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open input file");
    }

    if (!outputFile.is_open()) {
        throw std::runtime_error("Could not open output file");
    }

    std::cout << "Analyzing file..." << std::endl;
    DSString line;
    // Skip the header line
    getline(inputFile, line);
    outputFile << "Sentiment,id" << std::endl;

    while (getline(inputFile, line)) {
        std::istringstream stream(line.c_str());
        DSString id, date, query, user, tweet;

        // Read the CSV fields
        getline(stream, id, ',');
        getline(stream, date, ',');
        getline(stream, query, ',');
        getline(stream, user, ',');
        getline(stream, tweet, ',');

        double sentimentScore = analyzeSentiment(tweet);
        int sentiment = sentimentScore > 0 ? 4 : 0;

        outputFile << sentiment << "," << id << std::endl;
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Analysis complete!" << std::endl;
}

double SentimentAnalyzer::accuracy(const DSString& analyzedFile, const DSString& answersFile) const {
    std::ifstream analyzed(analyzedFile.c_str());
    std::ifstream answers(answersFile.c_str());
    std::ofstream mistakes("mistakes.csv");

    if (!analyzed.is_open()) {
        throw std::runtime_error("Could not open analyzed file");
    }

    if (!answers.is_open()) {
        throw std::runtime_error("Could not open answers file");
    }

    DSString analyzedLine, answersLine;
    int matchingLines = 0;
    int totalLines = 0;

    // Skip the header lines
    getline(analyzed, analyzedLine);
    getline(answers, answersLine);

    while (getline(analyzed, analyzedLine) && getline(answers, answersLine)) {
        std::istringstream analyzedStream(analyzedLine.c_str());
        std::istringstream answersStream(answersLine.c_str());

        int analyzedSentiment, answersSentiment;
        DSString analyzedId, answersId;

        // Read the CSV fields
        analyzedStream >> analyzedSentiment;
        analyzedStream.ignore(1, ',');
        getline(analyzedStream, analyzedId);

        answersStream >> answersSentiment;
        answersStream.ignore(1, ',');
        getline(answersStream, answersId);

        if (analyzedId != answersId) {
            std::cerr << "ID mismatch at line " << totalLines + 1 << std::endl;
            break;
        }

        if (analyzedSentiment == answersSentiment) {
            matchingLines++;
        }
        else{
            mistakes << analyzedSentiment << "," << analyzedId << std::endl;
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