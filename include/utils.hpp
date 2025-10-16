#ifndef SYSTEM_MONITORING_UTILS_HPP
#define SYSTEM_MONITORING_UTILS_HPP

#include <string>

std::string exec(const char* cmd);
std::string exec(std::string cmd);
int get_const_str_length(const char* input);
long long get_stoll(std::string value, long long replacement);

#endif