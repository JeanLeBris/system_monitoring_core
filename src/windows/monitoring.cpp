#include "monitoring.hpp"
#include "utils.hpp"

namespace monitoring{
    monitoring::System::System(){

    }

    long long monitoring::System::get_cpu_load_percentage(){
        std::string result;
        char bar[4]{};

        result = exec("wmic cpu get Loadpercentage");
        // result.copy(bar, 3, 18);
        result.copy(bar, 3, get_const_str_length("Loadpercentage    "));
        if(bar[2] == ' ' || bar[2] == '\n'){
            bar[2] = '\0';
            if(bar[1] == ' ' || bar[1] == '\n'){
                bar[1] = '\0';
            }
        }

        return atoll(bar);
    }

    long long monitoring::System::get_total_ram(){
        std::string result;
        char bar[20]{};

        result = exec("wmic computersystem get TotalPhysicalMemory");
        // result.copy(bar, 19, 23);
        result.copy(bar, 19, get_const_str_length("TotalPhysicalMemory    "));
        if(bar[2] == ' ' || bar[2] == '\n'){
            bar[2] = '\0';
            if(bar[1] == ' ' || bar[1] == '\n'){
                bar[1] = '\0';
            }
        }

        return atoll(bar) / 1000;
        // return atoll(bar);
    }

    long long monitoring::System::get_free_ram(){
        std::string result;
        char bar[20]{};

        result = exec("wmic OS get FreePhysicalMemory");
        // result.copy(bar, 19, 22);
        result.copy(bar, 19, get_const_str_length("FreePhysicalMemory    "));
        if(bar[2] == ' ' || bar[2] == '\n'){
            bar[2] = '\0';
            if(bar[1] == ' ' || bar[1] == '\n'){
                bar[1] = '\0';
            }
        }

        return atoll(bar);
    }

    long long monitoring::System::get_ram_load_percentage(){
        long long total_ram = get_total_ram();
        long long free_ram = get_free_ram();

        long long used_ram = total_ram - free_ram;

        return 100 * used_ram / total_ram;
    }
}