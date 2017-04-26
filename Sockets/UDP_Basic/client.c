#include <stdio.h>         // printf
#include <sys/socket.h>    // Sockets API
#include <net/netinet.h>   // AF_INET
#include <string.h>        // String APIs

#define CLI_ADDR 10.144.172.187
#define SIZE     50


static void TX_DataToServer()

main()
{
    int iSockfd = -1;
    int iRet    = -1;
    struct sockaddr_in serv_addr = {0};

    // Socket Creation
    iSockfd = socket(AF_INET, SOCK_DGRAM, 0); // 0 means letting the OS to decide
                                              // the protocol based on 1st 2 args
    if(iSockfd < 0)
    {
        perror("Error in Socket Creation");
    }

    bzero(&serv_addr, sizeof(serv_addr));

    // Initialise the port and address values
    serv_addr.sin_family = AF_INET;
    iRet = inet_pton(AF_INET, SERV_ADDR, &serv_addr.sin_addr);
    if(iRet != 1)
    {
        perror("inet_pton failed..");
    }

    serv_addr.sin_port = htons(5000);

     
}
