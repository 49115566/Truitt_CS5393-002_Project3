#include <iostream>
#include <exception>
#include "Trie.h"

int main() {
    try {
        std::cout << "Testing the trie!" << std::endl;
        Trie trie;
        trie.insert("hello", true);
        trie.insert("world", false);
        trie.insert("ThisString", false);
        trie.insert("anotherString", true);
        trie.insert("text", true);
        trie.insert("example", false);
        trie.insert("My", true);
        trie.insert("Name", false);
        trie.insert("Is", true);
        trie.insert("Trie", false);
        trie.insert("I", true);
        trie.insert("Am", false);
        trie.insert("A", true);
        trie.insert("Data", false);
        trie.insert("Structure", true);
        trie.insert("That", false);
        trie.insert("Stores", true);
        trie.insert("Strings", false);
        trie.insert("And", true);
        trie.insert("Counts", false);
        trie.insert("Positive", true);
        trie.insert("Numbers", false);
        trie.insert("For", true);
        trie.insert("Each", false);
        trie.insert("String", true);
        trie.insert("Inserted", false);
        trie.insert("Into", true);
        trie.insert("The", false);
        trie.insert("DS", true);
        trie.insert("If", false);
        trie.insert("You", true);
        trie.insert("Are", false);
        trie.insert("Reading", true);
        trie.insert("This", false);
        trie.insert("Please", true);
        trie.insert("Stop", false);
        trie.insert("Now", true);
        trie.insert("Jeremy", false);
        trie.insert("Will", true);
        trie.insert("Be", false);
        trie.insert("Mad", true);

        std::cout << "1: hello: " << trie.getSentimentScore("hello") << std::endl;
        std::cout << "1: world: " << trie.getSentimentScore("world") << std::endl;
        std::cout << "1: ThisString: " << trie.getSentimentScore("ThisString") << std::endl;
        std::cout << "1: anotherString: " << trie.getSentimentScore("anotherString") << std::endl;
        std::cout << "1: text: " << trie.getSentimentScore("text") << std::endl;
        std::cout << "1: example: " << trie.getSentimentScore("example") << std::endl;
        std::cout << "1: My: " << trie.getSentimentScore("My") << std::endl;
        std::cout << "1: Name: " << trie.getSentimentScore("Name") << std::endl;
        std::cout << "1: Is: " << trie.getSentimentScore("Is") << std::endl;
        std::cout << "1: Trie: " << trie.getSentimentScore("Trie") << std::endl;
        std::cout << "1: I: " << trie.getSentimentScore("I") << std::endl;
        std::cout << "1: Am: " << trie.getSentimentScore("Am") << std::endl;
        std::cout << "1: A: " << trie.getSentimentScore("A") << std::endl;
        std::cout << "1: Data: " << trie.getSentimentScore("Data") << std::endl;
        std::cout << "1: Structure: " << trie.getSentimentScore("Structure") << std::endl;
        std::cout << "1: That: " << trie.getSentimentScore("That") << std::endl;
        std::cout << "1: Stores: " << trie.getSentimentScore("Stores") << std::endl;
        std::cout << "1: Strings: " << trie.getSentimentScore("Strings") << std::endl;
        std::cout << "1: And: " << trie.getSentimentScore("And") << std::endl;
        std::cout << "1: Counts: " << trie.getSentimentScore("Counts") << std::endl;
        std::cout << "1: Positive: " << trie.getSentimentScore("Positive") << std::endl;
        std::cout << "1: Numbers: " << trie.getSentimentScore("Numbers") << std::endl;
        std::cout << "1: For: " << trie.getSentimentScore("For") << std::endl;
        std::cout << "1: Each: " << trie.getSentimentScore("Each") << std::endl;
        std::cout << "1: String: " << trie.getSentimentScore("String") << std::endl;
        std::cout << "1: Inserted: " << trie.getSentimentScore("Inserted") << std::endl;
        std::cout << "1: Into: " << trie.getSentimentScore("Into") << std::endl;
        std::cout << "1: The: " << trie.getSentimentScore("The") << std::endl;
        std::cout << "1: DS: " << trie.getSentimentScore("DS") << std::endl;
        std::cout << "1: If: " << trie.getSentimentScore("If") << std::endl;
        std::cout << "1: You: " << trie.getSentimentScore("You") << std::endl;
        std::cout << "1: Are: " << trie.getSentimentScore("Are") << std::endl;
        std::cout << "1: Reading: " << trie.getSentimentScore("Reading") << std::endl;
        std::cout << "1: This: " << trie.getSentimentScore("This") << std::endl;
        std::cout << "1: Please: " << trie.getSentimentScore("Please") << std::endl;
        std::cout << "1: Stop: " << trie.getSentimentScore("Stop") << std::endl;
        std::cout << "1: Now: " << trie.getSentimentScore("Now") << std::endl;
        std::cout << "1: Jeremy: " << trie.getSentimentScore("Jeremy") << std::endl;
        std::cout << "1: Will: " << trie.getSentimentScore("Will") << std::endl;
        std::cout << "1: Be: " << trie.getSentimentScore("Be") << std::endl;
        std::cout << "1: Mad: " << trie.getSentimentScore("Mad") << std::endl;

        trie.save("trie.dat");

        Trie trie2;
        trie2.load("trie.dat");

                std::cout << "1: hello: " << trie.getSentimentScore("hello") << std::endl;
        std::cout << "1: world: " << trie.getSentimentScore("world") << std::endl;
        std::cout << "1: ThisString: " << trie.getSentimentScore("ThisString") << std::endl;
        std::cout << "1: anotherString: " << trie.getSentimentScore("anotherString") << std::endl;
        std::cout << "1: text: " << trie.getSentimentScore("text") << std::endl;
        std::cout << "1: example: " << trie.getSentimentScore("example") << std::endl;
        std::cout << "1: My: " << trie.getSentimentScore("My") << std::endl;
        std::cout << "1: Name: " << trie.getSentimentScore("Name") << std::endl;
        std::cout << "1: Is: " << trie.getSentimentScore("Is") << std::endl;
        std::cout << "1: Trie: " << trie.getSentimentScore("Trie") << std::endl;
        std::cout << "1: I: " << trie.getSentimentScore("I") << std::endl;
        std::cout << "1: Am: " << trie.getSentimentScore("Am") << std::endl;
        std::cout << "1: A: " << trie.getSentimentScore("A") << std::endl;
        std::cout << "1: Data: " << trie.getSentimentScore("Data") << std::endl;
        std::cout << "1: Structure: " << trie.getSentimentScore("Structure") << std::endl;
        std::cout << "1: That: " << trie.getSentimentScore("That") << std::endl;
        std::cout << "1: Stores: " << trie.getSentimentScore("Stores") << std::endl;
        std::cout << "1: Strings: " << trie.getSentimentScore("Strings") << std::endl;
        std::cout << "1: And: " << trie.getSentimentScore("And") << std::endl;
        std::cout << "1: Counts: " << trie.getSentimentScore("Counts") << std::endl;
        std::cout << "1: Positive: " << trie.getSentimentScore("Positive") << std::endl;
        std::cout << "1: Numbers: " << trie.getSentimentScore("Numbers") << std::endl;
        std::cout << "1: For: " << trie.getSentimentScore("For") << std::endl;
        std::cout << "1: Each: " << trie.getSentimentScore("Each") << std::endl;
        std::cout << "1: String: " << trie.getSentimentScore("String") << std::endl;
        std::cout << "1: Inserted: " << trie.getSentimentScore("Inserted") << std::endl;
        std::cout << "1: Into: " << trie.getSentimentScore("Into") << std::endl;
        std::cout << "1: The: " << trie.getSentimentScore("The") << std::endl;
        std::cout << "1: DS: " << trie.getSentimentScore("DS") << std::endl;
        std::cout << "1: If: " << trie.getSentimentScore("If") << std::endl;
        std::cout << "1: You: " << trie.getSentimentScore("You") << std::endl;
        std::cout << "1: Are: " << trie.getSentimentScore("Are") << std::endl;
        std::cout << "1: Reading: " << trie.getSentimentScore("Reading") << std::endl;
        std::cout << "1: This: " << trie.getSentimentScore("This") << std::endl;
        std::cout << "1: Please: " << trie.getSentimentScore("Please") << std::endl;
        std::cout << "1: Stop: " << trie.getSentimentScore("Stop") << std::endl;
        std::cout << "1: Now: " << trie.getSentimentScore("Now") << std::endl;
        std::cout << "1: Jeremy: " << trie.getSentimentScore("Jeremy") << std::endl;
        std::cout << "1: Will: " << trie.getSentimentScore("Will") << std::endl;
        std::cout << "1: Be: " << trie.getSentimentScore("Be") << std::endl;
        std::cout << "1: Mad: " << trie.getSentimentScore("Mad") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return -1;
    }

    return 0;
}