#ifndef DSSTRING_H // Include guard to prevent multiple inclusions
#define DSSTRING_H // Include guard to prevent multiple inclusions

#include <iostream> // Include iostream for input/output operations
#include <string> // Include string for standard string operations
#include <algorithm> // Include algorithm for standard algorithms
#include <functional> // Include functional for function objects
#include <cctype> // Include cctype for character handling functions
#include <cstring> // Include cstring for C-style string functions
#include <stdexcept> // Include stdexcept for standard exceptions
#include <iterator> // Include iterator for iterator support

/**
 * @class DSString
 * @brief A custom string class that provides various string manipulation functionalities.
 * 
 * The DSString class is designed to handle strings with a variety of operations such as 
 * concatenation, comparison, substring extraction, and more. It supports both C-style 
 * strings and std::string for initialization and assignment.
 * 
 * @note This class manages its own memory and provides deep copy semantics.
 */
class DSString {
private:
    char *data; ///< Pointer to character data
    size_t len; ///< Length of the string

public:
    /**
     * @brief Default constructor. Initializes an empty string.
     */
    DSString();

    /**
     * @brief Constructor from C-string.
     * @param str C-string to initialize the DSString object.
     */
    DSString(const char *str);

    /**
     * @brief Copy constructor.
     * @param other DSString object to copy from.
     */
    DSString(const DSString &other);

    /**
     * @brief Constructor with size and character.
     * @param size Number of characters.
     * @param ch Character to fill the string with.
     */
    DSString(size_t size, char ch);

    /**
     * @brief Copy assignment operator.
     * @param other DSString object to assign from.
     * @return Reference to the assigned DSString object.
     */
    DSString &operator=(const DSString &other);

    /**
     * @brief Destructor. Releases allocated memory.
     */
    ~DSString();

    /**
     * @brief Get the length of the string.
     * @return Length of the string.
     */
    size_t length() const;

    /**
     * @brief Subscript operator.
     * @param index Position of the character.
     * @return Reference to the character at the specified position.
     */
    char &operator[](size_t index);

    /**
     * @brief Concatenation operator.
     * @param other DSString object to concatenate.
     * @return New DSString object resulting from concatenation.
     */
    DSString operator+(const DSString &other) const;

    /**
     * @brief Concatenation with character.
     * @param ch Character to concatenate.
     * @return New DSString object resulting from concatenation.
     */
    DSString operator+(char ch) const;

    /**
     * @brief Equality operator.
     * @param other DSString object to compare.
     * @return True if strings are equal, false otherwise.
     */
    bool operator==(const DSString &other) const;

    /**
     * @brief Inequality operator.
     * @param other DSString object to compare.
     * @return True if strings are not equal, false otherwise.
     */
    bool operator!=(const DSString &other) const;

    /**
     * @brief Less-than operator.
     * @param other DSString object to compare.
     * @return True if this string is less than the other, false otherwise.
     */
    bool operator<(const DSString &other) const;

    /**
     * @brief Get substring.
     * @param start Starting position.
     * @param numChars Number of characters to extract.
     * @return Substring as a new DSString object.
     */
    DSString substring(size_t start, size_t numChars) const;

    /**
     * @brief Convert to lowercase.
     * @return New DSString object with all characters in lowercase.
     */
    DSString toLower() const;

    /**
     * @brief Get C-string.
     * @return Pointer to the C-string.
     */
    const char *c_str() const;

    /**
     * @brief Append C-string.
     * @param str C-string to append.
     * @return Reference to the modified DSString object.
     */
    DSString &operator+=(const char *str);

    /**
     * @brief Append DSString.
     * @param other DSString object to append.
     * @return Reference to the modified DSString object.
     */
    DSString &operator+=(const DSString &other);

    /**
     * @brief Constructor from std::string.
     * @param str std::string to initialize the DSString object.
     */
    DSString(const std::string &str);

    /**
     * @brief Assignment from std::string.
     * @param str std::string to assign from.
     * @return Reference to the assigned DSString object.
     */
    DSString &operator=(const std::string &str);

    /**
     * @brief Conversion to std::string.
     * @return std::string representation of the DSString object.
     */
    operator std::string() const;

    /**
     * @brief Clear the string.
     */
    void clear();

    /**
     * @brief Check if the string is empty.
     * @return True if the string is empty, false otherwise.
     */
    bool empty() const;

