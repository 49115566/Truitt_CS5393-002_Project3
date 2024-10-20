#include "DSString.h" // Include the header file for the DSString class
#include <cctype> // Include the cctype library for character handling functions

// Helper function to calculate the length of a c-string
size_t cstr_length(const char *str) {
    if (str == nullptr) return 0; // Handle null pointer
    size_t length = 0; // Initialize length to 0
    while (str[length] != '\0') { // Loop until the null terminator is found
        ++length; // Increment length
    }
    return length; // Return the length of the c-string
}

// Helper function to copy c-string
void cstr_copy(char *dest, const char *src, size_t length) {
    if (dest == nullptr || src == nullptr) return; // Handle null pointers
    for (size_t i = 0; i < length; ++i) { // Loop through each character
        dest[i] = src[i]; // Copy character from src to dest
    }
    dest[length] = '\0'; // Add null terminator to the end of dest
}

// Default constructor
DSString::DSString() : data(nullptr), len(0) {} // Initialize data to nullptr and len to 0

// Constructor from c-string
DSString::DSString(const char *str) {
    if (str) { // If str is not null
        len = cstr_length(str); // Calculate the length of str
        data = new (std::nothrow) char[len + 1]; // Allocate memory for data with error handling
        if (data != nullptr) { // Check if memory allocation was successful
            cstr_copy(data, str, len); // Copy str to data
        } else {
            len = 0; // Set len to 0 if memory allocation failed
        }
    } else { // If str is null
        data = nullptr; // Set data to nullptr
        len = 0; // Set len to 0
    }
}

// Copy constructor
DSString::DSString(const DSString &other) : len(other.len) { // Initialize len with other's len
    data = new char[len + 1]; // Allocate memory for data
    cstr_copy(data, other.data, len); // Copy other's data to this data
}

// Copy assignment operator
DSString &DSString::operator=(const DSString &other) {
    if (this != &other) { // Check for self-assignment
        delete[] data; // Delete existing data
        len = other.len; // Set len to other's len
        data = new char[len + 1]; // Allocate memory for data
        cstr_copy(data, other.data, len); // Copy other's data to this data
    }
    return *this; // Return *this
}

// Destructor
DSString::~DSString() {
    delete[] data; // Delete data
}

// Returns the length of the string
size_t DSString::length() const {
    return len; // Return len
}

// Returns a reference to the character at the given index
char &DSString::operator[](size_t index) {
    return data[index]; // Return reference to the character at index
}

// Overloaded operator+ which appends the string in the argument to this string
DSString DSString::operator+(const DSString &other) const {
    DSString result; // Create a new DSString object
    result.len = len + other.len; // Set result's length to the sum of this length and other's length
    result.data = new char[result.len + 1]; // Allocate memory for result's data
    cstr_copy(result.data, data, len); // Copy this data to result's data
    cstr_copy(result.data + len, other.data, other.len); // Append other's data to result's data
    return result; // Return the result
}

// Standard relational operators to compare and order your strings
bool DSString::operator==(const DSString &other) const {
    if (len != other.len) { // If lengths are not equal
        return false; // Return false
    }
    for (size_t i = 0; i < len; ++i) { // Loop through each character
        if (data[i] != other.data[i]) { // If characters are not equal
            return false; // Return false
        }
    }
    return true; // Return true if all characters are equal
}

bool DSString::operator<(const DSString &other) const {
    size_t minLength = len < other.len ? len : other.len; // Find the minimum length
    for (size_t i = 0; i < minLength; ++i) { // Loop through each character
        if (data[i] < other.data[i]) { // If this character is less than other's character
            return true; // Return true
        } else if (data[i] > other.data[i]) { // If this character is greater than other's character
            return false; // Return false
        }
    }
    return len < other.len; // Return true if this length is less than other's length
}

// The substring method returns a new string object that contains a sequence of characters from this string object
DSString DSString::substring(size_t start, size_t numChars) const {
    DSString result; // Create a new DSString object
    if (start < len) { // If start is within bounds
        result.len = (numChars < len - start) ? numChars : len - start; // Calculate the length of the substring
        result.data = new char[result.len + 1]; // Allocate memory for result's data
        for (size_t i = 0; i < result.len; ++i) { // Loop through each character
            result.data[i] = data[start + i]; // Copy character from this data to result's data
        }
        result.data[result.len] = '\0'; // Add null terminator to the end of result's data
    }
    return result; // Return the result
}

// Returns a new string object with all characters in lowercase
DSString DSString::toLower() const {
    DSString result(*this); // Create a copy of this string
    for (size_t i = 0; i < len; ++i) { // Loop through each character
        result.data[i] = std::tolower(result.data[i]); // Convert character to lowercase
    }
    return result; // Return the result
}

// The c_str function returns a pointer to a null-terminated c-string holding the contents of this object
const char *DSString::c_str() const {
    return data; // Return data
}

// Overloaded stream insertion operator to print the contents of this string to the output stream
std::ostream &operator<<(std::ostream &os, const DSString &str) {
    os << str.data; // Output the data to the stream
    return os; // Return the stream
}
