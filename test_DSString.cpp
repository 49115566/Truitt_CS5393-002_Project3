#include <iostream>
#include <vector>
#include <algorithm>

#include "DSString.h"

/* TODO: This file is a start to test your DSString class. Add more tests and add code to check if the results are correct. */

int main()
{  
    DSString myString = "Hello, World!";
    std::cout << myString << "\n";

    // this uses the one argument constructor in DSString and then the inherited operator=
    myString = "Good bye!";
    std::cout << myString << "\n";

    std::cout << "substr: " << myString.substring(5, 3) << "\n";

    // test some operators (=, ==, [])
    DSString a = "test";
    DSString b;
    b = a;

    std::cout << std::boolalpha;
    std::cout << (a == b) << "\n";

    b[0] = 'T';
    std::cout << "a is now: " << a << "\n";
    std::cout << (a == b) << "\n";

    // use initialization list
    std::vector<DSString> strings = {
        DSString("bb"),
        DSString("aaa"),
        DSString("ddd"),
        DSString("ee"),
        DSString("ccc")};

    // find strings
    for (const auto &s : strings)
        std::cout
            << s << "\n";

    // find implements linear search
    std::cout << "found ddd: " << (std::find(strings.begin(), strings.end(), DSString("ddd")) != strings.end()) << "\n";
    std::cout << "found z: " << (std::find(strings.begin(), strings.end(), DSString("z")) != strings.end()) << "\n";

    // sorting using the STL (note thus needs operator= and operator<)
    std::sort(strings.begin(), strings.end());

    for (const auto &s : strings)
        std::cout
            << s << "\n";

    // the data structure is sorted. Now we can do more efficient search using STL binary search
    std::cout << "found ddd: " << binary_search(strings.begin(), strings.end(), DSString("ddd")) << "\n";
    std::cout << "found z: " << binary_search(strings.begin(), strings.end(), DSString("z")) << "\n";

    return 0;
}
