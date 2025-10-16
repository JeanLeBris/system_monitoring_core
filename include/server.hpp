#ifndef SYSTEM_MONITORING_SERVER_HPP
#define SYSTEM_MONITORING_SERVER_HPP

#include <string>
#include <cstdint>

#ifdef _WIN64
#include <winsock2.h>
#include <WS2tcpip.h>
#endif
#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SOCKET int
#define SOCKADDR_IN sockaddr_in
#define SOCKADDR sockaddr
#endif

#define APP_PORT 4148
#define SLAVE_PORT 4149
// #define BROADCAST_ADDRESS "192.168.1.255"

namespace server{
    struct connection{
        SOCKET sockfd;
        SOCKADDR_IN dest_addr;
    };

    /**
     * Create a socket descriptor
     */
    SOCKET CreateSocket();

    /**
     * Set socket options to broadcast
     */
    SOCKET SetSocketOptions(SOCKET sock);

    /**
     * Filling server information
     */
    SOCKADDR_IN CreateServerSinForBroadcast(uint16_t port);

    /**
     * Filling server information
     */
    SOCKADDR_IN CreateServerSinForNormalcast(uint16_t port);

    /**
     * Bind a socket with an address
     */
    void BindingSocket(SOCKET *fdsocket, SOCKADDR_IN *sin);

    /**
     * Set up a master socket
     */
    connection SetUpMasterConnection();

    /**
     * Set up a slave socket
     */
    connection SetUpSlaveConnection();

    /**
     * Set up a app socket
     */
    connection SetUpAppConnection();
}

#endif