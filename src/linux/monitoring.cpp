#include <iostream>
#include <cmath>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"

namespace monitoring{
    void System::update_hostname(){
        converter::organized_data_array converted_result = converter::linux_hostname_converter(exec("cat /etc/hostname"));

        this->hostname = converter::get_value_from_key(converted_result.data[0], "hostname");

        free(converted_result.data);
    }

    void System::update_basic_cpu_load_percentage(){
        converter::organized_data_array converted_result = converter::linux_stat_converter(exec("cat /proc/stat"));
        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();
        // char cpu_data_types[7][30] = {"user", "nice", "system", "idle", "iowait", "irq", "softirq"};
        long long user = get_stoll(converter::get_value_from_key(converted_result.data[0], "user"));
        long long nice = get_stoll(converter::get_value_from_key(converted_result.data[0], "nice"));
        long long system = get_stoll(converter::get_value_from_key(converted_result.data[0], "system"));
        long long idle = get_stoll(converter::get_value_from_key(converted_result.data[0], "idle"));
        long long iowait = get_stoll(converter::get_value_from_key(converted_result.data[0], "iowait"));
        long long irq = get_stoll(converter::get_value_from_key(converted_result.data[0], "irq"));
        long long softirq = get_stoll(converter::get_value_from_key(converted_result.data[0], "softirq"));

        // this->cpu_load_percentage = 100 * (user + nice + system + iowait + irq + softirq) / (user + nice + system + idle + iowait + irq + softirq);
        this->cpu.time = req_time;
        this->cpu.last_time = this->cpu.time - std::chrono::seconds{1};
        this->cpu.cpu_activity = user + nice + system + iowait + irq + softirq;
        this->cpu.cpu_idle = idle;
        this->cpu.last_cpu_activity = this->cpu.cpu_activity - 1;
        this->cpu.last_cpu_idle = this->cpu.cpu_idle - 1;
        this->cpu.cpu_load_percentage = 0 * (this->cpu.cpu_activity - this->cpu.last_cpu_activity) / (this->cpu.cpu_idle - this->cpu.last_cpu_idle);
        // std::cout << converted_result.data[0].keys[6] << "\t" << converted_result.data[0].values[6] << std::endl;
        free(converted_result.data);
    }

    void System::update_cpu_load_percentage(){
        converter::organized_data_array converted_result = converter::linux_stat_converter(exec("cat /proc/stat"));
        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();
        // char cpu_data_types[7][30] = {"user", "nice", "system", "idle", "iowait", "irq", "softirq"};
        long long user = get_stoll(converter::get_value_from_key(converted_result.data[0], "user"));
        long long nice = get_stoll(converter::get_value_from_key(converted_result.data[0], "nice"));
        long long system = get_stoll(converter::get_value_from_key(converted_result.data[0], "system"));
        long long idle = get_stoll(converter::get_value_from_key(converted_result.data[0], "idle"));
        long long iowait = get_stoll(converter::get_value_from_key(converted_result.data[0], "iowait"));
        long long irq = get_stoll(converter::get_value_from_key(converted_result.data[0], "irq"));
        long long softirq = get_stoll(converter::get_value_from_key(converted_result.data[0], "softirq"));

        // this->cpu_load_percentage = 100 * (user + nice + system + iowait + irq + softirq) / (user + nice + system + idle + iowait + irq + softirq);
        this->cpu.last_time = this->cpu.time;
        this->cpu.time = req_time;
        this->cpu.last_cpu_activity = this->cpu.cpu_activity;
        this->cpu.last_cpu_idle = this->cpu.cpu_idle;
        this->cpu.cpu_activity = user + nice + system + iowait + irq + softirq;
        this->cpu.cpu_idle = idle;
        this->cpu.cpu_load_percentage = 100 * (this->cpu.cpu_activity - this->cpu.last_cpu_activity) / (this->cpu.cpu_idle - this->cpu.last_cpu_idle);
        // std::cout << converted_result.data[0].keys[6] << "\t" << converted_result.data[0].values[6] << std::endl;
        free(converted_result.data);
    }

    void System::update_total_ram(){
        converter::organized_data_array converted_result = converter::linux_meminfo_converter(exec("cat /proc/meminfo"));
        this->ram.total_ram = get_stoll(converter::get_value_from_key(converted_result.data[0], "MemTotal"));
        free(converted_result.data);
    }

    void System::update_free_ram(){
        converter::organized_data_array converted_result = converter::linux_meminfo_converter(exec("cat /proc/meminfo"));
        this->ram.free_ram = get_stoll(converter::get_value_from_key(converted_result.data[0], "MemFree"));
        free(converted_result.data);
    }

