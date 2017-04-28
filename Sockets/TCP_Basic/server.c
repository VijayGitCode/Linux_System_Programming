#include <stdio.h>         // printf
#include <sys/socket.h>    // Socket APIs
#include <netinet/in.h>    // AF_INET
#include <string.h>        // string APIs

#define SERV_ADDR "192.168.56.111"
#define SIZE      50

static void RX_ClientData(const int iConnfd, struct sockaddr *const pCli_addr);

static void RX_ClientData(const int iConnfd, struct sockaddr *const pCli_addr)
{
    int  size           =  0;
    char recvbuff[SIZE] = {0}; 
    char sendbuff[SIZE] = {0};
    int  cli_length     = sizeof(struct sockaddr_in);

    while(1)
    {
        // Receive from client
        printf("%s: Ready to Receive..\n", __FILE__);
        size = recv(iConnfd, recvbuff, SIZE, 0);
        if(size < 0)
        {
            perror("recv failed..");
        } else
        {
            printf("%s: The received string is: %s\n", __FILE__, recvbuff);
        }

        // Transmit to client
        printf("%s: Sending ACK..\n", __FILE__);
        strcpy(sendbuff, "ACK: ");
        strcat(sendbuff, recvbuff);
        size = send(iConnfd, sendbuff, strlen(sendbuff), 0);
        if(size < 0)
        {
            perror("send failed..");
        }
    }
}

main()
{
    int iSockfd =  0;
    int iConnfd =  0;
    int iRet    = -1;
    struct sockaddr_in serv_addr = {0};
    struct sockaddr_in cli_addr  = {0};
    int cli_len = sizeof(cli_addr);
  
    // Socket Creation 
    iSockfd = socket(AF_INET, SOCK_STREAM, 0); // 0 means letting the OS to decide the 
                                               // protocol based on the combo of 1st two args
    if(iSockfd < 0)
    {
        perror("Socket Creation failed..");
    }

    bzero(&serv_addr, sizeof(serv_addr)); 
    bzero(&cli_addr, sizeof(cli_addr));
  
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

    // Listen to incoming connections
    iRet = listen(iSockfd, 1);
    if(iRet != 0)
    {
        perror("listen failed..");
    }
    
    // Accept client connections
    iConnfd = accept(iSockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_len);
    if(iConnfd < 0)
    {
        perror("Accept failure..");
    }

    // Communicate with the client
    RX_ClientData(iConnfd, (struct sockaddr *)&cli_addr);
}
