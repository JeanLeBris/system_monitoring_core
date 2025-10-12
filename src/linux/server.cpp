#include <sys/socket.h>
#include <iostream>

#include "server.hpp"

namespace server{
    SOCKET CreateSocket(){
        SOCKET fdsocket;
        if((fdsocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
            printf("socket creation failure: %s\n", strerror(WSAGetLastError()));
            exit(EXIT_FAILURE);
        }
        // printf("socket: %lld\n", fdsocket);
        return fdsocket;
    }

    SOCKET SetSocketOptions(SOCKET sock){
        int broadcast = 1;

        if(setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)) < 0){
            std::cout << "Error in setting Broadcast option";
            closesocket(sock);
            return 0;
        }

        return sock;
    }

    SOCKADDR_IN CreateServerSinForBroadcast(){
        SOCKADDR_IN sin;
        // sin.sin_addr.s_addr = INADDR_ANY;   // inet_addr(SIN_ADDR);
        // sin.sin_addr.s_addr = inet_addr("192.168.1.255");
        sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(4149);
        return sin;
    }

    SOCKADDR_IN CreateServerSinForNormalcast(){
        SOCKADDR_IN sin;
        sin.sin_addr.s_addr = INADDR_ANY;   // inet_addr(SIN_ADDR);
        // sin.sin_addr.s_addr = inet_addr("192.168.1.255");
        // sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(4149);
        return sin;
    }

    void BindingSocket(SOCKET *fdsocket, SOCKADDR_IN *sin){
        if(bind(*fdsocket, (SOCKADDR *) sin, sizeof(*sin)) != 0){
            printf("binding failure: %s\n", strerror(WSAGetLastError()));
            exit(EXIT_FAILURE);
        }
    }
}