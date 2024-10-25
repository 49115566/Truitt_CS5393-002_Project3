#include "DSString.h"

// Helper functions
size_t cstr_length(const char *str) {
    if (str == nullptr) return 0;
    size_t length = 0;
    while (str[length] != '\0') {
        ++length;
    }
    return length;
}

void cstr_copy(char *dest, const char *src, size_t length) {
    if (dest == nullptr || src == nullptr) return;
    for (size_t i = 0; i < length; ++i) {
        dest[i] = src[i];
    }
    dest[length] = '\0';
}

// Constructors and Destructor
DSString::DSString() : data(nullptr), len(0) {}

DSString::DSString(const char *str) {
    if (str) {
        len = cstr_length(str);
        data = new (std::nothrow) char[len + 1];
        if (data != nullptr) {
            cstr_copy(data, str, len);
        } else {
            len = 0;
        }
    } else {
        data = nullptr;
        len = 0;
    }
}

DSString::DSString(const DSString &other) : len(other.len) {
    data = new char[len + 1];
    cstr_copy(data, other.data, len);
}

DSString::DSString(size_t n, char c) : len(n) {
    data = new char[len + 1];
    for (size_t i = 0; i < len; ++i) {
        data[i] = c;
    }
    data[len] = '\0';
}

DSString &DSString::operator=(const DSString &other) {
    if (this != &other) {
        delete[] data;
        len = other.len;
        data = new char[len + 1];
        cstr_copy(data, other.data, len);
    }
    return *this;
}

DSString::~DSString() {
    delete[] data;
}

// New functions for compatibility with std::string
DSString::DSString(const std::string &str) {
    len = str.length();
    data = new char[len + 1];
    cstr_copy(data, str.c_str(), len);
}

DSString &DSString::operator=(const std::string &str) {
    delete[] data;
    len = str.length();
    data = new char[len + 1];
    cstr_copy(data, str.c_str(), len);
    return *this;
}

DSString::operator std::string() const {
    return std::string(data);
}

void DSString::clear() {
    delete[] data;
    data = nullptr;
    len = 0;
}

bool DSString::empty() const {
    return len == 0;
}

size_t DSString::find(const DSString &str, size_t pos) const {
    if (pos >= len) return std::string::npos;
    const char *found = std::strstr(data + pos, str.data);
    return found ? found - data : std::string::npos;
}

size_t DSString::find(const char *s, size_t pos) const {
    if (pos >= len) return std::string::npos;
    const char *found = std::strstr(data + pos, s);
    return found ? found - data : std::string::npos;
}

size_t DSString::find(char c, size_t pos) const {
    if (pos >= len) return std::string::npos;
    const char *found = std::strchr(data + pos, c);
    return found ? found - data : std::string::npos;
}

DSString &DSString::append(const DSString &str) {
    char *newData = new char[len + str.len + 1];
    cstr_copy(newData, data, len);
    cstr_copy(newData + len, str.data, str.len);
    delete[] data;
    data = newData;
    len += str.len;
    return *this;
}

DSString &DSString::append(const char *s) {
    size_t s_len = cstr_length(s);
    char *newData = new char[len + s_len + 1];
    cstr_copy(newData, data, len);
    cstr_copy(newData + len, s, s_len);
    delete[] data;
    data = newData;
    len += s_len;
    return *this;
}

DSString &DSString::append(size_t n, char c) {
    char *newData = new char[len + n + 1];
    cstr_copy(newData, data, len);
    for (size_t i = 0; i < n; ++i) {
        newData[len + i] = c;
    }
    newData[len + n] = '\0';
    delete[] data;
    data = newData;
    len += n;
    return *this;
}

DSString &DSString::erase(size_t pos, size_t length) {
    if (pos >= len) return *this;
    if (length == std::string::npos || pos + length > len) {
        length = len - pos;
    }
    char *newData = new char[len - length + 1];
    cstr_copy(newData, data, pos);
    cstr_copy(newData + pos, data + pos + length, len - pos - length);
    delete[] data;
    data = newData;
    len -= length;
    return *this;
}

DSString &DSString::replace(size_t pos, size_t length, const DSString &str) {
    erase(pos, length);
    insert(pos, str);
    return *this;
}

DSString &DSString::replace(size_t pos, size_t length, const char *s) {
    erase(pos, length);
    insert(pos, s);
    return *this;
}

DSString &DSString::insert(size_t pos, const DSString &str) {
    if (pos > len) pos = len;
    char *newData = new char[len + str.len + 1];
    cstr_copy(newData, data, pos);
    cstr_copy(newData + pos, str.data, str.len);
    cstr_copy(newData + pos + str.len, data + pos, len - pos);
    delete[] data;
    data = newData;
    len += str.len;
    return *this;
}

