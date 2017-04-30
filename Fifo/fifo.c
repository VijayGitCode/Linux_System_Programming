#include <stdio.h>
#include <string.h>   // string APIs
#include <unistd.h>   // pid_t, STDOUT_FILENO
#include <stdlib.h>   // exit
#include <fcntl.h>    // O_RDONLY
#include <sys/stat.h> // S_IRUSR
#include <errno.h>    // errno

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"
#define BUFFSIZE 50
#define MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP

void client (const int readfd, const int writefd);
void server(const int readfd, const int writefd);

void client (const int readfd, const int writefd)
{
    char *ptr = NULL;
    int   n   = 0;
    int file_len = 0;

    char recvbuff[BUFFSIZE] = {0};
    char readbuff[BUFFSIZE] = {0};

    // Get the file name from user
    printf("%s: Give a file name..\n", __func__);
    ptr = fgets(recvbuff, BUFFSIZE, stdin);
    if(ptr == NULL)
    {
        perror("fgets failed..");
    }

    file_len = strlen(recvbuff);

    // Discard new line
    if(recvbuff[file_len - 1] == '\n')
    {
        file_len--;
        recvbuff[file_len] = '\0';
    }

    printf("%s: Writing %s to Server..\n", __func__, recvbuff);
    n = write(writefd, recvbuff, file_len); 
    if(n != file_len)
    { 
        perror("write failed..");
    }

    n = 0;

    do
    {
        n = read(readfd, readbuff, BUFFSIZE);
        if(n < 0)
        {
            perror("Read failed..");
        }
        readbuff[n] = '\0';
        printf("%s: read %s from server..\n", __func__, readbuff);

        n = write(STDOUT_FILENO, readbuff, n);
        if(n < 0)
        {
            perror("write failed..");
        }
        readbuff[n] = '\0';
        printf("%s: written %s to STDOUT..\n", __func__, readbuff);
    } while(n > 0);
}

void server(const int readfd, const int writefd)
{
    int n = 0;
    int fd = 0;
    char readbuff[BUFFSIZE] = {0};

    n = read(readfd, readbuff, BUFFSIZE);
    if(n < 0)
    {
        perror("read failed..");
    }
    readbuff[n] = '\0';
    printf("%s: Read %s from client..\n", __func__, readbuff);

    printf("%s: Opening %s..\n", __func__, readbuff);
    fd = open(readbuff, O_RDONLY);
    if(fd < 0)
    {
        perror("Open failed..");
    }

    memset(readbuff, 0, sizeof(readbuff));
    n = 0;

    do
    {
        n = read(fd, readbuff, BUFFSIZE);
        if(n < 0)
        {
            perror("Read failed..");
        }
        readbuff[n] = '\0';
        printf("%s: Read %s..\n", __func__, readbuff);

        n = write(writefd, readbuff, n);
        if(n < 0)
        {
            perror("write failed..");
        }
        readbuff[n] = '\0';
        printf("%s: Written %s to client..\n", __func__, readbuff);
    } while(n > 0);
}

int main()
{
    int readfd  = {0};
    int writefd = {0};
    int iRet    = 0;
    pid_t childpid = 0;

    // Create fifo 1
    iRet = mkfifo(FIFO1, MODE);
    if((iRet < 0) && (errno != EEXIST))
    {
        perror("mkfifo1 failed..");
    }

    // Create fifo 2
    iRet = mkfifo(FIFO2, MODE);
    if((iRet < 0) && (errno != EEXIST))
    {
        unlink(FIFO1);
        perror("mkfifo2 failed..");
    }

    // Fork a child
    childpid = fork();
    if(childpid == 0)  // Child
    {
        readfd = open(FIFO1, O_RDONLY, 0);
        writefd = open(FIFO2, O_WRONLY, 0);
        server(readfd, writefd);
    } else  // Parent
    {
        writefd = open(FIFO1, O_WRONLY, 0);
        readfd = open(FIFO2, O_RDONLY, 0);
        client(readfd, writefd);
        waitpid(childpid, NULL, 0);
        close(readfd);
        close(writefd);
        unlink(FIFO1);
        unlink(FIFO2);
        exit(0);
    }
}

