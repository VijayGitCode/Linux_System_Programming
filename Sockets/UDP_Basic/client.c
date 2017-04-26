#include <stdio.h>         // printf
#include <sys/socket.h>    // Sockets API
#include <netinet/in.h>    // AF_INET
#include <string.h>        // String APIs

#define CLI_ADDR "10.144.172.187"
#define SIZE     50


static void TX_DataToServer(const int iSockfd, const struct sockaddr *pServAddr);


static void TX_DataToServer(const int iSockfd, const struct sockaddr *pServAddr)
{
    int   size     = 0;
    int   serv_len = sizeof(struct sockaddr_in);
    char  sendbuff[SIZE] = {0};
    char  recvbuff[SIZE] = {0};

    while(1)
    {
        printf("%s Enter Input String to send:\n", __FILE__);

        fgets(sendbuff, sizeof(sendbuff), stdin);

        size = sendto(iSockfd, sendbuff, sizeof(sendbuff), 0, (struct sockaddr *)pServAddr, \
                                                                           (socklen_t) serv_len);
        if(size < 0)
        {
            perror("Sendto failed..");
        }

        size = recvfrom(iSockfd, recvbuff, sizeof(recvbuff), 0, (struct sockaddr *)pServAddr, \
                                                                    (socklen_t *)&serv_len);
        if(size < 0)
        {
            perror("recvfrom failed..");
        }

        printf("%s: Received from server is %s\n", __FILE__, recvbuff);
    }
}

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
    iRet = inet_pton(AF_INET, CLI_ADDR, &serv_addr.sin_addr);
    if(iRet != 1)
    {
        perror("inet_pton failed..");
    }

    serv_addr.sin_port = htons(5000);

    TX_DataToServer(iSockfd, (struct sockaddr *)&serv_addr);
     
}
