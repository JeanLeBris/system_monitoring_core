#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <thread>
#include <chrono>
#include <filesystem>
#include <cstring>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"

int test1(int argc, char** argv){
    monitoring::System sys1 = monitoring::System();

    for(int i = 0; i < 10000; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds{500});

        sys1.update_info();
        sys1.display_system_info();
    }

    return 0;
}

// struct thread_args{
//     monitoring::System *sys;
// };

void *sys_updater(void *_args){
    // thread_args *args = (thread_args*) _args;
    // monitoring::System *sys = args->sys;
    monitoring::System *sys = (monitoring::System*) _args;
    std::chrono::duration<double, std::milli> duration;
    while(1){
        // std::this_thread::sleep_for(std::chrono::milliseconds{500});
        duration = std::chrono::system_clock::now() - sys->get_time();
        while(duration.count() < 500){
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
            duration = std::chrono::system_clock::now() - sys->get_time();
        }
        // printf("ping\n");
        
        sys->update_info();
        // sys->display_system_info();
        // printf("test\n");
        // printf("%lld%\n", sys->get_cpu_load_percentage());
        // printf("%s%\n", sys->get_hostname().c_str());
        // std::cout << sys->get_hostname();
        // printf("test\n");
    }
    // free(args);
    pthread_exit(NULL);
}

int test2(int argc, char** argv){
    if(argc == 2){
        if(strcmp(argv[1], "slave") == 0){
            monitoring::System sys =  monitoring::System();
            sys.basic_update_info();
            // monitoring::System *sys = new monitoring::System;
            // sys.update_info();
            // sys.display_system_info();

            // thread_args *args = (thread_args*) malloc(sizeof(thread_args));
            pthread_t sniffer_thread;
            if(pthread_create(&sniffer_thread, NULL, sys_updater, (void*) &sys) < 0){
                perror("could not create thread");
                return 1;
            }
            // pthread_join(sniffer_thread, NULL);
            std::chrono::duration<double, std::milli> duration;
            std::chrono::system_clock::time_point time;
            while(1){
                // std::this_thread::sleep_for(std::chrono::milliseconds{1000});
                time = std::chrono::system_clock::now();
                duration = std::chrono::system_clock::now() - time;
                while(duration.count() < 1000){
                    std::this_thread::sleep_for(std::chrono::milliseconds{100});
                    duration = std::chrono::system_clock::now() - time;
                }
                sys.display_system_info();
                // sys->display_system_info();
            }
            // delete sys;
        }
        else if(strcmp(argv[1], "master") == 0){
            monitoring::System sys = monitoring::System();
            sys.basic_update_info();
            // monitoring::System *sys2 = new monitoring::System;
            monitoring::System sys2 = monitoring::System();
            // sys2->basic_update_info();

            pthread_t sniffer_thread;
            if(pthread_create(&sniffer_thread, NULL, sys_updater, (void*) &sys) < 0){
                perror("could not create thread");
                return 1;
            }

            std::chrono::duration<double, std::milli> duration;
            std::chrono::system_clock::time_point time;
            while(1){
                time = std::chrono::system_clock::now();
                duration = std::chrono::system_clock::now() - time;
                while(duration.count() < 1000){
                    std::this_thread::sleep_for(std::chrono::milliseconds{100});
                    duration = std::chrono::system_clock::now() - time;
                }
                sys.display_system_info();
                // std::cout << sys.to_json() << std::endl;
                sys2.from_json(sys.to_json());
                // std::cout << "==================" << std::endl;
                // // std::cout << sys2->to_json() << std::endl;
                // sys2.display_system_info();
                // std::cout << "++++++++++++++++++" << std::endl;
                sys2.from_bytes(sys.to_bytes());
                // sys2.to_bytes();
            }
        }
    }

    return 0;
}

int main(int argc, char** argv){
    return test2(argc, argv);
}