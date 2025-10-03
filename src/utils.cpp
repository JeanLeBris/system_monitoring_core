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

int get_const_str_length(const char* input){
    int count = 0;

    for(int i = 0; input[i] != '\0'; i++){
        count++;
    }

    return count;
}
