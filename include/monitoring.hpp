#ifndef SYSTEM_MONITORING_MONITORING_HPP
#define SYSTEM_MONITORING_MONITORING_HPP

namespace monitoring{
    class System{
        private:

        public:
        System();

        long long get_cpu_load_percentage();

        long long get_total_ram();
        long long get_free_ram();
        long long get_ram_load_percentage();

        
    };
}

#endif