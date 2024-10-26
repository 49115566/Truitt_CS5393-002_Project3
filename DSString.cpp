#include "DSString.h" // Include the header file for DSString class

// Helper functions
size_t cstr_length(const char *str) { // Function to calculate the length of a C-string
    if (str == nullptr) return 0; // Return 0 if the string is null
    size_t length = 0; // Initialize length to 0
    while (str[length] != '\0') { // Loop until the end of the string
        ++length; // Increment length for each character
    }
    return length; // Return the calculated length
}

void cstr_copy(char *dest, const char *src, size_t length) { // Function to copy a C-string
    if (dest == nullptr || src == nullptr) return; // Return if either destination or source is null
    for (size_t i = 0; i < length; ++i) { // Loop through each character
        dest[i] = src[i]; // Copy character from source to destination
    }
    dest[length] = '\0'; // Null-terminate the destination string
}

// Constructors and Destructor
DSString::DSString() : data(nullptr), len(0) {} // Default constructor initializing data to nullptr and length to 0

DSString::DSString(const char *str) { // Constructor from C-string
    if (str) { // If the input string is not null
        len = cstr_length(str); // Calculate the length of the input string
        data = new (std::nothrow) char[len + 1]; // Allocate memory for the new string
        if (data != nullptr) { // If memory allocation is successful
            cstr_copy(data, str, len); // Copy the input string to data
        } else { // If memory allocation fails
            len = 0; // Set length to 0
        }
    } else { // If the input string is null
        data = nullptr; // Set data to nullptr
        len = 0; // Set length to 0
    }
}

DSString::DSString(const DSString &other) : len(other.len) { // Copy constructor
    data = new char[len + 1]; // Allocate memory for the new string
    cstr_copy(data, other.data, len); // Copy the other string to data
}

DSString::DSString(size_t n, char c) : len(n) { // Constructor to create a string with n copies of character c
    data = new char[len + 1]; // Allocate memory for the new string
    for (size_t i = 0; i < len; ++i) { // Loop through each character
        data[i] = c; // Set each character to c
    }
    data[len] = '\0'; // Null-terminate the string
}

DSString &DSString::operator=(const DSString &other) { // Assignment operator
    if (this != &other) { // Check for self-assignment
        delete[] data; // Delete the existing data
        len = other.len; // Set the new length
        data = new char[len + 1]; // Allocate memory for the new string
        cstr_copy(data, other.data, len); // Copy the other string to data
    }
    return *this; // Return the current object
}

DSString::~DSString() { // Destructor
    delete[] data; // Delete the allocated memory
}

// New functions for compatibility with std::string
DSString::DSString(const std::string &str) { // Constructor from std::string
    len = str.length(); // Get the length of the std::string
    data = new char[len + 1]; // Allocate memory for the new string
    cstr_copy(data, str.c_str(), len); // Copy the std::string to data
}

DSString &DSString::operator=(const std::string &str) { // Assignment operator from std::string
    delete[] data; // Delete the existing data
    len = str.length(); // Get the length of the std::string
    data = new char[len + 1]; // Allocate memory for the new string
    cstr_copy(data, str.c_str(), len); // Copy the std::string to data
    return *this; // Return the current object
}

DSString::operator std::string() const { // Conversion operator to std::string
    return std::string(data); // Return a std::string constructed from data
}

void DSString::clear() { // Function to clear the string
    delete[] data; // Delete the allocated memory
    data = nullptr; // Set data to nullptr
    len = 0; // Set length to 0
}

bool DSString::empty() const { // Function to check if the string is empty
    return len == 0; // Return true if length is 0, false otherwise
}

size_t DSString::find(const DSString &str, size_t pos) const { // Function to find a substring
    if (pos >= len) return std::string::npos; // Return npos if position is out of bounds
    const char *found = std::strstr(data + pos, str.data); // Find the substring starting from pos
    return found ? found - data : std::string::npos; // Return the position of the found substring or npos
}

size_t DSString::find(const char *s, size_t pos) const { // Function to find a C-string
    if (pos >= len) return std::string::npos; // Return npos if position is out of bounds
    const char *found = std::strstr(data + pos, s); // Find the C-string starting from pos
    return found ? found - data : std::string::npos; // Return the position of the found C-string or npos
}

size_t DSString::find(char c, size_t pos) const { // Function to find a character
    if (pos >= len) return std::string::npos; // Return npos if position is out of bounds
    const char *found = std::strchr(data + pos, c); // Find the character starting from pos
    return found ? found - data : std::string::npos; // Return the position of the found character or npos
}

DSString &DSString::append(const DSString &str) { // Append a DSString to the current string
    char *newData = new char[len + str.len + 1]; // Allocate new memory for the combined string
    cstr_copy(newData, data, len); // Copy the current string to newData
    cstr_copy(newData + len, str.data, str.len); // Copy the appended string to newData
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len += str.len; // Update the length
    return *this; // Return the current object
}

