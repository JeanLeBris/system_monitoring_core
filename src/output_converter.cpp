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