    /**
     * @brief Find substring.
     * @param str Substring to find.
     * @param pos Starting position for the search.
     * @return Position of the first occurrence, or npos if not found.
     */
    size_t find(const DSString &str, size_t pos = 0) const;

    /**
     * @brief Find C-string.
     * @param s C-string to find.
     * @param pos Starting position for the search.
     * @return Position of the first occurrence, or npos if not found.
     */
    size_t find(const char *s, size_t pos = 0) const;

    /**
     * @brief Find character.
     * @param c Character to find.
     * @param pos Starting position for the search.
     * @return Position of the first occurrence, or npos if not found.
     */
    size_t find(char c, size_t pos = 0) const;

    /**
     * @brief Append DSString.
     * @param str DSString object to append.
     * @return Reference to the modified DSString object.
     */
    DSString &append(const DSString &str);

    /**
     * @brief Append C-string.
     * @param s C-string to append.
     * @return Reference to the modified DSString object.
     */
    DSString &append(const char *s);

    /**
     * @brief Append character multiple times.
     * @param n Number of times to append the character.
     * @param c Character to append.
     * @return Reference to the modified DSString object.
     */
    DSString &append(size_t n, char c);

    /**
     * @brief Erase part of the string.
     * @param pos Starting position.
     * @param len Number of characters to erase.
     * @return Reference to the modified DSString object.
     */
    DSString &erase(size_t pos = 0, size_t len = std::string::npos);

    /**
     * @brief Replace part of the string with DSString.
     * @param pos Starting position.
     * @param len Number of characters to replace.
     * @param str DSString object to replace with.
     * @return Reference to the modified DSString object.
     */
    DSString &replace(size_t pos, size_t len, const DSString &str);

    /**
     * @brief Replace part of the string with C-string.
     * @param pos Starting position.
     * @param len Number of characters to replace.
     * @param s C-string to replace with.
     * @return Reference to the modified DSString object.
     */
    DSString &replace(size_t pos, size_t len, const char *s);

    /**
     * @brief Insert DSString.
     * @param pos Position to insert at.
     * @param str DSString object to insert.
     * @return Reference to the modified DSString object.
     */
    DSString &insert(size_t pos, const DSString &str);

    /**
     * @brief Insert C-string.
     * @param pos Position to insert at.
     * @param s C-string to insert.
     * @return Reference to the modified DSString object.
     */
    DSString &insert(size_t pos, const char *s);

    /**
     * @brief Insert character multiple times.
     * @param pos Position to insert at.
     * @param n Number of times to insert the character.
     * @param c Character to insert.
     * @return Reference to the modified DSString object.
     */
    DSString &insert(size_t pos, size_t n, char c);

    /**
     * @brief Erase range.
     * @param first Iterator to the first character.
     * @param last Iterator to the last character.
     * @return Reference to the modified DSString object.
     */
    DSString &erase(char *first, char *last);

    /**
     * @brief Erase with predicate.
     * @param pred Predicate function to determine which characters to erase.
     * @return Reference to the modified DSString object.
     */
    DSString &erase(std::function<bool(char)> pred);

    /**
     * @brief Get iterator to beginning.
     * @return Iterator to the beginning of the string.
     */
    char *begin();

    /**
     * @brief Get iterator to end.
     * @return Iterator to the end of the string.
     */
    char *end();

    /**
     * @brief Get const iterator to beginning.
     * @return Const iterator to the beginning of the string.
     */
    const char *begin() const;

    /**
     * @brief Get const iterator to end.
     * @return Const iterator to the end of the string.
     */
    const char *end() const;

    /**
     * @brief Output stream operator.
     * @param os Output stream.
     * @param str DSString object to output.
     * @return Reference to the output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, const DSString &str);

    /**
     * @brief Input stream operator.
     * @param is Input stream.
     * @param str DSString object to input.
     * @return Reference to the input stream.
     */
    friend std::istream &operator>>(std::istream &is, DSString &str);

    /**
     * @brief Get line with delimiter.
     * @param is Input stream.
     * @param str DSString object to store the line.
     * @param delim Delimiter character.
     * @return Reference to the input stream.
     */
    friend std::istream &getline(std::istream &is, DSString &str, char delim);

    /**
     * @brief Get line.
     * @param is Input stream.
     * @param str DSString object to store the line.
     * @return Reference to the input stream.
     */
    friend std::istream &getline(std::istream &is, DSString &str);
};

#endif // End of include guard
