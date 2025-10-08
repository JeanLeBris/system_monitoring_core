#include <iostream>
#include <cmath>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"

namespace monitoring{
    monitoring::System::System(){
        this->update_cpu_load_percentage();

        this->update_total_ram();
        this->update_free_ram();

        this->update_basic_logical_disk_info();

        this->update_basic_physical_disk_info();
    }

    monitoring::System::~System(){
        delete[] this->logical_disk.data;
    }

    // long long monitoring::System::get_cpu_load_percentage(){
    //     std::string result;
    //     char bar[4]{};

    //     result = exec("wmic cpu get Loadpercentage");
    //     // result.copy(bar, 3, 18);
    //     result.copy(bar, 3, get_const_str_length("Loadpercentage    "));
    //     if(bar[2] == ' ' || bar[2] == '\n'){
    //         bar[2] = '\0';
    //         if(bar[1] == ' ' || bar[1] == '\n'){
    //             bar[1] = '\0';
    //         }
    //     }

    //     return atoll(bar);
    // }

    // long long monitoring::System::get_total_ram(){
    //     std::string result;
    //     char bar[20]{};

    //     result = exec("wmic computersystem get TotalPhysicalMemory");
    //     // result.copy(bar, 19, 23);
    //     result.copy(bar, 19, get_const_str_length("TotalPhysicalMemory    "));
    //     if(bar[2] == ' ' || bar[2] == '\n'){
    //         bar[2] = '\0';
    //         if(bar[1] == ' ' || bar[1] == '\n'){
    //             bar[1] = '\0';
    //         }
    //     }

    //     return atoll(bar) / 1000;
    //     // return atoll(bar);
    // }

    // long long monitoring::System::get_free_ram(){
    //     std::string result;
    //     char bar[20]{};

    //     result = exec("wmic OS get FreePhysicalMemory");
    //     // result.copy(bar, 19, 22);
    //     result.copy(bar, 19, get_const_str_length("FreePhysicalMemory    "));
    //     if(bar[2] == ' ' || bar[2] == '\n'){
    //         bar[2] = '\0';
    //         if(bar[1] == ' ' || bar[1] == '\n'){
    //             bar[1] = '\0';
    //         }
    //     }

    //     return atoll(bar);
    // }

    // long long monitoring::System::get_ram_load_percentage(){
    //     long long total_ram = get_total_ram();
    //     long long free_ram = get_free_ram();

    //     long long used_ram = total_ram - free_ram;

    //     return 100 * used_ram / total_ram;
    // }

    void monitoring::System::update_cpu_load_percentage(){
        converter::organized_data_array converted_result = converter::wmic_converter(exec("wmic cpu get LoadPercentage /VALUE"));
        this->cpu.cpu_load_percentage = stoll(converter::get_value_from_key(converted_result.data[0], "LoadPercentage"));
        free(converted_result.data);
    }

    long long monitoring::System::get_cpu_load_percentage(){
        return this->cpu.cpu_load_percentage;
    }

    void monitoring::System::update_total_ram(){
        converter::organized_data_array converted_result = converter::wmic_converter(exec("wmic computersystem get TotalPhysicalMemory /VALUE"));
        this->total_ram = stoll(converter::get_value_from_key(converted_result.data[0], "TotalPhysicalMemory")) / 1024;
        free(converted_result.data);
    }

    void monitoring::System::update_free_ram(){
        converter::organized_data_array converted_result = converter::wmic_converter(exec("wmic OS get FreePhysicalMemory /VALUE"));
        this->free_ram = stoll(converter::get_value_from_key(converted_result.data[0], "FreePhysicalMemory"));
        free(converted_result.data);
    }

    long long monitoring::System::get_ram_load_percentage(){
        long long used_ram = this->total_ram - this->free_ram;

        return 100 * used_ram / total_ram;
    }

    void monitoring::System::update_basic_logical_disk_info(){
        converter::organized_data_array converted_result = converter::wmic_converter(exec("wmic logicaldisk get /VALUE"));
        // time_t req_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();

        this->logical_disk = {.size = 0, .data = NULL};

        this->logical_disk.size = converted_result.size;
        // result.data = (logical_disk_type*) malloc(result.size * sizeof(logical_disk_type));
        this->logical_disk.data = new logical_disk_type[this->logical_disk.size];
        for(int i = 0; i < this->logical_disk.size; i++){
            this->logical_disk.data[i].time = req_time;
            this->logical_disk.data[i].last_time = this->logical_disk.data[i].time - std::chrono::seconds{1};
            this->logical_disk.data[i].id = converter::get_value_from_key(converted_result.data[i], "DeviceID");
            this->logical_disk.data[i].volume_name = converter::get_value_from_key(converted_result.data[i], "VolumeName");
            this->logical_disk.data[i].total_space = std::stoll(converter::get_value_from_key(converted_result.data[i], "Size"));
            this->logical_disk.data[i].free_space = std::stoll(converter::get_value_from_key(converted_result.data[i], "FreeSpace"));
            this->logical_disk.data[i].last_free_space = this->logical_disk.data[i].free_space;
        }

        free(converted_result.data);
    }

