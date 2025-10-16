#include "utils.hpp"

#include <array>
#include <memory>
#include <stdexcept>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

std::string exec(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

int get_const_str_length(const char* input){
    int count = 0;

    for(int i = 0; input[i] != '\0'; i++){
        count++;
    }

    return count;
}

long long get_stoll(std::string value, long long replacement){
    if(value.length() > 0){
        return std::stoll(value);
    }
    else{
        return replacement;
        // printf("this crashed in cpu basic stoll\n");
        // exit(1);
    }
    // long long output = 0;
    // try{
    //     output = std::stoll(value);
    // }
    // catch(...){

    // }
    // return output;
}

long long get_stoll(std::string value){
    return get_stoll(value, 0);
}