DSString &DSString::append(const char *s) { // Append a C-string to the current string
    size_t s_len = cstr_length(s); // Calculate the length of the C-string
    char *newData = new char[len + s_len + 1]; // Allocate new memory for the combined string
    cstr_copy(newData, data, len); // Copy the current string to newData
    cstr_copy(newData + len, s, s_len); // Copy the appended C-string to newData
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len += s_len; // Update the length
    return *this; // Return the current object
}

DSString &DSString::append(size_t n, char c) { // Append n copies of character c to the current string
    char *newData = new char[len + n + 1]; // Allocate new memory for the combined string
    cstr_copy(newData, data, len); // Copy the current string to newData
    for (size_t i = 0; i < n; ++i) { // Loop through each character to append
        newData[len + i] = c; // Set each character to c
    }
    newData[len + n] = '\0'; // Null-terminate the new string
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len += n; // Update the length
    return *this; // Return the current object
}

DSString &DSString::erase(size_t pos, size_t length) { // Erase a portion of the string
    if (pos >= len) return *this; // Return if position is out of bounds
    if (length == std::string::npos || pos + length > len) { // Adjust length if it exceeds bounds
        length = len - pos; // Set length to the remaining length from pos
    }
    char *newData = new char[len - length + 1]; // Allocate new memory for the shortened string
    cstr_copy(newData, data, pos); // Copy the part before the erased portion
    cstr_copy(newData + pos, data + pos + length, len - pos - length); // Copy the part after the erased portion
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len -= length; // Update the length
    return *this; // Return the current object
}

DSString &DSString::replace(size_t pos, size_t length, const DSString &str) { // Replace a portion of the string with another DSString
    erase(pos, length); // Erase the specified portion
    insert(pos, str); // Insert the new string at the specified position
    return *this; // Return the current object
}

DSString &DSString::replace(size_t pos, size_t length, const char *s) { // Replace a portion of the string with a C-string
    erase(pos, length); // Erase the specified portion
    insert(pos, s); // Insert the new C-string at the specified position
    return *this; // Return the current object
}

DSString &DSString::insert(size_t pos, const DSString &str) { // Insert a DSString at the specified position
    if (pos > len) pos = len; // Adjust position if it exceeds bounds
    char *newData = new char[len + str.len + 1]; // Allocate new memory for the combined string
    cstr_copy(newData, data, pos); // Copy the part before the insertion point
    cstr_copy(newData + pos, str.data, str.len); // Copy the inserted string
    cstr_copy(newData + pos + str.len, data + pos, len - pos); // Copy the part after the insertion point
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len += str.len; // Update the length
    return *this; // Return the current object
}

DSString &DSString::insert(size_t pos, const char *s) { // Insert a C-string at the specified position
    size_t s_len = cstr_length(s); // Calculate the length of the C-string
    if (pos > len) pos = len; // Adjust position if it exceeds bounds
    char *newData = new char[len + s_len + 1]; // Allocate new memory for the combined string
    cstr_copy(newData, data, pos); // Copy the part before the insertion point
    cstr_copy(newData + pos, s, s_len); // Copy the inserted C-string
    cstr_copy(newData + pos + s_len, data + pos, len - pos); // Copy the part after the insertion point
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len += s_len; // Update the length
    return *this; // Return the current object
}

DSString &DSString::insert(size_t pos, size_t n, char c) { // Insert n copies of character c at the specified position
    if (pos > len) pos = len; // Adjust position if it exceeds bounds
    char *newData = new char[len + n + 1]; // Allocate new memory for the combined string
    cstr_copy(newData, data, pos); // Copy the part before the insertion point
    for (size_t i = 0; i < n; ++i) { // Loop through each character to insert
        newData[pos + i] = c; // Set each character to c
    }
    cstr_copy(newData + pos + n, data + pos, len - pos); // Copy the part after the insertion point
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len += n; // Update the length
    return *this; // Return the current object
}

DSString &DSString::erase(char *first, char *last) { // Erase a range of characters from the string
    if (first < data || last > data + len || first >= last) { // Check if the range is valid
        return *this; // Return if the range is invalid
    }
    size_t newLen = len - (last - first); // Calculate the new length
    char *newData = new char[newLen + 1]; // Allocate new memory for the shortened string
    size_t i = 0; // Initialize index for newData
    for (char *it = data; it < first; ++it, ++i) { // Copy the part before the erased range
        newData[i] = *it; // Copy character
    }
    for (char *it = last; it < data + len; ++it, ++i) { // Copy the part after the erased range
        newData[i] = *it; // Copy character
    }
    newData[newLen] = '\0'; // Null-terminate the new string
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len = newLen; // Update the length
    return *this; // Return the current object
}

// Additional functions to support Trie.cpp usage
DSString &DSString::erase(std::function<bool(char)> pred) { // Erase characters based on a predicate
    char *newData = new char[len + 1]; // Allocate new memory for the modified string
    size_t newLen = 0; // Initialize new length to 0
    for (size_t i = 0; i < len; ++i) { // Loop through each character
        if (!pred(data[i])) { // If the character does not match the predicate
            newData[newLen++] = data[i]; // Copy the character to newData
        }
    }
    newData[newLen] = '\0'; // Null-terminate the new string
    delete[] data; // Delete the old data
    data = newData; // Update data to point to newData
    len = newLen; // Update the length
    return *this; // Return the current object
}

