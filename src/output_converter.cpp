#include <iostream>
#include <cstring>

#include "output_converter.hpp"
#include "utils.hpp"

namespace converter{
    organized_data_array wmic_converter(std::string data){
        std::string::const_iterator index = data.cbegin();
        std::string::const_iterator end = data.cend();

        organized_data_array output = {.size = 0, .data = NULL};
        wmic_status status = new_array;

        while(index != end){
            if(*index.base() == 13 && *(index+1).base() == 10){
                index++;
                if(status == value){
                    status = new_data;
                }
                else if(status == new_data){
                    status = new_array;
                }
            }
            else if(*index.base() == '='){
                if(status == key){
                    status = value;
                }
            }
            else if(*index.base() == 0){

            }
            else{
                if(status == new_data){
                    output.data[output.size-1].size++;
                    // output.data[output.size-1].keys.push_back("");
                    // output.data[output.size-1].values.push_back("");
                    output.data[output.size-1].keys[output.data[output.size-1].size-1][0] = '\0';
                    output.data[output.size-1].values[output.data[output.size-1].size-1][0] = '\0';
                    status = key;
                }
                else if(status == new_array){
                    output.size++;
                    output.data = (organized_data*) realloc(output.data, output.size*sizeof(*output.data));
                    output.data[output.size-1].size = 1;
                    // output.data[output.size-1].keys.push_back("");
                    // output.data[output.size-1].values.push_back("");
                    output.data[output.size-1].keys[output.data[output.size-1].size-1][0] = '\0';
                    output.data[output.size-1].values[output.data[output.size-1].size-1][0] = '\0';
                    status = key;
                }
                
                // std::string test;
                // printf("%d:%d\n", output.size, output.data[output.size-1].size);
                if(status == key){
                    // output.data[output.size-1].keys[output.data[output.size-1].size-1].push_back(*index.base());

                    // std::cout << output.data[output.size-1].keys[output.data[output.size-1].size-1] << std::endl;
                    // output.data[output.size-1].keys[output.data[output.size-1].size-1] += *index.base();

                    output.data[output.size-1].keys[output.data[output.size-1].size-1][std::strlen(output.data[output.size-1].keys[output.data[output.size-1].size-1])+1] = '\0';
                    output.data[output.size-1].keys[output.data[output.size-1].size-1][std::strlen(output.data[output.size-1].keys[output.data[output.size-1].size-1])] = *index.base();
                }
                else if(status == value){
                    // output.data[output.size-1].values[output.data[output.size-1].size-1].push_back(*index.base());

                    // std::cout << output.data[output.size-1].values[output.data[output.size-1].size-1] << std::endl;
                    // output.data[output.size-1].values[output.data[output.size-1].size-1] += *index.base();
                    
                    output.data[output.size-1].values[output.data[output.size-1].size-1][std::strlen(output.data[output.size-1].values[output.data[output.size-1].size-1])+1] = '\0';
                    output.data[output.size-1].values[output.data[output.size-1].size-1][std::strlen(output.data[output.size-1].values[output.data[output.size-1].size-1])] = *index.base();
                }
            }

            index++;
        }

        return output;
    }

    organized_data_array linux_stat_converter(std::string data){
        // std::string::const_iterator index = data.cbegin();
        // std::string::const_iterator end = data.cend();

        organized_data_array output = {.size = 0, .data = NULL};

        std::string iteration_string = data;
        std::string string_element;
        bool keep_going = true;
        int space_pos = 0;
        int nl_pos = 0;
        int nearest_pos;

        char cpu_data_types[7][30] = {"user", "nice", "system", "idle", "iowait", "irq", "softirq"};

        while(keep_going){
            space_pos = iteration_string.find(' ');
            nl_pos = iteration_string.find("\n");
            nearest_pos = (space_pos < nl_pos) ? space_pos : nl_pos;
            string_element = iteration_string.substr(0, nearest_pos);
            iteration_string = iteration_string.substr(nearest_pos+1, iteration_string.length());
            if(string_element == "intr"){
                keep_going = false;
            }
            // std::cout << string_element << std::endl;
            if(string_element.find("cpu") != std::string::npos){
                output.size++;
                output.data = (organized_data*) realloc(output.data, output.size*sizeof(*output.data));
                output.data[output.size-1].size = 7;

                for(int i = 0; i < 7; i++){
                    space_pos = iteration_string.find(' ');
                    nl_pos = iteration_string.find("\n");
                    nearest_pos = (space_pos < nl_pos) ? space_pos : nl_pos;
                    string_element = iteration_string.substr(0, nearest_pos);
                    iteration_string = iteration_string.substr(nearest_pos+1, iteration_string.length());
                    while(nearest_pos == 0){
                        space_pos = iteration_string.find(' ');
                        nl_pos = iteration_string.find("\n");
                        nearest_pos = (space_pos < nl_pos) ? space_pos : nl_pos;
                        string_element = iteration_string.substr(0, nearest_pos);
                        iteration_string = iteration_string.substr(nearest_pos+1, iteration_string.length());
                    }

                    std::strcpy(output.data[output.size-1].keys[i], cpu_data_types[i]);
                    std::strcpy(output.data[output.size-1].values[i], string_element.c_str());
                }
            }
        }

        return output;
    }

