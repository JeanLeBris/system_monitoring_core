#include <iostream>
#include <cmath>
#include <windows.h>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"

namespace monitoring{
    void System::update_hostname(){
        converter::organized_data_array converted_result = converter::windows_hostname_converter(exec("hostname"));

        this->hostname = converter::get_value_from_key(converted_result.data[0], "hostname");

        free(converted_result.data);
    }

    void System::update_basic_cpu_load_percentage(){
        FILETIME idleTime, kernelTime, userTime;
        GetSystemTimes(&idleTime, &kernelTime, &userTime);
        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();
        int64_t idle_tick, kernel_tick, user_tick, total_tick, activity_tick;
        idle_tick = (((unsigned long long)(idleTime.dwHighDateTime))<<32)|((unsigned long long)idleTime.dwLowDateTime);
        kernel_tick = (((unsigned long long)(kernelTime.dwHighDateTime))<<32)|((unsigned long long)kernelTime.dwLowDateTime);
        user_tick = (((unsigned long long)(userTime.dwHighDateTime))<<32)|((unsigned long long)userTime.dwLowDateTime);
        total_tick = kernel_tick + user_tick;
        activity_tick = total_tick - idle_tick;

        this->cpu.time = req_time;
        this->cpu.last_time = this->cpu.time - std::chrono::seconds{1};
        this->cpu.cpu_activity = activity_tick;
        this->cpu.cpu_idle = idle_tick;
        this->cpu.last_cpu_activity = this->cpu.cpu_activity - 1;
        this->cpu.last_cpu_idle = this->cpu.cpu_idle - 1;
        this->cpu.cpu_load_percentage = 0 * (this->cpu.cpu_activity - this->cpu.last_cpu_activity) / (this->cpu.cpu_idle - this->cpu.last_cpu_idle);
    }

    void System::update_cpu_load_percentage(){
        FILETIME idleTime, kernelTime, userTime;
        GetSystemTimes(&idleTime, &kernelTime, &userTime);
        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();
        int64_t idle_tick, kernel_tick, user_tick, total_tick, activity_tick;
        idle_tick = (((unsigned long long)(idleTime.dwHighDateTime))<<32)|((unsigned long long)idleTime.dwLowDateTime);
        kernel_tick = (((unsigned long long)(kernelTime.dwHighDateTime))<<32)|((unsigned long long)kernelTime.dwLowDateTime);
        user_tick = (((unsigned long long)(userTime.dwHighDateTime))<<32)|((unsigned long long)userTime.dwLowDateTime);
        total_tick = kernel_tick + user_tick;
        activity_tick = total_tick - idle_tick;

        this->cpu.last_time = this->cpu.time;
        this->cpu.time = req_time;
        this->cpu.last_cpu_activity = this->cpu.cpu_activity;
        this->cpu.last_cpu_idle = this->cpu.cpu_idle;
        this->cpu.cpu_activity = activity_tick;
        this->cpu.cpu_idle = idle_tick;
        this->cpu.cpu_load_percentage = 100 * (this->cpu.cpu_activity - this->cpu.last_cpu_activity) / (this->cpu.cpu_idle - this->cpu.last_cpu_idle);
    }

    void System::update_total_ram(){
        MEMORYSTATUS memory_state;
        GlobalMemoryStatus(&memory_state);
        this->ram.total_ram = memory_state.dwTotalPhys/1024;
    }

    void System::update_free_ram(){
        MEMORYSTATUS memory_state;
        GlobalMemoryStatus(&memory_state);
        this->ram.free_ram = memory_state.dwAvailPhys/1024;
    }

