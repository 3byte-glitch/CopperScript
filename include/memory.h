#ifndef MEMORY_H
#define MEMORY_H
#include <map>
#include <iostream>
struct memory {
    std::map<std::string, std::string> strings;
    std::map<std::string, int> ints;
};
#endif