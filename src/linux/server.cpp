// #include <sys/socket.h>
#include <iostream>

#include <bits/stdc++.h>
#include <stdlib.h>
// #include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "server.hpp"

namespace server{
    SOCKET CreateSocket(){
        SOCKET fdsocket;
        if((fdsocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
            printf("socket creation failure\n");
            exit(EXIT_FAILURE);
        }
        // printf("socket: %lld\n", fdsocket);
        return fdsocket;
    }

    SOCKET SetSocketBroadcastOptions(SOCKET sock){
        int broadcast = 1;

        if(setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)) < 0){
            std::cout << "Error in setting Broadcast option";
            close(sock);
            return 0;
        }

        return sock;
    }

    SOCKET SetSocketTimeoutOptions(SOCKET sock){
        timeval timeout;

        // timeout.tv_sec = 3;   // seconds
        timeout.tv_usec = 100000;  // microseconds
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            perror("setsockopt failed");
            close(sock);
            exit(EXIT_FAILURE);
        }

        return sock;
    }

    SOCKADDR_IN CreateServerSinForBroadcast(uint16_t port){
        sockaddr_in sin;
        // sin.sin_addr.s_addr = INADDR_ANY;   // inet_addr(SIN_ADDR);
        // sin.sin_addr.s_addr = inet_addr("192.168.1.255");
        sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        return sin;
    }

    SOCKADDR_IN CreateServerSinForUnicast(std::string ip_address, uint16_t port){
        SOCKADDR_IN sin;
        // sin.sin_addr.s_addr = INADDR_ANY;   // inet_addr(SIN_ADDR);
        sin.sin_addr.s_addr = inet_addr(ip_address.c_str());
        // sin.sin_addr.s_addr = inet_addr("255.255.255.255");
        // sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        return sin;
    }

    SOCKADDR_IN CreateServerSinForNormalcast(uint16_t port){
        SOCKADDR_IN sin;
        sin.sin_addr.s_addr = INADDR_ANY;   // inet_addr(SIN_ADDR);
        // sin.sin_addr.s_addr = inet_addr("192.168.1.255");
        // sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        return sin;
    }

    void BindingSocket(SOCKET *fdsocket, SOCKADDR_IN *sin){
        if(bind(*fdsocket, (SOCKADDR *) sin, sizeof(*sin)) != 0){
            printf("binding failure\n");
            exit(EXIT_FAILURE);
        }
    }

    // void ListenSocket(SOCKET *fdsocket){
    //     if(listen(*fdsocket, 3) != 0){
    //         printf("listening failure: %s\n", strerror(WSAGetLastError()));
    //         exit(EXIT_FAILURE);
    //     }
    // }

    // SOCKET AcceptClientSocket(SOCKET *fdsocket, SOCKADDR_IN *clientAdress){
    //     int clientSocket;
    //     int addrlen = sizeof(*clientAdress);
    //     if((clientSocket = accept(*fdsocket, (SOCKADDR *) clientAdress, &addrlen)) != -1){
    //         char ip[16];
    //         inet_ntop(AF_INET, &(clientAdress->sin_addr), ip, 16);
    //         // printf("connexion: %s:%i\n", ip, clientAdress->sin_port);
    //     }
    //     return clientSocket;
    // }
}