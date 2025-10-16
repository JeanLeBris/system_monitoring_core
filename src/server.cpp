#include "server.hpp"

// #define MASTER_PORT 4148
#define SLAVE_PORT 4149

namespace server{
    connection SetUpMasterConnection(){
        connection output;

        output.sockfd = server::CreateSocket();
        output.sockfd = server::SetSocketOptions(output.sockfd);
        output.dest_addr = server::CreateServerSinForBroadcast(SLAVE_PORT);

        return output;
    }

    connection SetUpSlaveConnection(){
        connection output;
        SOCKADDR_IN servaddr;

        output.sockfd = server::CreateSocket();
        servaddr = server::CreateServerSinForNormalcast(SLAVE_PORT);
        server::BindingSocket(&(output.sockfd), &servaddr);

        // SOCKADDR_IN cliaddr;

        return output;
    }
}