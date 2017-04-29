#include <stdio.h>
#include <string.h>   // string APIs
#include <unistd.h>   // pid_t, STDOUT_FILENO
#include <stdlib.h>   // exit
#include <fcntl.h>    // O_RDONLY

#define BUFFSIZE 50

void client (const int readfd, const writefd);
void server(const int readfd, const writefd);

void client (const int readfd, const writefd)
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

void server(const int readfd, const writefd)
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
    int pipe1[2] = {0};
    int pipe2[2] = {0};
    int iRet     = 0;
    pid_t childpid = 0;

    // Create pipe 1
    iRet = pipe(pipe1);
    if(iRet < 0)
    {
        perror("pipe1 failed..");
    }

    // Create pipe 2
    iRet = pipe(pipe2);
    if(iRet < 0)
    {
        perror("pipe2 failed..");
    }

    // Fork a child
    childpid = fork();
    if(childpid == 0)  // Child
    {
        close(pipe1[1]);
        close(pipe2[0]);
        server(pipe1[0], pipe2[1]);
    } else  // Parent
    {
        close(pipe1[0]);
        close(pipe2[1]);
        client(pipe2[0], pipe1[1]);
        waitpid(childpid, NULL, 0);
        exit(0);
    }
}

