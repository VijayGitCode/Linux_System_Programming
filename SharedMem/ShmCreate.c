#include <stdio.h>
#include <sys/mman.h>
#include <mqueue.h>  // Message Queue

#define MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP

int main(int argc, char **argv)
{
   int     flags  = O_RDWR | O_CREAT;
   int     iRet   = -1;
   int     fd     =  0;
   char   *ptr    =  NULL;
   int     length =  50;

   // Create a Msg Queue
   printf("%s: Creating shared mem  %s\n", __FILE__, argv[1]);
   fd = shm_open(argv[1], flags, MODE);
   if(fd < 0)
   {
       perror("shm_open failed..");
   }

   iRet = ftruncate(fd, length);
   if(fd < 0)
   {
       perror("ftruncate failed..");
   }

   ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if((int)ptr == -1)
   {
       perror("mmap failed..");
   }  
   printf("%s: mmap value - %p\n", __FILE__, ptr);
}