    void System::update_basic_logical_disk_info(){
        char test[80];
        int ret = GetLogicalDriveStringsA(79, (LPSTR) test);
        int counter = 0;
        char string_buffer[20] = "";
        for(int i = 0; i < ret; i++){
            if(test[i] == NULL){
                counter++;
            }
        }

        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();

        this->logical_disk = {.size = 0, .data = NULL};

        this->logical_disk.size = counter;
        this->logical_disk.data = new logical_disk_type[this->logical_disk.size];
        counter = 0;
        int internal_counter = 0;
        DWORD sectors_per_cluster, bytes_per_sector, number_of_free_clusters, total_number_of_clusters;
        char volume_name[100] = "";
        for(int i = 0; i < ret; i++){
            if(test[i] != NULL){
                string_buffer[counter] = test[i];
                counter++;
            }
            else{
                string_buffer[counter] = '\0';
                counter = 0;
                this->logical_disk.data[internal_counter].time = req_time;
                this->logical_disk.data[internal_counter].last_time = this->logical_disk.data[i].time - std::chrono::seconds{1};
                this->logical_disk.data[internal_counter].id.assign(string_buffer);
                GetVolumeInformationA(string_buffer, volume_name, 99, NULL, NULL, NULL, NULL, NULL);
                this->logical_disk.data[internal_counter].volume_name.assign(volume_name);
                GetDiskFreeSpaceA(string_buffer, &sectors_per_cluster, &bytes_per_sector, &number_of_free_clusters, &total_number_of_clusters);
                this->logical_disk.data[internal_counter].total_space = ((long long) total_number_of_clusters * (long long) sectors_per_cluster * (long long) bytes_per_sector) / 1024;
                this->logical_disk.data[internal_counter].free_space = ((long long) number_of_free_clusters * (long long) sectors_per_cluster * (long long) bytes_per_sector) / 1024;
                this->logical_disk.data[internal_counter].last_free_space = this->logical_disk.data[i].free_space;
                internal_counter++;
            }
        }
    }

    void System::update_logical_disk_info(){
        char test[80];
        int ret = GetLogicalDriveStringsA(79, (LPSTR) test);
        int counter = 0;
        char string_buffer[20] = "";
        for(int i = 0; i < ret; i++){
            if(test[i] == NULL){
                counter++;
            }
        }

        std::chrono::system_clock::time_point req_time = std::chrono::system_clock::now();

        counter = 0;
        DWORD sectors_per_cluster, bytes_per_sector, number_of_free_clusters, total_number_of_clusters;
        char volume_name[100] = "";
        for(int i = 0; i < ret; i++){
            if(test[i] != NULL){
                string_buffer[counter] = test[i];
                counter++;
            }
            else{
                string_buffer[counter] = '\0';
                counter = 0;
                for(int j = 0; j < this->logical_disk.size; j++){
                    if(string_buffer == this->logical_disk.data[j].id){
                        this->logical_disk.data[j].last_time = this->logical_disk.data[j].time;
                        this->logical_disk.data[j].last_free_space = this->logical_disk.data[j].free_space;
                        this->logical_disk.data[j].time = req_time;
                        GetDiskFreeSpaceA(string_buffer, &sectors_per_cluster, &bytes_per_sector, &number_of_free_clusters, &total_number_of_clusters);
                        this->logical_disk.data[j].free_space = ((long long) number_of_free_clusters * (long long) sectors_per_cluster * (long long) bytes_per_sector) / 1024;
                    }
                }
            }
        }
    }

    void System::update_basic_physical_disk_info(){
        converter::organized_data_array converted_result = converter::wmic_converter(exec("wmic diskdrive get /VALUE"));

        this->physical_disk = {.size = 0, .data = NULL};

        this->physical_disk.size = converted_result.size;
        // result.data = (logical_disk_type*) malloc(result.size * sizeof(logical_disk_type));
        this->physical_disk.data = new physical_disk_type[this->physical_disk.size];
        for(int i = 0; i < this->physical_disk.size; i++){
            this->physical_disk.data[i].caption = converter::get_value_from_key(converted_result.data[i], "Caption");
            this->physical_disk.data[i].total_space = get_stoll(converter::get_value_from_key(converted_result.data[i], "Size")) / 1024;
        }

        free(converted_result.data);
    }
}