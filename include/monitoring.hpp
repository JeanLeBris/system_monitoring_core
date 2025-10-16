#ifndef SYSTEM_MONITORING_MONITORING_HPP
#define SYSTEM_MONITORING_MONITORING_HPP

#include <string>
#include <chrono>
#include <vector>

namespace monitoring{
    struct cpu_type{
        std::chrono::system_clock::time_point last_time;
        long long last_cpu_activity;
        long long last_cpu_idle;
        std::chrono::system_clock::time_point time;
        long long cpu_activity;
        long long cpu_idle;
        int cpu_load_percentage;
    };

    struct ram_type{
        long long total_ram;
        long long free_ram;
    };

    struct logical_disk_type{
        std::string id;
        std::string volume_name;
        long long total_space;
        std::chrono::system_clock::time_point last_time;
        long long last_free_space;
        std::chrono::system_clock::time_point time;
        long long free_space;
    };

    struct logical_disk_array{
        int size;
        logical_disk_type* data;
    };

    struct physical_disk_type{
        std::string caption;
        long long total_space;
    };

    struct physical_disk_array{
        int size;
        physical_disk_type* data;
    };

    class System{
        private:
        std::chrono::system_clock::time_point initial_time;
        std::chrono::system_clock::time_point last_time;
        std::chrono::system_clock::time_point time;

        std::string hostname;

        // long long cpu_load_percentage;
        cpu_type cpu;

        // long long total_ram;
        // long long free_ram;
        ram_type ram;

        logical_disk_array logical_disk;

        physical_disk_array physical_disk;


        public:
        System();
        ~System();

        /**
         * Update the system object time
         */
        void update_basic_time();

        /**
         * Update the system object time
         */
        void update_time();

        /**
         * Get the system object initial time
         */
        std::chrono::system_clock::time_point get_initial_time();

        /**
         * Get the system object last time
         */
        std::chrono::system_clock::time_point get_last_time();

        /**
         * Get the system object time
         */
        std::chrono::system_clock::time_point get_time();



        /**
         * Update the hostname
         */
        void update_hostname();

        /**
         * Get the hostname
         */
        std::string get_hostname();



        /**
         * Update the CPU load percentage in %
         */
        void update_basic_cpu_load_percentage();

        /**
         * Update the CPU load percentage in %
         */
        void update_cpu_load_percentage();

        /**
         * Returns the CPU load percentage in %
         */
        long long get_cpu_load_percentage();


        
        /**
         * Update the total RAM in KB
         */
        void update_total_ram();
        
        /**
         * Update the free RAM in KB
         */
        void update_free_ram();

        /**
         * Returns the RAM load percentage in %
         */
        long long get_ram_load_percentage();



        /**
         * Set the logical disks basic info
         */
        void update_basic_logical_disk_info();

        /**
         * Update the logical disks info
         */
        void update_logical_disk_info();

        /**
         * Returns the logical disks informations such as its ID, its volume name, its total space (in kB) as well as its free space (in kB) (change on windows)
         */
        logical_disk_array get_logical_disk_info();



        /**
         * Set the physical disks basic info
         */
        void update_basic_physical_disk_info();

        /**
         * Returns the physical disks informations such as its caption and its total space (in kB) (change on windows)
         */
        physical_disk_array get_physical_disk_info();



        /**
         * Update the system's info
         */
        void basic_update_info();

        /**
         * Update the system's info
         */
        void update_info();



        /**
         * Display the system's information basically
         */
        void display_system_info();



        /**
         * Get a json string from the system's data
         */
        std::string to_json();

        /**
         * Get the system's data from a json string
         */
        void from_json(std::string data);

        std::vector<uint8_t> append_to_byte_array(std::vector<uint8_t> data, uint64_t size, uint8_t* byte_array);

        /**
         * Get a byte array from the system's data
         */
        std::vector<uint8_t> to_bytes();

        /**
         * Get the system's data from a byte array
         */
        void from_bytes(std::vector<uint8_t> data);
    };

    class Environment{
        private:
        int size;
        std::vector<std::string> keys;
        std::vector<System*> systems;

        public:
        Environment();
        ~Environment();

        void push(std::string key, System* system);

        std::vector<std::string> get_keys();

        System* get_system_by_key(std::string key);



        /**
         * Display the system's information basically
         */
        void display_environment_info();



        /**
         * Get a json string from the system's data
         */
        std::string to_json();

        /**
         * Get the system's data from a json string
         */
        void from_json(std::string data, char* source_ip);
    };
}

#endif