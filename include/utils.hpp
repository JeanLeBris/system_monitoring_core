#ifndef SYSTEM_MONITORING_UTILS_HPP
#define SYSTEM_MONITORING_UTILS_HPP

#include <string>
#include <vector>

std::string exec(const char* cmd);
std::string exec(std::string cmd);
int get_const_str_length(const char* input);
long long get_stoll(std::string value, long long replacement);
long long get_stoll(std::string value);
std::vector<std::string> get_ip_addresses(std::string path);

#endif