    void System::update_basic_logical_disk_info(){
        converter::organized_data_array converted_result = converter::linux_sys_block_converter();
        // time_t req_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();

        int out_size = 0;
        for(int i = 0; i < converted_result.size; i++){
            if(converter::get_value_from_key(converted_result.data[i], "type") == "logical"){
                out_size++;
                // std::cout << i;
            }
        }

        this->logical_disk = {.size = 0, .data = NULL};

        this->logical_disk.size = out_size;
        // result.data = (logical_disk_type*) malloc(result.size * sizeof(logical_disk_type));
        this->logical_disk.data = new logical_disk_type[this->logical_disk.size];
        int j = 0;
        for(int i = 0; i < converted_result.size; i++){
            if(converter::get_value_from_key(converted_result.data[i], "type") == "logical"){
                // std::cout << converter::get_value_from_key(converted_result.data[i], "type");
                this->logical_disk.data[j].time = req_time;
                this->logical_disk.data[j].last_time = this->logical_disk.data[j].time - std::chrono::seconds{1};
                this->logical_disk.data[j].id = converter::get_value_from_key(converted_result.data[i], "id");
                this->logical_disk.data[j].volume_name = converter::get_value_from_key(converted_result.data[i], "name");
                this->logical_disk.data[j].total_space = get_stoll(converter::get_value_from_key(converted_result.data[i], "total_space"))*512/1024;
                this->logical_disk.data[j].free_space = get_stoll(converter::get_value_from_key(converted_result.data[i], "free_space"))*512/1024;
                this->logical_disk.data[j].last_free_space = this->logical_disk.data[j].free_space;
                // printf("/%d", converter::get_value_from_key(converted_result.data[j], "free_space").c_str()[converter::get_value_from_key(converted_result.data[j], "free_space").length()-1]);
                j++;
            }
        }

        // std::cout << converted_result.data[3].values[1];

        free(converted_result.data);
    }

    void System::update_logical_disk_info(){
        converter::organized_data_array converted_result = converter::linux_sys_block_converter();
        // time_t req_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();

        for(int i = 0; i < converted_result.size; i++){
            if(converter::get_value_from_key(converted_result.data[i], "type") == "logical"){
                for(int j = 0; j < this->logical_disk.size; j++){
                    if(converter::get_value_from_key(converted_result.data[i], "id") == this->logical_disk.data[j].id){
                        this->logical_disk.data[j].last_time = this->logical_disk.data[j].time;
                        this->logical_disk.data[j].last_free_space = this->logical_disk.data[j].free_space;
                        this->logical_disk.data[j].time = req_time;
                        this->logical_disk.data[j].free_space = get_stoll(converter::get_value_from_key(converted_result.data[i], "free_space"))*512/1024;
                    }
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
    //         result.data[i].total_space = get_stoll(converter::get_value_from_key(converted_result.data[i], "Size"));
    //         result.data[i].free_space = get_stoll(converter::get_value_from_key(converted_result.data[i], "FreeSpace"));
    //     }

    //     free(converted_result.data);

    //     return result;
    // }

    void System::update_basic_physical_disk_info(){
        converter::organized_data_array converted_result = converter::linux_sys_block_converter();

        int out_size = 0;
        for(int i = 0; i < converted_result.size; i++){
            if(converter::get_value_from_key(converted_result.data[i], "type") == "physical"){
                out_size++;
                // std::cout << i;
            }
        }

        this->physical_disk = {.size = 0, .data = NULL};

        this->physical_disk.size = out_size;
        // result.data = (logical_disk_type*) malloc(result.size * sizeof(logical_disk_type));
        this->physical_disk.data = new physical_disk_type[this->physical_disk.size];

        int j = 0;
        for(int i = 0; i < converted_result.size; i++){
            if(converter::get_value_from_key(converted_result.data[i], "type") == "physical"){
                // std::cout << converter::get_value_from_key(converted_result.data[i], "type");
                this->physical_disk.data[j].caption = converter::get_value_from_key(converted_result.data[i], "name");
                this->physical_disk.data[j].total_space = get_stoll(converter::get_value_from_key(converted_result.data[i], "total_space"))*512/1024;
                // printf("/%d", converter::get_value_from_key(converted_result.data[j], "free_space").c_str()[converter::get_value_from_key(converted_result.data[j], "free_space").length()-1]);
                j++;
            }
        }

        free(converted_result.data);
    }
}