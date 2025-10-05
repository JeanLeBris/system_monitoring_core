#ifndef SYSTEM_MONITORING_MONITORING_HPP
#define SYSTEM_MONITORING_MONITORING_HPP

#include <string>

namespace monitoring{
    struct logical_disk_type{
        std::string id;
        std::string volume_name;
        long long total_space;
        long long free_space;
    };

    struct logical_disk_array{
        int size;
        logical_disk_type* data;
    };

    class System{
        private:
        long long cpu_load_percentage;

        long long total_ram;
        long long free_ram;

        logical_disk_array logical_disk;


        public:
        System();
        ~System();

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
         * Returns the logical disks informations such as its ID, its volume name, its total space (in B) as well as its free space (in B)
         */
        logical_disk_array get_basic_logical_disk_info();
    };
}

#endif