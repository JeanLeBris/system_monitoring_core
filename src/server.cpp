#include "server.hpp"

namespace server{
    connection SetUpBroadcastMasterConnection(){
        connection output;

        output.sockfd = server::CreateSocket();
        output.sockfd = server::SetSocketBroadcastOptions(output.sockfd);
        output.dest_addr = server::CreateServerSinForBroadcast(SLAVE_PORT);

        return output;
    }
    
    connection SetUpUnicastMasterConnection(std::string ip_address){
        connection output;

        output.sockfd = server::CreateSocket();
        output.sockfd = server::SetSocketTimeoutOptions(output.sockfd);
        output.dest_addr = server::CreateServerSinForUnicast(ip_address, SLAVE_PORT);
        // if (inet_pton(AF_INET, ip_address.c_str(), &output.dest_addr.sin_addr) <= 0) {
        //     perror("Invalid address / Address not supported");
        //     // close(sockfd);
        //     exit(EXIT_FAILURE);
        // }

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