    organized_data_array linux_meminfo_converter(std::string data){
        std::string::const_iterator index = data.cbegin();
        std::string::const_iterator end = data.cend();

        organized_data_array output = {.size = 0, .data = NULL};
        output.size++;
        output.data = (organized_data*) realloc(output.data, output.size*sizeof(*output.data));
        output.data[output.size-1].size = 0;
        wmic_status status = new_data;

        int prefic_suffix = -1;

        while(index != end){
            if(*index.base() == 10){
                if(status == value){
                    status = new_data;
                }
            }
            else if(*index.base() == ':'){
                if(status == key){
                    status = value;
                    prefic_suffix = -1;
                }
            }
            else if(*index.base() == 0){

            }
            else{
                if(status == new_data){
                    output.data[output.size-1].size++;
                    // output.data[output.size-1].keys.push_back("");
                    // output.data[output.size-1].values.push_back("");
                    output.data[output.size-1].keys[output.data[output.size-1].size-1][0] = '\0';
                    output.data[output.size-1].values[output.data[output.size-1].size-1][0] = '\0';
                    status = key;
                }
                
                // std::string test;
                // printf("%d:%d\n", output.size, output.data[output.size-1].size);
                if(status == key){
                    // output.data[output.size-1].keys[output.data[output.size-1].size-1].push_back(*index.base());

                    // std::cout << output.data[output.size-1].keys[output.data[output.size-1].size-1] << std::endl;
                    // output.data[output.size-1].keys[output.data[output.size-1].size-1] += *index.base();

                    output.data[output.size-1].keys[output.data[output.size-1].size-1][std::strlen(output.data[output.size-1].keys[output.data[output.size-1].size-1])+1] = '\0';
                    output.data[output.size-1].keys[output.data[output.size-1].size-1][std::strlen(output.data[output.size-1].keys[output.data[output.size-1].size-1])] = *index.base();
                }
                else if(status == value){
                    // output.data[output.size-1].values[output.data[output.size-1].size-1].push_back(*index.base());

                    // std::cout << output.data[output.size-1].values[output.data[output.size-1].size-1] << std::endl;
                    // output.data[output.size-1].values[output.data[output.size-1].size-1] += *index.base();
                    
                    if(prefic_suffix == -1){
                        if(*index.base() != ' '){
                            prefic_suffix = 0;
                        }
                    }
                    if(prefic_suffix == 0){
                        if(*index.base() != ' '){
                            output.data[output.size-1].values[output.data[output.size-1].size-1][std::strlen(output.data[output.size-1].values[output.data[output.size-1].size-1])+1] = '\0';
                            output.data[output.size-1].values[output.data[output.size-1].size-1][std::strlen(output.data[output.size-1].values[output.data[output.size-1].size-1])] = *index.base();
                        }
                        else{
                            prefic_suffix = 1;
                        }
                    }
                }
            }

            index++;
        }

        return output;
    }

    void display_data(organized_data_array data){
        for(int i = 0; i < data.size; i++){
            for(int j = 0; j < data.data[i].size; j++){
                std::cout << data.data[i].keys[j] << " : " << data.data[i].values[j] << std::endl;
            }
            std::cout << std::endl;
        }
    }

    std::string get_value_from_key(organized_data data, const char* key){
        std::string result;

        for(int i = 0; i < data.size; i++){
            if(std::strcmp(data.keys[i], key) == 0){
                result.assign(data.values[i]);
                return result;
            }
        }

        return result;
    }
}