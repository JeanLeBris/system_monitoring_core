#include <iostream>
// #include <date/date.h>
#include <cmath>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"

namespace monitoring{
    System::System(){
        this->update_basic_time();

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

    void System::update_basic_time(){
        this->time = std::chrono::system_clock::now();
        this->last_time = this->time - std::chrono::seconds{1};
        this->initial_time = this->last_time - std::chrono::seconds{1};
    }
    
    void System::update_time(){
        this->last_time = this->time;
        this->time = std::chrono::system_clock::now();
    }

    std::chrono::system_clock::time_point System::get_last_time(){
        return this->last_time;
    }

    std::chrono::system_clock::time_point System::get_time(){
        return this->time;
    }

    std::string System::get_hostname(){
        return this->hostname;
    }

    long long System::get_cpu_load_percentage(){
        return this->cpu.cpu_load_percentage;
    }

    long long System::get_ram_load_percentage(){
        long long used_ram = this->ram.total_ram - this->ram.free_ram;

        return 100 * used_ram / this->ram.total_ram;
    }

    logical_disk_array System::get_logical_disk_info(){
        return this->logical_disk;
    }

    physical_disk_array System::get_physical_disk_info(){
        return this->physical_disk;
    }

    void System::update_info(){
        this->update_time();
        this->update_cpu_load_percentage();
        this->update_free_ram();
        this->update_logical_disk_info();
    }

    void System::display_system_info(){
        std::chrono::duration<double> duration;

        // std::cout << ;
        std::cout << "Hostname" << std::endl;
        std::cout << "\t" << this->get_hostname() << std::endl;
        std::cout << "CPU info" << std::endl;
        std::cout << "\t" << this->get_cpu_load_percentage() << "%" << std::endl;
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

    std::string System::get_json_data(){
        std::string output;
        // std::chrono::duration<double, std::chrono::milliseconds> duration;

        output.append("{");

        output.append("\"last_time\":");
        output.append(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(this->last_time - this->initial_time).count()));
        output.append("");
        output.append(",");

        output.append("\"time\":");
        output.append(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(this->time - this->initial_time).count()));
        output.append("");
        output.append(",");

        output.append("\"hostname\":\"");
        output.append(this->hostname);
        output.append("\"");
        output.append(",");

        output.append("\"cpu\":");
        output.append("{");
        output.append("\"load_percentage\":");
        output.append(std::to_string(this->cpu.cpu_load_percentage));
        output.append("");
        output.append("}");
        output.append("");
        output.append(",");

        output.append("\"ram\":");
        output.append("{");
        output.append("\"total_ram\":");
        output.append(std::to_string(this->ram.total_ram));
        output.append("");
        output.append(",");
        output.append("\"free_ram\":");
        output.append(std::to_string(this->ram.free_ram));
        output.append("");
        output.append("}");
        output.append("");
        output.append(",");

        output.append("\"logical_disk\":");
        output.append("{");
        output.append("\"size\":");
        output.append(std::to_string(this->logical_disk.size));
        output.append("");
        output.append(",");
        output.append("\"data\":");
        output.append("[");
        for(int i = 0; i < this->logical_disk.size; i++){
            output.append("{");
            output.append("\"id\":\"");
            output.append(this->logical_disk.data[i].id);
            output.append("\"");
            output.append(",");
            output.append("\"volume_name\":\"");
            output.append(this->logical_disk.data[i].volume_name);
            output.append("\"");
            output.append(",");
            output.append("\"total_space\":");
            output.append(std::to_string(this->logical_disk.data[i].total_space));
            output.append("");
            output.append(",");
            output.append("\"last_time\":");
            output.append(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(this->logical_disk.data[i].last_time - this->initial_time).count()));
            output.append("");
            output.append(",");
            output.append("\"last_free_space\":");
            output.append(std::to_string(this->logical_disk.data[i].last_free_space));
            output.append("");
            output.append(",");
            output.append("\"time\":");
            output.append(std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(this->logical_disk.data[i].time - this->initial_time).count()));
            output.append("");
            output.append(",");
            output.append("\"free_space\":");
            output.append(std::to_string(this->logical_disk.data[i].free_space));
            output.append("");
            output.append("}");
            if(i < this->logical_disk.size - 1){
                output.append(",");
            }
        }
        output.append("]");
        output.append("");
        output.append("}");
        output.append("");
        output.append(",");

        output.append("\"physical_disk\":");
        output.append("{");
        output.append("\"size\":");
        output.append(std::to_string(this->physical_disk.size));
        output.append("");
        output.append(",");
        output.append("\"data\":");
        output.append("[");
        for(int i = 0; i < this->physical_disk.size; i++){
            output.append("{");
            output.append("\"caption\":\"");
            output.append(this->physical_disk.data[i].caption);
            output.append("\"");
            output.append(",");
            output.append("\"total_space\":");
            output.append(std::to_string(this->physical_disk.data[i].total_space));
            output.append("");
            output.append("}");
            if(i < this->physical_disk.size - 1){
                output.append(",");
            }
        }
        output.append("]");
        output.append("");
        output.append("}");
        output.append("");
        // output.append(",");

        output.append("}");

        // std::cout << output << std::endl;
        // duration = this->time - this->initial_time;
        // std::cout << std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(this->time - this->initial_time).count()) << std::endl;
        // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(this->last_time - this->initial_time).count() << std::endl;

        return output;
    }
}