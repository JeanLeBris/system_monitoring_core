#include <iostream>
#include <cmath>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"

namespace monitoring{
    System::System(){
        this->update_hostname();

        this->update_basic_cpu_load_percentage();

        this->update_total_ram();
        this->update_free_ram();

        this->update_basic_logical_disk_info();

        this->update_basic_physical_disk_info();
    }

    System::~System(){
        delete[] this->logical_disk.data;
    }

    std::string System::get_hostname(){
        return this->hostname;
    }

    long long System::get_cpu_load_percentage(){
        return this->cpu.cpu_load_percentage;
    }

    long long System::get_ram_load_percentage(){
        long long used_ram = this->total_ram - this->free_ram;

        return 100 * used_ram / total_ram;
    }

    logical_disk_array System::get_logical_disk_info(){
        return this->logical_disk;
    }

    physical_disk_array System::get_physical_disk_info(){
        return this->physical_disk;
    }

    void System::update_info(){
        this->update_cpu_load_percentage();
        this->update_free_ram();
        this->update_logical_disk_info();
    }

    void System::display_system_info(){
        std::chrono::duration<double> duration;

        // std::cout << ;
        std::cout << "CPU info" << std::endl;
        std::cout << "\t" << this->get_cpu_load_percentage() << "%" << std::endl;
        std::cout << "Hostname" << std::endl;
        std::cout << "\t" << this->get_hostname() << std::endl;
        std::cout << "RAM info" << std::endl;
        std::cout << "\t" << this->get_ram_load_percentage() << "%" << std::endl;
        std::cout << "logical disk info" << std::endl;
        for(int i = 0; i < this->get_logical_disk_info().size; i++){
            duration = (this->get_logical_disk_info().data[i].time - this->get_logical_disk_info().data[i].last_time);
            std::cout << "\t" << this->get_logical_disk_info().data[i].id;
            std::cout << this->get_logical_disk_info().data[i].volume_name;
            std::cout << "\t" << this->get_logical_disk_info().data[i].total_space / std::pow(1024, 2) << "GB";
            std::cout << "\t" << this->get_logical_disk_info().data[i].free_space / std::pow(1024, 2) << "GB";
            std::cout << "\t" << (long long) (std::abs(this->get_logical_disk_info().data[i].free_space - this->get_logical_disk_info().data[i].last_free_space) / duration.count()) << "kB/s" << std::endl;
        }
        std::cout << "physical disk info" << std::endl;
        for(int i = 0; i < this->get_physical_disk_info().size; i++){
            std::cout << "\t" << this->get_physical_disk_info().data[i].caption;
            std::cout << "\t" << this->get_physical_disk_info().data[i].total_space / std::pow(1024, 2) << "GB" << std::endl;
        }
    }
}