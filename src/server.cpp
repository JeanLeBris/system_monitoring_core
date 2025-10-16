#include "server.hpp"

namespace server{
    connection SetUpMasterConnection(){
        connection output;

        output.sockfd = server::CreateSocket();
        output.sockfd = server::SetSocketOptions(output.sockfd);
        output.dest_addr = server::CreateServerSinForBroadcast();

        return output;
    }

    connection SetUpSlaveConnection(){
        connection output;
        SOCKADDR_IN servaddr;

        output.sockfd = server::CreateSocket();
        servaddr = server::CreateServerSinForNormalcast();
        server::BindingSocket(&(output.sockfd), &servaddr);

        // SOCKADDR_IN cliaddr;

        return output;
    }
}