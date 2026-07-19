#ifndef DEBUG_H
#define DEBUG_H
#include <vector>
#include <iostream>
#include "memory.h"
bool validate_syntax(std::vector<std::string> code);
void seprate_line_arguments(std::vector<std::string> code, memory& m);
int get_error();
int get_error_type();
#endif