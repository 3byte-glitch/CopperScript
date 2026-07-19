#include <iostream>
#include <vector> 
#include <algorithm>
#include <sstream>
#include "include/debug.h"
#include <map>
#include <string>
std::map<std::string, int> functions = {
    {"#", 1},
    {"newline", 1},
    {"printvar", 2},
    {"print", 1},
    {"println", 1},
    {"int", 2},
    {"str", 2},
    {"add", 2},
    {"sub", 2},
    {"multiply", 2},
    {"div", 2},
    {"repeat", 1},
    {"endloop", 1}
};
std::vector<std::string> noargumentfunctions = {"newline", "endloop"};
int error_place;
int error_type;
/* Error type values: 
0 = unvalid function
1 = too much or too few arguments
*/
void run_command(std::vector<std::string> keywords, memory& m);
int string_to_int(std::string str, memory& m);
bool validate_syntax(std::vector<std::string> code) {
    int line_number = 1;
    std::vector<std::string> keywords;
    for (std::string line : code) {
        std::stringstream ss(line);
        std::string function;
        std::string keyword;
        while (ss >> keyword) {
            keywords.push_back(keyword);
        }
        function = keywords[0];
        if (functions.find(function) == functions.end() && !line.empty()) {
            error_place = line_number;
            error_type = 0;
            return false;
        }
        bool non_argument_accurate = std::find(noargumentfunctions.begin(), noargumentfunctions.end(), function) != noargumentfunctions.end() && keywords.size() == 1;
        bool with_argument_accurate = functions[function] == keywords.size()+1;
        if (non_argument_accurate || with_argument_accurate) {
            error_place = line_number;
            error_type = 1;
            return false;
        }
        line_number++;
    }
    return true;
}
int get_error() {
    return error_place;
}
int get_error_type() {
    return error_type;
}
void seprate_line_arguments(std::vector<std::string> code, memory& m) {
    for (size_t i = 0; i < code.size(); ++i) {
        std::stringstream ss(code[i]);
        std::string command;
        std::vector<std::string> keywords;
        
        if (!(ss >> command)) {
            continue;
        }
        if (command == "repeat") {
            ss >> command; 
            int repeat_times = string_to_int(command, m);
            std::vector<std::string> loop_body;
            size_t j = i + 1; 
            while (j < code.size()) {
                std::stringstream body_ss(code[j]);
                std::string check_cmd;
                body_ss >> check_cmd;
                if (check_cmd == "endloop") {
                    break;
                }
                loop_body.push_back(code[j]);
                j++;
            }
            for (int r = 0; r < repeat_times; r++) {
                seprate_line_arguments(loop_body, m);
            }
            i = j; 
            continue;
        }
        
        if (command == "endloop") {
            continue; 
        }
        keywords.push_back(command);
        bool is_non_argument = std::find(noargumentfunctions.begin(), noargumentfunctions.end(), command) != noargumentfunctions.end();
        
        if (functions[command] > 1 && !is_non_argument) {
            std::string arg;
            while (ss >> arg) {
                keywords.push_back(arg);
            }
            size_t split_index = functions[command]; 
            if (keywords.size() > split_index) {
                std::string merged = "";
                for (size_t i = split_index; i < keywords.size(); ++i) {
                    if (i > split_index) merged += " ";
                    merged += keywords[i];
                }
                keywords.erase(keywords.begin() + split_index, keywords.end());
                keywords.push_back(merged);
            }
        } else if (functions[command] == 1 && !is_non_argument) {
            std::string rest_of_line;
            std::getline(ss, rest_of_line); 
            if (!rest_of_line.empty()) {
                rest_of_line.erase(0, 1);
            }
            keywords.push_back(rest_of_line);
        }
        
        run_command(keywords, m);
    }
}
void run_command(std::vector<std::string> keywords, memory& m) {
    std::string command = keywords[0];
    if (command == "print") {
        std::string str = keywords[1];
        std::cout << str;
    } else if (command == "println") {
        std::string str = keywords[1];
        std::cout << str << '\n';
    } else if (command == "printvar") {
        std::string target = keywords[1];
        if (m.ints.find(target) != m.ints.end()) {
            std::cout << m.ints[target];
        } else if (m.strings.find(target) != m.strings.end()) {
            std::cout << m.strings[target];
        }
    } else if (command == "str") {
        std::string name = keywords[1];
        std::string value = keywords[2];
        if (value == "INPUT") {
            getline(std::cin, value);
        }
        m.strings[name] = value;
    } else if (command == "int") {
        if (keywords.size() < 3) return;
        
        std::string name = keywords[1];
        std::string value = keywords[2];
        
        if (value == "INPUT") {
            std::getline(std::cin, value);
        }
        try {
            m.ints[name] = string_to_int(value, m);
        } catch (...) {
            m.ints[name] = 0; 
        }
    } else if (command == "add") {
        int number;
        try {
            number = string_to_int(keywords[2], m);
        } catch (...) {
            number = 0; 
        }
        m.ints[keywords[1]] += number;
    } else if (command == "sub") {
        int number;
        try {
            number = string_to_int(keywords[2], m);
        } catch (...) {
            number = 0; 
        }
        m.ints[keywords[1]] -= number;
    } else if (command == "multiply") {
        int number;
        try {
            number = string_to_int(keywords[2], m);
        } catch (...) {
            number = 0; 
        }
        m.ints[keywords[1]] *= number;
    } else if (command == "div") {
        int number;
        try {
            number = string_to_int(keywords[2], m);
        } catch (...) {
            number = 0; 
        }
        m.ints[keywords[1]] /= number;
    } else if (command == "newline") {
        std::cout << '\n';
    }
}
int string_to_int(std::string str, memory& m) {
    if (m.ints.find(str) != m.ints.end()) {
        return m.ints[str];
    }
    try {
        return std::stoi(str);
    } catch (...) {
        return 0;
    }
}