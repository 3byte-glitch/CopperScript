#include <iostream>
#include <vector>
#include "include/debug.h"
#include "include/memory.h"

std::vector<std::string> code;
int main() {
    std::string line;
    int line_number = 1;
    while (true) {
        std::cout << line_number << ' ';
        getline(std::cin, line);
        if (line == "RUN") {
            break;
        } else {
            code.push_back(line);
        }
        line_number++;
    }
    if (validate_syntax(code)) {
        memory CopperMemory;
        seprate_line_arguments(code, CopperMemory);
    } else {
        std::cout << "Error at line " << get_error() << ", ";
        int error_type = get_error_type();
        if (error_type == 0) {
            std::cout << "function not recoginzed\n";
        } else if (error_type == 1) {
            std::cout << "function has incorrect number of arguments\n";
        }
    }
    return 0;
}