DSString &DSString::insert(size_t pos, const char *s) {
    size_t s_len = cstr_length(s);
    if (pos > len) pos = len;
    char *newData = new char[len + s_len + 1];
    cstr_copy(newData, data, pos);
    cstr_copy(newData + pos, s, s_len);
    cstr_copy(newData + pos + s_len, data + pos, len - pos);
    delete[] data;
    data = newData;
    len += s_len;
    return *this;
}

DSString &DSString::insert(size_t pos, size_t n, char c) {
    if (pos > len) pos = len;
    char *newData = new char[len + n + 1];
    cstr_copy(newData, data, pos);
    for (size_t i = 0; i < n; ++i) {
        newData[pos + i] = c;
    }
    cstr_copy(newData + pos + n, data + pos, len - pos);
    delete[] data;
    data = newData;
    len += n;
    return *this;
}

DSString &DSString::erase(char *first, char *last) {
    if (first < data || last > data + len || first >= last) {
        return *this;
    }
    size_t newLen = len - (last - first);
    char *newData = new char[newLen + 1];
    size_t i = 0;
    for (char *it = data; it < first; ++it, ++i) {
        newData[i] = *it;
    }
    for (char *it = last; it < data + len; ++it, ++i) {
        newData[i] = *it;
    }
    newData[newLen] = '\0';
    delete[] data;
    data = newData;
    len = newLen;
    return *this;
}

// Additional functions to support Trie.cpp usage
DSString &DSString::erase(std::function<bool(char)> pred) {
    char *newData = new char[len + 1];
    size_t newLen = 0;
    for (size_t i = 0; i < len; ++i) {
        if (!pred(data[i])) {
            newData[newLen++] = data[i];
        }
    }
    newData[newLen] = '\0';
    delete[] data;
    data = newData;
    len = newLen;
    return *this;
}

char *DSString::begin() {
    return data;
}

char *DSString::end() {
    return data + len;
}

const char *DSString::begin() const {
    return data;
}

const char *DSString::end() const {
    return data + len;
}

// Existing functions
size_t DSString::length() const {
    return len;
}

char &DSString::operator[](size_t index) {
    //if (index >= len) throw std::out_of_range("Index out of range");
    return data[index];
}

DSString DSString::operator+(const DSString &other) const {
    DSString result;
    result.len = len + other.len;
    result.data = new char[result.len + 1];
    cstr_copy(result.data, data, len);
    cstr_copy(result.data + len, other.data, other.len);
    return result;
}

DSString DSString::operator+(char c) const {
    DSString result;
    result.len = len + 1;
    result.data = new char[result.len + 1];
    cstr_copy(result.data, data, len);
    result.data[len] = c;
    result.data[len + 1] = '\0';
    return result;
}

bool DSString::operator==(const DSString &other) const {
    if (len != other.len) {
        return false;
    }
    for (size_t i = 0; i < len; ++i) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

bool DSString::operator!=(const DSString &other) const {
    if (len != other.len) {
        return true;
    }
    for (size_t i = 0; i < len; ++i) {
        if (data[i] != other.data[i]) {
            return true;
        }
    }
    return false;
}

bool DSString::operator<(const DSString &other) const {
    size_t minLength = len < other.len ? len : other.len;
    for (size_t i = 0; i < minLength; ++i) {
        if (data[i] < other.data[i]) {
            return true;
        } else if (data[i] > other.data[i]) {
            return false;
        }
    }
    return len < other.len;
}

DSString &DSString::operator+=(const char *s) {
    return this->append(s);
}

DSString &DSString::operator+=(const DSString &s) {
    return this->append(s);
}

DSString DSString::substring(size_t start, size_t numChars) const {
    DSString result;
    if (start < len) {
        result.len = (numChars < len - start) ? numChars : len - start;
        result.data = new char[result.len + 1];
        for (size_t i = 0; i < result.len; ++i) {
            result.data[i] = data[start + i];
        }
        result.data[result.len] = '\0';
    }
    return result;
}

DSString DSString::toLower() const {
    DSString result(*this);
    for (size_t i = 0; i < len; ++i) {
        result.data[i] = std::tolower(result.data[i]);
    }
    return result;
}

const char *DSString::c_str() const {
    return data;
}

std::ostream &operator<<(std::ostream &os, const DSString &str) {
    os << str.data;
    return os;
}
std::istream &operator>>(std::istream &is, DSString &str) {
    char buffer[1024];
    is >> buffer;
    str = DSString(buffer);
    return is;
}

std::istream &getline(std::istream &is, DSString &str, char delim) {
    std::string temp;
    std::getline(is, temp, delim);
    str = DSString(temp.c_str());
    return is;
}

std::istream &getline(std::istream &is, DSString &str) {
    std::string temp;
    std::getline(is, temp);
    str = DSString(temp.c_str());
    return is;
}