char *DSString::begin() { // Return a pointer to the beginning of the string
    return data; // Return data
}

char *DSString::end() { // Return a pointer to the end of the string
    return data + len; // Return data + len
}

const char *DSString::begin() const { // Return a const pointer to the beginning of the string
    return data; // Return data
}

const char *DSString::end() const { // Return a const pointer to the end of the string
    return data + len; // Return data + len
}

// Existing functions
size_t DSString::length() const { // Return the length of the string
    return len; // Return len
}

char &DSString::operator[](size_t index) { // Return a reference to the character at the specified index
    //if (index >= len) throw std::out_of_range("Index out of range");
    return data[index]; // Return data[index]
}

DSString DSString::operator+(const DSString &other) const { // Concatenate two DSString objects
    DSString result; // Create a new DSString object
    result.len = len + other.len; // Set the length of the result
    result.data = new char[result.len + 1]; // Allocate memory for the result
    cstr_copy(result.data, data, len); // Copy the current string to result
    cstr_copy(result.data + len, other.data, other.len); // Copy the other string to result
    return result; // Return the result
}

DSString DSString::operator+(char c) const { // Concatenate a character to the DSString
    DSString result; // Create a new DSString object
    result.len = len + 1; // Set the length of the result
    result.data = new char[result.len + 1]; // Allocate memory for the result
    cstr_copy(result.data, data, len); // Copy the current string to result
    result.data[len] = c; // Set the last character to c
    result.data[len + 1] = '\0'; // Null-terminate the result
    return result; // Return the result
}

bool DSString::operator==(const DSString &other) const { // Check if two DSString objects are equal
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

bool DSString::operator!=(const DSString &other) const { // Check if two DSString objects are not equal
    if (len != other.len) { // If lengths are not equal
        return true; // Return true
    }
    for (size_t i = 0; i < len; ++i) { // Loop through each character
        if (data[i] != other.data[i]) { // If characters are not equal
            return true; // Return true
        }
    }
    return false; // Return false if all characters are equal
}

bool DSString::operator<(const DSString &other) const { // Check if the current DSString is less than the other
    size_t minLength = len < other.len ? len : other.len; // Get the minimum length
    for (size_t i = 0; i < minLength; ++i) { // Loop through each character
        if (data[i] < other.data[i]) { // If the current character is less than the other
            return true; // Return true
        } else if (data[i] > other.data[i]) { // If the current character is greater than the other
            return false; // Return false
        }
    }
    return len < other.len; // Return true if the current length is less than the other
}

DSString &DSString::operator+=(const char *s) { // Append a C-string to the current DSString
    return this->append(s); // Call the append function and return the result
}

DSString &DSString::operator+=(const DSString &s) { // Append a DSString to the current DSString
    return this->append(s); // Call the append function and return the result
}

DSString DSString::substring(size_t start, size_t numChars) const { // Get a substring of the DSString
    DSString result; // Create a new DSString object
    if (start < len) { // If the start position is within bounds
        result.len = (numChars < len - start) ? numChars : len - start; // Set the length of the result
        result.data = new char[result.len + 1]; // Allocate memory for the result
        for (size_t i = 0; i < result.len; ++i) { // Loop through each character
            result.data[i] = data[start + i]; // Copy the character to result
        }
        result.data[result.len] = '\0'; // Null-terminate the result
    }
    return result; // Return the result
}

DSString DSString::toLower() const { // Convert the string to lowercase
    DSString result(*this); // Create a copy of the current string
    for (size_t i = 0; i < len; ++i) { // Loop through each character
        result.data[i] = std::tolower(result.data[i]); // Convert the character to lowercase
    }
    return result; // Return the result
}

const char *DSString::c_str() const { // Return a C-string representation of the DSString
    return data; // Return data
}

std::ostream &operator<<(std::ostream &os, const DSString &str) { // Output stream operator for DSString
    os << str.data; // Output the string data
    return os; // Return the output stream
}

std::istream &operator>>(std::istream &is, DSString &str) { // Input stream operator for DSString
    char buffer[1024]; // Buffer to hold the input
    is >> buffer; // Read input into the buffer
    str = DSString(buffer); // Assign the buffer to the DSString
    return is; // Return the input stream
}

std::istream &getline(std::istream &is, DSString &str, char delim) { // Get a line from the input stream with a delimiter
    std::string temp; // Temporary string to hold the input
    std::getline(is, temp, delim); // Read input into the temporary string
    str = DSString(temp.c_str()); // Assign the temporary string to the DSString
    return is; // Return the input stream
}

std::istream &getline(std::istream &is, DSString &str) { // Get a line from the input stream
    std::string temp; // Temporary string to hold the input
    std::getline(is, temp); // Read input into the temporary string
    str = DSString(temp.c_str()); // Assign the temporary string to the DSString
    return is; // Return the input stream
}