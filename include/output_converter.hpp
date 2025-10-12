#ifndef SYSTEM_MONITORING_OUTPUT_CONVERTER_HPP
#define SYSTEM_MONITORING_OUTPUT_CONVERTER_HPP

#include <string>
#include <vector>

namespace converter{
    struct organized_data{
        int size;
        // std::string keys[100];
        // std::string values[100];
        // std::vector<std::string> keys;
        // std::vector<std::string> values;
        char keys[100][100];
        char values[100][100];
    };

    struct organized_data_array{
        int size;
        organized_data* data;
    };

    enum wmic_status{
        key,
        value,
        new_data,
        new_array
    };

    organized_data_array wmic_converter(std::string data);

    organized_data_array windows_hostname_converter(std::string data);

    organized_data_array linux_hostname_converter(std::string data);

    organized_data_array linux_stat_converter(std::string data);

    organized_data_array linux_meminfo_converter(std::string data);

    organized_data_array linux_diskstats_converter(std::string data);

    organized_data_array linux_sys_block_converter();

    void display_data(organized_data_array data);

    std::string get_value_from_key(organized_data data, const char* key);
}

#endif