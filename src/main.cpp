#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "monitoring.hpp"

int main(int argc, char** argv){
    // std::cout << exec("wmic cpu get Loadpercentage");
    long long buffer = 0;
    // std::cout << get_const_str_length("TotalPhysicalMemory    ") << "\n";
    monitoring::System sys1 = monitoring::System();

    for(int i = 0; i < 5; i++){
        buffer = sys1.get_ram_load_percentage();

        // std::cout << buffer;
        printf("%lld\n", buffer);
    }

    return 0;
}