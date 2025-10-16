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
#include "server.hpp"

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

void *master_connection_thread_sys_level(void *_args){
    // thread_args *args = (thread_args*) _args;
    // monitoring::System *sys = args->sys;
    monitoring::System *sys = (monitoring::System*) _args;
    std::chrono::duration<double, std::milli> duration;
    std::chrono::system_clock::time_point time;
    
    #ifdef _WIN64
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    #endif

    server::connection conn = server::SetUpMasterConnection();

    socklen_t len = sizeof(conn.dest_addr);
    int n = 0;
    char buffer_string[10000] = "\0";
    std::string buffer_string2;

    time = std::chrono::system_clock::now();
    while(1){
        // std::this_thread::sleep_for(std::chrono::milliseconds{500});
        duration = std::chrono::system_clock::now() - time;
        while(duration.count() < 500){
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
            duration = std::chrono::system_clock::now() - time;
        }
        time = std::chrono::system_clock::now();
        // printf("ping\n");
        
        // sys->update_info();

        sendto(conn.sockfd, (const char *)"get info", strlen("get info"), 0, (const struct sockaddr *) &(conn.dest_addr), len);
        n = recvfrom(conn.sockfd, (char *)buffer_string, 10000, 0, ( struct sockaddr *) &(conn.dest_addr), &len);
        // std::cout << inet_ntoa(conn.dest_addr.sin_addr) << std::endl;
        buffer_string[n] = '\0';
        buffer_string2.assign(buffer_string);
        sys->from_json(buffer_string2);
    }
    // free(args);

    #ifdef _WIN64
    WSACleanup();
    #endif
    
    pthread_exit(NULL);
}

void *master_connection_thread_env_level(void *_args){
    // thread_args *args = (thread_args*) _args;
    // monitoring::System *sys = args->sys;
    monitoring::Environment *env = (monitoring::Environment*) _args;
    std::chrono::duration<double, std::milli> duration;
    std::chrono::system_clock::time_point time;
    
    #ifdef _WIN64
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    #endif

    server::connection conn = server::SetUpMasterConnection();

    socklen_t len = sizeof(conn.dest_addr);
    int n = 0;
    char buffer_string[100000] = "\0";
    std::string buffer_string2;

    time = std::chrono::system_clock::now();
    while(1){
        // std::this_thread::sleep_for(std::chrono::milliseconds{500});
        duration = std::chrono::system_clock::now() - time;
        while(duration.count() < 500){
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
            duration = std::chrono::system_clock::now() - time;
        }
        time = std::chrono::system_clock::now();
        // printf("ping\n");
        
        // sys->update_info();

        sendto(conn.sockfd, (const char *)"get info", strlen("get info"), 0, (const struct sockaddr *) &(conn.dest_addr), len);
        n = recvfrom(conn.sockfd, (char *)buffer_string, 100000, 0, ( struct sockaddr *) &(conn.dest_addr), &len);
        // std::cout << inet_ntoa(conn.dest_addr.sin_addr) << std::endl;
        buffer_string[n] = '\0';
        buffer_string2.assign(buffer_string);
        env->from_json(buffer_string2, inet_ntoa(conn.dest_addr.sin_addr));
    }
    // free(args);

    #ifdef _WIN64
    WSACleanup();
    #endif
    
    pthread_exit(NULL);
}

void *slave_connection_thread_sys_level(void *_args){
    // thread_args *args = (thread_args*) _args;
    // monitoring::System *sys = args->sys;
    monitoring::System *sys = (monitoring::System*) _args;
    std::chrono::duration<double, std::milli> duration;
    std::chrono::system_clock::time_point time;
    
    #ifdef _WIN64
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    #endif

    server::connection conn = server::SetUpSlaveConnection();

    // SOCKADDR_IN cliaddr;
    socklen_t len = sizeof(conn.dest_addr);
    int n = 0;
    char buffer_string[10000] = "\0";

    time = std::chrono::system_clock::now();
    while(1){
        // std::this_thread::sleep_for(std::chrono::milliseconds{500});
        duration = std::chrono::system_clock::now() - time;
        while(duration.count() < 500){
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
            duration = std::chrono::system_clock::now() - time;
        }
        time = std::chrono::system_clock::now();
        // printf("ping\n");
        
        // sys->update_info();

        n = recvfrom(conn.sockfd, (char *)buffer_string, 1024, 0, ( struct sockaddr *) &(conn.dest_addr), &len);
        sendto(conn.sockfd, (const char *) sys->to_json().c_str(), strlen(sys->to_json().c_str()), 0, (const struct sockaddr *) &(conn.dest_addr), len);
    }
    // free(args);

    #ifdef _WIN64
    WSACleanup();
    #endif
    
    pthread_exit(NULL);
}

void *slave_connection_thread_env_level(void *_args){
    // thread_args *args = (thread_args*) _args;
    // monitoring::System *sys = args->sys;
    monitoring::Environment *env = (monitoring::Environment*) _args;
    std::chrono::duration<double, std::milli> duration;
    std::chrono::system_clock::time_point time;
    
    #ifdef _WIN64
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    #endif

    server::connection conn = server::SetUpSlaveConnection();

    // SOCKADDR_IN cliaddr;
    socklen_t len = sizeof(conn.dest_addr);
    int n = 0;
    char buffer_string[10000] = "\0";

    time = std::chrono::system_clock::now();
    while(1){
        // std::this_thread::sleep_for(std::chrono::milliseconds{500});
        duration = std::chrono::system_clock::now() - time;
        while(duration.count() < 500){
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
            duration = std::chrono::system_clock::now() - time;
        }
        time = std::chrono::system_clock::now();
        // printf("ping\n");
        
        // sys->update_info();

        n = recvfrom(conn.sockfd, (char *)buffer_string, 1024, 0, ( struct sockaddr *) &(conn.dest_addr), &len);
        sendto(conn.sockfd, (const char *) env->to_json().c_str(), strlen(env->to_json().c_str()), 0, (const struct sockaddr *) &(conn.dest_addr), len);
    }
    // free(args);

    #ifdef _WIN64
    WSACleanup();
    #endif
    
    pthread_exit(NULL);
}

