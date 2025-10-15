#ifndef SYSTEM_MONITORING_SERVER_HPP
#define SYSTEM_MONITORING_SERVER_HPP

#include <string>

#ifdef __linux__
#define SOCKET int
#define SOCKADDR_IN sockaddr_in
#define SOCKADDR sockaddr
#endif

namespace server{
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
    SOCKADDR_IN CreateServerSinForBroadcast();

    /**
     * Filling server information
     */
    SOCKADDR_IN CreateServerSinForNormalcast();

    /**
     * Bind a socket with an address
     */
    void BindingSocket(SOCKET *fdsocket, SOCKADDR_IN *sin);
}

#endif