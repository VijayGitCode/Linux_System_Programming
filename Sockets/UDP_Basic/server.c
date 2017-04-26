#include <stdio.h>         // printf
#include <sys/socket.h>    // Socket APIs
#include <netinet/in.h>    // AF_INET
#include <string.h>        // string APIs

#define SERV_ADDR "10.144.172.186"
#define SIZE      50

static void RX_ClientData(const int iSockfd, struct sockaddr *const pCli_addr);

static void RX_ClientData(const int iSockfd, struct sockaddr *const pCli_addr)
{
    int  size           =  0;
    char recvbuff[SIZE] = {0}; 
    char sendbuff[SIZE] = {0};
    int  cli_length     = sizeof(struct sockaddr_in);

    while(1)
    {
        // Receive from client
        printf("%s: Ready to Receive..\n", __FILE__);
        size = recvfrom(iSockfd, recvbuff, SIZE, 0, (struct sockaddr *)pCli_addr, \
                                                        (socklen_t *)&cli_length); 
        if(size < 0)
        {
            perror("recvfrom failed..");
        } else
        {
            printf("%s: The received string is \"%s\"\n", __FILE__, recvbuff);
        }

        // Transmit to client
        printf("%s: Sending ACK..\n");
        strcpy(sendbuff, "ACK: ");
        strcat(sendbuff, recvbuff);
        size = sendto(iSockfd, sendbuff, strlen(sendbuff), 0, (struct sockaddr *)pCli_addr, \
                                                                (socklen_t )cli_length);  
        if(size < 0)
        {
            perror("sendto failed..");
        }
    }
}

main()
{
    int iSockfd =  0;
    int iRet    = -1;
    struct sockaddr_in serv_addr = {0};
    struct sockaddr_in cli_addr  = {0};
  
    // Socket Creation 
    iSockfd = socket(AF_INET, SOCK_DGRAM, 0); // 0 means letting the OS to decide the 
                                              // protocol based on the combo of 1st two args
    if(iSockfd < 0)
    {
        perror("Socket Creation failed..");
    }

    bzero(&serv_addr, sizeof(serv_addr)); 
    bzero(&serv_addr, sizeof(cli_addr));
  
    // Initialise address and port values 
    serv_addr.sin_family  =  AF_INET;
    iRet = inet_pton(AF_INET, SERV_ADDR, &serv_addr.sin_addr);
    if(iRet != 1)
    {
        perror("inet_pton failed..");
    }
    serv_addr.sin_port  =  htons(5000);
   
    // Bind the socket 
    iRet = bind(iSockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(iRet < 0)
    {
        perror("bind failed..");
    }

    // Communicate with the client
    RX_ClientData(iSockfd, (struct sockaddr *)&cli_addr);
}
