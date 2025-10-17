#include "utils.hpp"

#include <array>
#include <memory>
#include <stdexcept>
#include <iostream>

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

std::vector<std::string> get_ip_addresses(std::string path){
    std::vector<std::string> output;

    if(path != ""){
        FILE *file = fopen(path.c_str(), "r");
        if (file == NULL) {
            printf("Cannot open file %s\n", path.c_str());
            exit(0);
        }
        
        char buffer_char_array[1000] = "\0";
        fread(buffer_char_array, sizeof(buffer_char_array[0]), 999, file);

        std::string buffer_string;
        buffer_string.assign(buffer_char_array);
        
        std::string::const_iterator index = buffer_string.cbegin();
        std::string::const_iterator end = buffer_string.cend();
        int buffer_int = 1;

        while(index != end){
            if(*index.base() == 13 || *index.base() == 10){
                buffer_int = 1;
            }
            else{
                if(buffer_int == 1){
                    output.push_back("");
                    buffer_int = 0;
                }
                output.at(output.size()-1).push_back(*index.base());
            }
            index++;
        }

        fclose(file);
    }

    return output;
}