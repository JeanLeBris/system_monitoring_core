#include <iostream>
// #include <date/date.h>
#include <cmath>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"

namespace monitoring{
    System::System(){
        this->accessed = false;
        this->logical_disk.size = 0;
        this->physical_disk.size = 0;
    }

    System::~System(){
        delete[] this->logical_disk.data;
        delete[] this->physical_disk.data;
    }

    bool System::get_accessed(){
        return this->accessed;
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

    std::chrono::system_clock::time_point System::get_initial_time(){
        return this->initial_time;
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

    void System::basic_update_info(){
        this->update_basic_time();

        this->update_hostname();

        this->update_basic_cpu_load_percentage();

        this->update_total_ram();
        this->update_free_ram();

        this->update_basic_logical_disk_info();

        this->update_basic_physical_disk_info();

        this->accessed = true;
    }

    void System::update_info(){
        this->update_time();
        this->update_cpu_load_percentage();
        this->update_free_ram();
        this->update_logical_disk_info();
    }

    void System::display_system_info(){
        std::chrono::duration<double> duration;

        if(this->accessed){
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
                // std::cout << "\t" << duration.count();
                std::cout << "\t" << (unsigned long long) (std::abs(this->get_logical_disk_info().data[i].free_space - this->get_logical_disk_info().data[i].last_free_space) / duration.count()) << "kB/s" << std::endl;
            }
            std::cout << "physical disk info" << std::endl;
            for(int i = 0; i < this->get_physical_disk_info().size; i++){
                std::cout << "\t" << this->get_physical_disk_info().data[i].caption;
                std::cout << "\t" << this->get_physical_disk_info().data[i].total_space / std::pow(1024, 2) << "GB" << std::endl;
            }
        }
        else{
            std::cout << "unaccessible" << std::endl;
        }
    }

    std::string System::to_json(){
        std::string output;
        // std::chrono::duration<double, std::chrono::milliseconds> duration;

        output.append("{");

        output.append("\"accessed\":");
        output.append(std::to_string(this->accessed));
        output.append("");
        output.append(",");

        output.append("\"last_time\":");
        output.append(std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(this->last_time - this->initial_time).count()));
        output.append("");
        output.append(",");

        output.append("\"time\":");
        output.append(std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(this->time - this->initial_time).count()));
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
            output.append(std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(this->logical_disk.data[i].last_time - this->initial_time).count()));
            output.append("");
            output.append(",");
            output.append("\"last_free_space\":");
            output.append(std::to_string(this->logical_disk.data[i].last_free_space));
            output.append("");
            output.append(",");
            output.append("\"time\":");
            output.append(std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(this->logical_disk.data[i].time - this->initial_time).count()));
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

    void System::from_json(std::string data){
        std::string buffer_string = data;
        int buffer_int = 0;
        int size_buffer = 0;
        // std::chrono::system_clock::time_point zero_point = std::chrono::system_clock::time_point(std::chrono::nanoseconds(0));

        // std::cout << std::to_string(this->logical_disk.size) << ":" << std::to_string(this->physical_disk.size) << std::endl;

        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(",");
        this->accessed = get_stoll(buffer_string.substr(0, buffer_int));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());

        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(",");
        this->last_time = std::chrono::system_clock::time_point(std::chrono::nanoseconds(get_stoll(buffer_string.substr(0, buffer_int))));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());

        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(",");
        this->time = std::chrono::system_clock::time_point(std::chrono::nanoseconds(get_stoll(buffer_string.substr(0, buffer_int))));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find("\"");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find("\"");
        this->hostname = buffer_string.substr(0, buffer_int);
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find("}");
        this->cpu.cpu_load_percentage = std::stoi(buffer_string.substr(0, buffer_int));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(",");
        this->ram.total_ram = get_stoll(buffer_string.substr(0, buffer_int));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find("}");
        this->ram.free_ram = get_stoll(buffer_string.substr(0, buffer_int));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        
        size_buffer = this->logical_disk.size;
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(",");
        this->logical_disk.size = std::stoi(buffer_string.substr(0, buffer_int));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());

        if(size_buffer == 0){
            this->logical_disk.data = new logical_disk_type[this->logical_disk.size];
        }
        else if(size_buffer != this->logical_disk.size){
            delete[] this->logical_disk.data;
            this->logical_disk.data = new logical_disk_type[this->logical_disk.size];
        }
        for(int i = 0; i < this->logical_disk.size; i++){
            buffer_int = buffer_string.find("{");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("\"");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("\"");
            this->logical_disk.data[i].id = buffer_string.substr(0, buffer_int);
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("\"");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("\"");
            this->logical_disk.data[i].volume_name = buffer_string.substr(0, buffer_int);
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find(",");
            this->logical_disk.data[i].total_space = get_stoll(buffer_string.substr(0, buffer_int));
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find(",");
            this->logical_disk.data[i].last_time = std::chrono::system_clock::time_point(std::chrono::nanoseconds(get_stoll(buffer_string.substr(0, buffer_int))));
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find(",");
            this->logical_disk.data[i].last_free_space = get_stoll(buffer_string.substr(0, buffer_int));
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find(",");
            this->logical_disk.data[i].time = std::chrono::system_clock::time_point(std::chrono::nanoseconds(get_stoll(buffer_string.substr(0, buffer_int))));
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("}");
            this->logical_disk.data[i].free_space = get_stoll(buffer_string.substr(0, buffer_int));
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        }
        
        size_buffer = this->physical_disk.size;
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(",");
        this->physical_disk.size = std::stoi(buffer_string.substr(0, buffer_int));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());

        if(size_buffer == 0){
            this->physical_disk.data = new physical_disk_type[this->physical_disk.size];
        }
        else if(size_buffer != this->physical_disk.size){
            delete[] this->physical_disk.data;
            this->physical_disk.data = new physical_disk_type[this->physical_disk.size];
        }
        // this->physical_disk.data = new physical_disk_type[this->physical_disk.size];
        for(int i = 0; i < this->physical_disk.size; i++){
            buffer_int = buffer_string.find("{");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("\"");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("\"");
            this->physical_disk.data[i].caption = buffer_string.substr(0, buffer_int);
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("}");
            this->physical_disk.data[i].total_space = get_stoll(buffer_string.substr(0, buffer_int));
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        }

        // std::cout << std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(this->last_time - zero_point).count()) << std::endl;
        // std::cout << this->hostname << std::endl;
        // std::cout << std::to_string(this->cpu.cpu_load_percentage) << std::endl;
    }

    std::vector<uint8_t> System::append_to_byte_array(std::vector<uint8_t> data, uint64_t size, uint8_t* byte_array){
        uint8_t *a_size = (uint8_t*) &size;
        data.insert(data.end(), a_size, a_size + 8);
        data.insert(data.end(), byte_array, byte_array + size);
        return data;
    }
    
    std::vector<uint8_t> System::to_bytes(){
        std::vector<uint8_t> output;
        uint64_t size = 0;
        // uint8_t *a_size = (uint8_t*) &size;
        // uint64_t size2 = 0;

        size = sizeof(*this);
        // size = 999999999;
        // output.insert(output.end(), a_size, a_size+8);
        // size2 = *(uint64_t*) a_size;
        // size2 = *(uint64_t*) output.cbegin().base();

        // std::cout << sizeof(*this) << std::endl;
        // std::cout << size << std::endl;
        // std::cout << size2 << std::endl;
        // std::cout << (int) a_size[8] << std::endl;
        // output.insert(output.end(), (uint8_t*) this, ((uint8_t*) this) + size);
        output = append_to_byte_array(output, size, (uint8_t*) this);

        size = this->hostname.size()+1;
        output = append_to_byte_array(output, size, (uint8_t*) this->hostname.c_str());

        // size = (uint64_t) this->logical_disk.size;
        // output.insert(output.end(), a_size, a_size + 8);
        
        for(int i = 0; i < this->logical_disk.size; i++){
            size = sizeof(this->logical_disk.data[i]);
            output = append_to_byte_array(output, size, (uint8_t*) &(this->logical_disk.data[i]));

            size = this->logical_disk.data[i].id.size()+1;
            output = append_to_byte_array(output, size, (uint8_t*) this->logical_disk.data[i].id.c_str());

            size = this->logical_disk.data[i].volume_name.size()+1;
            output = append_to_byte_array(output, size, (uint8_t*) this->logical_disk.data[i].volume_name.c_str());
        }

        for(int i = 0; i < this->physical_disk.size; i++){
            size = sizeof(this->physical_disk.data[i]);
            output = append_to_byte_array(output, size, (uint8_t*) &(this->physical_disk.data[i]));

            size = this->physical_disk.data[i].caption.size()+1;
            output = append_to_byte_array(output, size, (uint8_t*) this->physical_disk.data[i].caption.c_str());
        }
        // std::cout << sizeof(int) << std::endl;

        // monitoring::System sys3;
        // sys3 = *(monitoring::System*) (output.cbegin().base()+8);

        // std::cout << sys3.get_hostname() << std::endl;

        return output;
    }
    
    void System::from_bytes(std::vector<uint8_t> data){
        for(unsigned long long i = 0; i < data.size(); i++){
            std::cout << "/" << (int) data.at(i);
        }
        std::cout << std::endl;
    }

    Environment::Environment(){
        this->size = 0;
        this->push("local", new System());
        this->get_system_by_key("local")->basic_update_info();
    }

    Environment::Environment(std::string key){
        this->size = 0;
        this->push(key, new System());
        this->get_system_by_key(key)->basic_update_info();
    }

    Environment::~Environment(){
        for(int i = 0; i < this->size; i++){
            delete this->systems.at(i);
        }
    }

    void Environment::push(std::string key, System* system){
        this->size++;
        this->keys.push_back(key);
        this->systems.push_back(system);
    }

    std::vector<std::string> Environment::get_keys(){
        return this->keys;
    }

    System* Environment::get_system_by_key(std::string key){
        for(int i = 0; i < this->size; i++){
            if(key == this->keys.at(i)){
                return this->systems.at(i);
            }
        }
        this->push(key, new System());
        return this->get_system_by_key(key);
    }

    void Environment::display_environment_info(){
        std::cout << "======================" << std::endl;

        for(int i = 0; i < this->size; i++){
            std::cout << "ip" << std::endl;
            std::cout << "\t" << this->keys.at(i) << std::endl;
            this->systems.at(i)->display_system_info();
            std::cout << std::endl;
        }

        std::cout << "======================" << std::endl;
    }

    std::string Environment::to_json(){
        std::string output;
        std::string system_json;

        output.append("{");

        output.append("\"size\":");
        output.append(std::to_string(this->size));
        output.append("");
        output.append(",");

        output.append("\"data\":");

        output.append("[");

        for(int i = 0; i < this->size; i++){
            output.append("{");
            
            output.append("\"ip\":\"");
            output.append(this->keys.at(i));
            output.append("\"");
            output.append(",");
            
            system_json = this->systems.at(i)->to_json();
            output.append("\"size\":");
            output.append(std::to_string(system_json.length()));
            output.append("");
            output.append(",");
            
            output.append("\"system\":");
            output.append(system_json);
            output.append("");

            output.append("}");
            if(i < this->size - 1){
                output.append(",");
            }
        }

        output.append("]");
        output.append("}");

        return output;
    }

    void Environment::from_json(std::string data, char* source_ip){
        std::string buffer_string = data;
        int buffer_int = 0;
        // int size_buffer = 0;

        int amount_of_systems = 0;
        std::string buffer_ip;
        int system_size = 0;
        
        buffer_int = buffer_string.find(":");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        buffer_int = buffer_string.find(",");
        amount_of_systems = std::stoi(buffer_string.substr(0, buffer_int));
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());

        buffer_int = buffer_string.find("[");
        buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());

        for(int i = 0; i < amount_of_systems; i++){
            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("\"");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find("\"");
            buffer_ip = buffer_string.substr(0, buffer_int);
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());

            if(buffer_ip == "local"){
                buffer_ip.assign(source_ip);
            }

            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            buffer_int = buffer_string.find(",");
            system_size = std::stoi(buffer_string.substr(0, buffer_int));
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());

            buffer_int = buffer_string.find(":");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
            // buffer_int = buffer_string.find(",");
            this->get_system_by_key(buffer_ip)->from_json(buffer_string.substr(0, system_size));
            buffer_string = buffer_string.substr(system_size+1, buffer_string.length());
            
            buffer_int = buffer_string.find("}");
            buffer_string = buffer_string.substr(buffer_int+1, buffer_string.length());
        }
    }
}