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
#include <unistd.h>

#include "monitoring.hpp"
#include "output_converter.hpp"
#include "utils.hpp"
#include "server.hpp"
#include "argparse.hpp"

int test1(int argc, char** argv){
    monitoring::System sys1 = monitoring::System();

    for(int i = 0; i < 10000; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds{500});

        sys1.update_info();
        sys1.display_system_info();
    }

    return 0;
}

struct thread_args{
    monitoring::Environment *env;
    std::string target_ip;
    bool *access_open;
};

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

void *master_connection_thread_env_level(void *_args){
    // thread_args *args = (thread_args*) _args;
    // monitoring::System *sys = args->sys;
    thread_args *args = (thread_args*) _args;
    monitoring::Environment *env = args->env;
    std::string target_ip = args->target_ip;
    std::chrono::duration<double, std::milli> duration;
    std::chrono::system_clock::time_point time;
    
    #ifdef _WIN64
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    #endif

    server::connection conn = server::SetUpUnicastMasterConnection(target_ip);

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
        n = recvfrom(conn.sockfd, (char *)buffer_string, 99999, 0, ( struct sockaddr *) &(conn.dest_addr), &len);
        if(n <= 0){
            // std::cout << inet_ntoa(conn.dest_addr.sin_addr) << " : " << n << std::endl;
            env->get_system_by_key(inet_ntoa(conn.dest_addr.sin_addr))->set_accessed(false);
        }
        else{
            // std::cout << inet_ntoa(conn.dest_addr.sin_addr) << " : " << n << std::endl;
            // std::cout << inet_ntoa(conn.dest_addr.sin_addr) << std::endl;
            buffer_string[n] = '\0';
            buffer_string2.assign(buffer_string);
            // while(!*access_open);{
            //     std::this_thread::sleep_for(std::chrono::milliseconds{10});
            // }
            // *access_open = false;
            env->from_json(buffer_string2, inet_ntoa(conn.dest_addr.sin_addr));
            // *access_open = true;
            // env->from_json(buffer_string2, target_ip);
            // env->from_json(buffer_string2, "192.168.1.98");
            // std::cout << inet_ntoa(conn.dest_addr.sin_addr) << std::endl;
            // std::cout << buffer_string2 << std::endl;
        }
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

        recvfrom(conn.sockfd, (char *)buffer_string, 9999, 0, ( struct sockaddr *) &(conn.dest_addr), &len);
        // if(n > 0){
        //     // printf("%s\n", buffer_string);
        // }
        sendto(conn.sockfd, (const char *) env->to_json().c_str(), strlen(env->to_json().c_str()), 0, (const struct sockaddr *) &(conn.dest_addr), len);
    }
    // free(args);

    #ifdef _WIN64
    WSACleanup();
    #endif
    
    pthread_exit(NULL);
}