int test2(int argc, char** argv){
    if(argc == 2){
        if(strcmp(argv[1], "slave") == 0){
            // monitoring::System sys =  monitoring::System();
            // sys.basic_update_info();
            // // monitoring::System *sys = new monitoring::System;
            // // sys.update_info();
            // // sys.display_system_info();

            monitoring::Environment env = monitoring::Environment();

            // thread_args *args = (thread_args*) malloc(sizeof(thread_args));
            pthread_t sniffer_thread_1, sniffer_thread_2;
            if(pthread_create(&sniffer_thread_1, NULL, sys_updater, (void*) env.get_system_by_key("local")) < 0){
                perror("could not create thread");
                return 1;
            }
            // if(pthread_create(&sniffer_thread_2, NULL, slave_connection_thread_sys_level, (void*) env.get_system_by_key("local")) < 0){
            //     perror("could not create thread");
            //     return 1;
            // }
            if(pthread_create(&sniffer_thread_2, NULL, slave_connection_thread_env_level, (void*) &env) < 0){
                perror("could not create thread");
                return 1;
            }
            
            // #ifdef _WIN64
            // WSADATA WSAData;
            // WSAStartup(MAKEWORD(2, 0), &WSAData);
            // #endif

            // // SOCKET sockfd = server::CreateSocket();
            // // // sockfd = server::SetSocketOptions(sockfd);
            // // SOCKADDR_IN servaddr = server::CreateServerSinForNormalcast();
            // // server::BindingSocket(&sockfd, &servaddr);

            // server::connection conn = server::SetUpSlaveConnection();

            // // SOCKADDR_IN cliaddr;
            // socklen_t len = sizeof(conn.dest_addr);
            // int n = 0;
            // char buffer_string[10000] = "\0";

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
                // sys.display_system_info();
                // sys->display_system_info();
                
                // n = recvfrom(conn.sockfd, (char *)buffer_string, 1024, 0, ( struct sockaddr *) &(conn.dest_addr), &len);
                // sendto(conn.sockfd, (const char *) sys.to_json().c_str(), strlen(sys.to_json().c_str()), 0, (const struct sockaddr *) &(conn.dest_addr), len);
            }
            // delete sys;
            
            #ifdef _WIN64
            WSACleanup();
            #endif
        }
        else if(strcmp(argv[1], "master") == 0){
            // monitoring::System sys = monitoring::System();
            // sys.basic_update_info();
            // // monitoring::System *sys2 = new monitoring::System;
            // monitoring::System sys2 = monitoring::System();
            // // sys2->basic_update_info();

            monitoring::Environment env = monitoring::Environment();
            env.push("other", new monitoring::System());

            pthread_t sniffer_thread_1, sniffer_thread_2;
            if(pthread_create(&sniffer_thread_1, NULL, sys_updater, (void*) env.get_system_by_key("local")) < 0){
                perror("could not create thread");
                return 1;
            }
            // if(pthread_create(&sniffer_thread_2, NULL, master_connection_thread_sys_level, (void*) env.get_system_by_key("other")) < 0){
            //     perror("could not create thread");
            //     return 1;
            // }
            if(pthread_create(&sniffer_thread_2, NULL, master_connection_thread_env_level, (void*) &env) < 0){
                perror("could not create thread");
                return 1;
            }

            // #ifdef _WIN64
            // WSADATA WSAData;
            // WSAStartup(MAKEWORD(2, 0), &WSAData);
            // #endif

            // // SOCKET sockfd = server::CreateSocket();
            // // sockfd = server::SetSocketOptions(sockfd);
            // // SOCKADDR_IN servaddr = server::CreateServerSinForBroadcast();

            // server::connection conn = server::SetUpMasterConnection();

            // socklen_t len = sizeof(conn.dest_addr);
            // int n = 0;
            // char buffer_string[10000] = "\0";
            // std::string buffer_string2;
            // // server::BindingSocket(&sockfd, &servaddr);

            std::chrono::duration<double, std::milli> duration;
            std::chrono::system_clock::time_point time;
            while(1){
                time = std::chrono::system_clock::now();
                duration = std::chrono::system_clock::now() - time;
                while(duration.count() < 1000){
                    std::this_thread::sleep_for(std::chrono::milliseconds{100});
                    duration = std::chrono::system_clock::now() - time;
                }

                // sendto(conn.sockfd, (const char *)"get info", strlen("get info"), 0, (const struct sockaddr *) &(conn.dest_addr), len);
                // n = recvfrom(conn.sockfd, (char *)buffer_string, 10000, 0, ( struct sockaddr *) &(conn.dest_addr), &len);
                // std::cout << inet_ntoa(conn.dest_addr.sin_addr) << std::endl;
                // buffer_string[n] = '\0';
                // buffer_string2.assign(buffer_string);
                // sys2.from_json(buffer_string2);

                // env.get_system_by_key("local")->display_system_info();
                // std::cout << "==================" << std::endl;
                // env.get_system_by_key("other")->display_system_info();
                std::cout << "++++++++++++++++++" << std::endl;
                std::cout << env.to_json() << std::endl;
            }

            // #ifdef _WIN64
            // WSACleanup();
            // #endif
        }
    }

    return 0;
}

int main(int argc, char** argv){
    return test2(argc, argv);
}