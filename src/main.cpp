#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <thread>
#include <chrono>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"

int main(int argc, char** argv){
    // // std::cout << exec("wmic cpu get Loadpercentage");
    monitoring::logical_disk_array buffer1 = {.size = 0, .data = NULL};
    long long buffer2 = 0;
    // // std::cout << get_const_str_length("TotalPhysicalMemory    ") << "\n";
    monitoring::System sys1 = monitoring::System();
    // sys1.display_system_info();
    // converter::organized_data_array info = converter::linux_meminfo_converter(exec("cat /proc/meminfo"));
    // converter::display_data(info);
    // free(info.data);


    // sys1.update_cpu_load_percentage();

    
    // char bar[4000]{};

    // std::string result = exec("wmic logicaldisk get /VALUE");
    // // // result.copy(bar, 19, 23);
    // // result.copy(bar, 3999, 0);
    // // // std::cout << std::hex << bar;
    // // for(int i = 0; i < 4000; i++){
    // //     printf("/%d", bar[i]);
    // // }

    // converter::organized_data_array converted_result = converter::wmic_converter(result);
    // // std::cout << result;
    // converter::display_data(converted_result);
    // std::cout << converter::get_value_from_key(converted_result.data[0], "Access") << std::endl;
    // std::cout << converter::get_value_from_key(converted_result.data[0], "Accessl") << std::endl;
    // std::cout << converter::get_value_from_key(converted_result.data[0], "Availability") << std::endl;
    // std::cout << converter::get_value_from_key(converted_result.data[0], "Caption") << std::endl;
    // free(converted_result.data);

    for(int i = 0; i < 10000; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds{500});

        // std::cin.get();
        // // std::scanf("");
        // std::cout << exec("wmic logicaldisk get /VALUE") << std::endl;

        // // buffer1 = sys1.get_basic_logical_disk_info();
        // sys1.update_cpu_load_percentage();
        // buffer2 = sys1.get_cpu_load_percentage();

        // // std::cout << buffer;
        // // printf("%lld\n", buffer1.data[0].free_space);
        // printf("%lld\n", buffer2);
        // // delete[] buffer1.data;

        sys1.update_info();
        sys1.display_system_info();
    }
    // free(buffer.data);

    return 0;
}