void *app_test_local(void *_args){
    // thread_args *args = (thread_args*) _args;
    // monitoring::System *sys = args->sys;
    monitoring::Environment *env = (monitoring::Environment*) _args;
    std::chrono::system_clock::time_point time;
    
    #ifdef _WIN64
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    #endif

    // create a socket linked to a specific transport service provider
    SOCKET fdsocket = socket(AF_INET, SOCK_STREAM, 0);
    if((long long unsigned int) fdsocket == (long long unsigned int) -1){
        // printf("socket creation failure: %s\n", strerror(WSAGetLastError()));
        printf("socket creation failure\n");
        exit(EXIT_FAILURE);
    }

    // Specify a transport address and a port for the server
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   // inet_addr(SIN_ADDR);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(APP_PORT);

    // Associates a local address with a socket
    if(bind(fdsocket, (SOCKADDR *) &sin, sizeof(sin)) != 0){
        // printf("binding failure: %s\n", strerror(WSAGetLastError()));
        printf("binding failure\n");
        exit(EXIT_FAILURE);
    }

    // change socket state to listening
    if(listen(fdsocket, 3) != 0){
        // printf("listening failure: %s\n", strerror(WSAGetLastError()));
        printf("listening failure\n");
        exit(EXIT_FAILURE);
    }

    SOCKADDR_IN clientAdress;
    SOCKET clientSocket;

    socklen_t addrlen = sizeof(clientAdress);
    char buffer_string[10000] = "\0";
    std::string prefix = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: ";
    // "%zu\r\n\r\n";
    std::string content;

    time = std::chrono::system_clock::now();
    while(1){
        clientSocket = accept(fdsocket, (SOCKADDR *) &clientAdress, &addrlen);
        if((long long unsigned int) clientSocket != (long long unsigned int) -1){
            char ip[16];
            inet_ntop(AF_INET, &(clientAdress.sin_addr), ip, 16);
            // printf("connexion: %s:%i\n", ip, clientAdress->sin_port);
        }

        // std::cout << env->to_json() << std::endl;
        recv(clientSocket, buffer_string, 9999, 0);
        // std::this_thread::sleep_for(std::chrono::milliseconds{1000});
        // std::cout << env->to_json() << std::endl;
        content = env->to_json();
        send(clientSocket, (prefix + std::to_string(content.length()) + "\r\n\r\n" + content).c_str(), (prefix + std::to_string(content.length()) + "\r\n\r\n" + content).length(), 0);

        #ifdef _WIN64
        closesocket(clientSocket);
        #endif
        #ifdef __linux__
        close(clientSocket);
        #endif
    }
    // free(args);

    #ifdef _WIN64
    WSACleanup();
    #endif
    
    pthread_exit(NULL);
}

void *app_test_remote(void *_args){
    // thread_args *args = (thread_args*) _args;
    // monitoring::System *sys = args->sys;
    monitoring::Environment *env = (monitoring::Environment*) _args;
    std::chrono::system_clock::time_point time;
    
    #ifdef _WIN64
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    #endif

    // create a socket linked to a specific transport service provider
    SOCKET fdsocket = socket(AF_INET, SOCK_STREAM, 0);
    if((long long unsigned int) fdsocket == (long long unsigned int) -1){
        // printf("socket creation failure: %s\n", strerror(WSAGetLastError()));
        printf("socket creation failure\n");
        exit(EXIT_FAILURE);
    }

    // Specify a transport address and a port for the server
    SOCKADDR_IN sin;
    sin.sin_addr.s_addr = INADDR_ANY;   // inet_addr(SIN_ADDR);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(APP_PORT);

    // Associates a local address with a socket
    if(bind(fdsocket, (SOCKADDR *) &sin, sizeof(sin)) != 0){
        // printf("binding failure: %s\n", strerror(WSAGetLastError()));
        printf("binding failure\n");
        exit(EXIT_FAILURE);
    }

    // change socket state to listening
    if(listen(fdsocket, 3) != 0){
        // printf("listening failure: %s\n", strerror(WSAGetLastError()));
        printf("listening failure\n");
        exit(EXIT_FAILURE);
    }

    SOCKADDR_IN clientAdress;
    SOCKET clientSocket;

    socklen_t addrlen = sizeof(clientAdress);
    char buffer_string[10000] = "\0";
    std::string prefix = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nContent-Length: ";
    // "%zu\r\n\r\n";
    std::string content;

    time = std::chrono::system_clock::now();
    while(1){
        clientSocket = accept(fdsocket, (SOCKADDR *) &clientAdress, &addrlen);
        if((long long unsigned int) clientSocket != (long long unsigned int) -1){
            char ip[16];
            inet_ntop(AF_INET, &(clientAdress.sin_addr), ip, 16);
            // printf("connexion: %s:%i\n", ip, clientAdress->sin_port);
        }

        // std::cout << env->to_json() << std::endl;
        recv(clientSocket, buffer_string, 9999, 0);
        // std::this_thread::sleep_for(std::chrono::milliseconds{1000});
        // std::cout << env->to_json() << std::endl;
        content = env->to_json();
        send(clientSocket, (prefix + std::to_string(content.length()) + "\r\n\r\n" + content).c_str(), (prefix + std::to_string(content.length()) + "\r\n\r\n" + content).length(), 0);

        #ifdef _WIN64
        closesocket(clientSocket);
        #endif
        #ifdef __linux__
        close(clientSocket);
        #endif
    }
    // free(args);

    #ifdef _WIN64
    WSACleanup();
    #endif
    
    pthread_exit(NULL);
}

