#ifndef DSSTRING_H
#define DSSTRING_H

#include <iostream>
#include <string> // Include this for compatibility with std::string
#include <algorithm> // Include this for std::remove_if
#include <functional> // Include this for std::function
#include <cctype>
#include <cstring>
#include <stdexcept>
#include <iterator>

class DSString
{
private:
    char *data;
    size_t len;

public:
    DSString();
    DSString(const char *);
    DSString(const DSString &);
    DSString(size_t, char); // Constructor with size_t and char
    DSString &operator=(const DSString &);
    ~DSString();

    size_t length() const;
    char &operator[](size_t);
    DSString operator+(const DSString &) const;
    DSString operator+(char) const; // Operator+ with char
    bool operator==(const DSString &) const;
    bool operator!=(const DSString &) const;
    bool operator<(const DSString &) const;
    DSString substring(size_t start, size_t numChars) const;
    DSString toLower() const;
    const char *c_str() const;
    DSString &operator+=(const char *); // Declaration for word += "not"

    // New functions for compatibility with std::string
    DSString(const std::string &); // Constructor from std::string
    DSString &operator=(const std::string &); // Assignment from std::string
    operator std::string() const; // Conversion to std::string
    void clear();
    bool empty() const;
    size_t find(const DSString &str, size_t pos = 0) const;
    size_t find(const char *s, size_t pos = 0) const;
    size_t find(char c, size_t pos = 0) const;
    DSString &append(const DSString &str);
    DSString &append(const char *s);
    DSString &append(size_t n, char c);
    DSString &erase(size_t pos = 0, size_t len = std::string::npos);
    DSString &replace(size_t pos, size_t len, const DSString &str);
    DSString &replace(size_t pos, size_t len, const char *s);
    DSString &insert(size_t pos, const DSString &str);
    DSString &insert(size_t pos, const char *s);
    DSString &insert(size_t pos, size_t n, char c);

    // Additional functions to support Trie.cpp usage
    DSString &erase(char *first, char *last); // Erase with iterators
    // Function to support std::remove_if usage
    DSString &erase(std::function<bool(char)> pred); // Erase with predicate
    char *begin(); // Begin iterator
    char *end(); // End iterator
    const char *begin() const; // Const begin iterator
    const char *end() const; // Const end iterator

    friend std::ostream &operator<<(std::ostream &, const DSString &);
    friend std::istream &operator>>(std::istream &, DSString &);

    // Friend function declaration for getline
    friend std::istream &getline(std::istream &is, DSString &str, char delim);
    friend std::istream &getline(std::istream &is, DSString &str);
};

#endif
