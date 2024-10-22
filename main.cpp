#include <iostream>
#include <exception>
#include "Trie.h"

int main() {
    try {
        std::cout << "Testing the trie!" << std::endl;
        Trie trie;
        trie.insert("hello", true);
        trie.insert("world", false);
        trie.insert("hello", false);
        trie.insert("world", true);
        trie.insert("hello", true);
        trie.insert("world", false);
        trie.insert("hello", true);
        trie.insert("world", false);

        std::cout << "1: hello: " << trie.getSentimentScore("hello") << std::endl;
        std::cout << "1: world: " << trie.getSentimentScore("world") << std::endl;

        trie.save("trie.dat");

        Trie trie2;
        trie2.load("trie.dat");

        std::cout << "2: hello: " << trie2.getSentimentScore("hello") << std::endl;
        std::cout << "2: world: " << trie2.getSentimentScore("world") << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return -1;
    }

    return 0;
}