int test2(int argc, char** argv){
    auto parser = argparse::ArgumentParser(argv[0],
                                           NULL,
                                           "This is the system_monitoring's core",
                                           "As this is just the core, it is not the GUI",
                                           NULL,
                                           "-",
                                           NULL,
                                           "0.1.0",
                                           true,
                                           true,
                                           true);
    
    parser.add_argument("-ip --ip-addresses", "store", 1, NULL, "", "string", 0, NULL, false, "IP addresses file", NULL, NULL, 0);
    const char* choices1[3] = {"none", "local", "remote"};
    parser.add_argument("-aa --app-access", "store", 1, NULL, "none", "string", 3, choices1, false, "Type of access for the app", NULL, NULL, 0);

    argparse::ParsedArguments* parsed_args = parser.parse_args(argc, argv);

    std::string ip_addresses_file;
    if(parsed_args->get_value_by_key("ip-addresses")._string != NULL)
        ip_addresses_file = parsed_args->get_value_by_key("ip-addresses")._string;
    else
        ip_addresses_file = "";

    std::vector<std::string> ip_addresses = get_ip_addresses(ip_addresses_file);

    std::string app_access;
    if(parsed_args->get_value_by_key("app-access")._string != NULL)
        app_access = parsed_args->get_value_by_key("app-access")._string;
    else
        app_access = "";

    // parsed_args->print_keys_and_values();
    // std::cout << mode << " : " << ip_addresses.size() << std::endl;

    monitoring::Environment env = monitoring::Environment();
    for(long unsigned int i = 1; i < ip_addresses.size(); i++){
        env.push(ip_addresses.at(i), new monitoring::System());
    }

    pthread_t sniffer_thread_1, sniffer_thread_2, sniffer_thread_3;
    std::vector<pthread_t*> master_threads;
    std::vector<thread_args*> master_thread_args;
    bool access_open = true;
    if(pthread_create(&sniffer_thread_1, NULL, sys_updater, (void*) env.get_system_by_key("local")) < 0){
        perror("could not create thread");
        return 1;
    }
    for(long unsigned int i = 1; i < ip_addresses.size(); i++){
        master_threads.push_back(new pthread_t());
        master_thread_args.push_back(new thread_args);
        master_thread_args.at(master_thread_args.size()-1)->env = &env;
        master_thread_args.at(master_thread_args.size()-1)->target_ip = ip_addresses.at(i);
        master_thread_args.at(master_thread_args.size()-1)->access_open = &access_open;
        if(pthread_create(master_threads.at(master_threads.size()-1), NULL, master_connection_thread_env_level, (void*) master_thread_args.at(master_thread_args.size()-1)) < 0){
            perror("could not create thread");
            return 1;
        }
    }
    if(pthread_create(&sniffer_thread_2, NULL, slave_connection_thread_env_level, (void*) &env) < 0){
        perror("could not create thread");
        return 1;
    }
    if(app_access == "local"){
        if(pthread_create(&sniffer_thread_3, NULL, app_test_local, (void*) &env) < 0){
            perror("could not create thread");
            return 1;
        }
    }
    else if(app_access == "remote"){
        if(pthread_create(&sniffer_thread_3, NULL, app_test_remote, (void*) &env) < 0){
            perror("could not create thread");
            return 1;
        }
    }
    else if(app_access == "none"){

    }
    else{
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

        env.display_environment_info();
    }

    return 0;
}

int main(int argc, char** argv){
    return test2(argc, argv);
}