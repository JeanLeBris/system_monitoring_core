#include "server.hpp"

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

    connection SetUpAppConnection(){
        connection output;
        SOCKADDR_IN servaddr;

        output.sockfd = server::CreateSocket();
        servaddr = server::CreateServerSinForNormalcast(APP_PORT);
        server::BindingSocket(&(output.sockfd), &servaddr);

        // SOCKADDR_IN cliaddr;

        return output;
    }
}