    void monitoring::System::update_logical_disk_info(){
        converter::organized_data_array converted_result = converter::wmic_converter(exec("wmic logicaldisk get /VALUE"));
        // time_t req_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();

        for(int i = 0; i < converted_result.size; i++){
            for(int j = 0; j < this->logical_disk.size; j++){
                if(converter::get_value_from_key(converted_result.data[i], "DeviceID") == this->logical_disk.data[j].id){
                    this->logical_disk.data[j].last_time = this->logical_disk.data[j].time;
                    this->logical_disk.data[j].last_free_space = this->logical_disk.data[j].free_space;
                    this->logical_disk.data[j].time = req_time;
                    this->logical_disk.data[j].free_space = std::stoll(converter::get_value_from_key(converted_result.data[i], "FreeSpace"));
                }
            }
        }

        free(converted_result.data);
    }

    // logical_disk_array monitoring::System::get_basic_logical_disk_info(){
    //     converter::organized_data_array converted_result = converter::wmic_converter(exec("wmic logicaldisk get /VALUE"));

    //     logical_disk_array result = {.size = 0, .data= NULL};

    //     result.size = converted_result.size;
    //     // result.data = (logical_disk_type*) malloc(result.size * sizeof(logical_disk_type));
    //     result.data = new logical_disk_type[result.size];
    //     for(int i = 0; i < result.size; i++){
    //         result.data[i].id = converter::get_value_from_key(converted_result.data[i], "DeviceID");
    //         result.data[i].volume_name = converter::get_value_from_key(converted_result.data[i], "VolumeName");
    //         result.data[i].total_space = std::stoll(converter::get_value_from_key(converted_result.data[i], "Size"));
    //         result.data[i].free_space = std::stoll(converter::get_value_from_key(converted_result.data[i], "FreeSpace"));
    //     }

    //     free(converted_result.data);

    //     return result;
    // }

    logical_disk_array monitoring::System::get_logical_disk_info(){
        return this->logical_disk;
    }

    void monitoring::System::update_basic_physical_disk_info(){
        converter::organized_data_array converted_result = converter::wmic_converter(exec("wmic diskdrive get /VALUE"));

        this->physical_disk = {.size = 0, .data = NULL};

        this->physical_disk.size = converted_result.size;
        // result.data = (logical_disk_type*) malloc(result.size * sizeof(logical_disk_type));
        this->physical_disk.data = new physical_disk_type[this->physical_disk.size];
        for(int i = 0; i < this->physical_disk.size; i++){
            this->physical_disk.data[i].caption = converter::get_value_from_key(converted_result.data[i], "Caption");
            this->physical_disk.data[i].total_space = std::stoll(converter::get_value_from_key(converted_result.data[i], "Size"));
        }

        free(converted_result.data);
    }

    physical_disk_array monitoring::System::get_physical_disk_info(){
        return this->physical_disk;
    }

    void monitoring::System::update_info(){
        this->update_cpu_load_percentage();
        this->update_free_ram();
        this->update_logical_disk_info();
    }

    void monitoring::System::display_system_info(){
        std::chrono::duration<double> duration;

        // std::cout << ;
        std::cout << "CPU info" << std::endl;
        std::cout << "\t" << this->get_cpu_load_percentage() << "%" << std::endl;
        std::cout << "RAM info" << std::endl;
        std::cout << "\t" << this->get_ram_load_percentage() << "%" << std::endl;
        std::cout << "logical disk info" << std::endl;
        for(int i = 0; i < this->get_logical_disk_info().size; i++){
            duration = (this->get_logical_disk_info().data[i].time - this->get_logical_disk_info().data[i].last_time);
            std::cout << "\t" << this->get_logical_disk_info().data[i].id;
            std::cout << this->get_logical_disk_info().data[i].volume_name;
            std::cout << "\t" << this->get_logical_disk_info().data[i].total_space / std::pow(1024, 3) << "GB";
            std::cout << "\t" << (long long) this->get_logical_disk_info().data[i].free_space / std::pow(1024, 3) << "GB";
            std::cout << "\t" << std::abs(this->get_logical_disk_info().data[i].free_space - this->get_logical_disk_info().data[i].last_free_space) / duration.count() << "B/s" << std::endl;
        }
        std::cout << "physical disk info" << std::endl;
        for(int i = 0; i < this->get_physical_disk_info().size; i++){
            std::cout << "\t" << this->get_physical_disk_info().data[i].caption << "\t" << this->get_physical_disk_info().data[i].total_space / std::pow(1024, 3) << "GB" << std::endl;
        }
    }
}