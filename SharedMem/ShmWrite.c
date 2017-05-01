#include <stdio.h>
#include <sys/mman.h> // PROT_READ
#include <mqueue.h>  // Message Queue
#include <string.h>  // memset

#define MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP

int main(int argc, char **argv)
{
   int     flags  = O_RDWR;
   int     iRet   = -1;
   int     fd     =  0;
   char   *ptr    =  NULL;
   struct stat stat = {0};
   int     i      =  0;

   // Create a Msg Queue
   printf("%s: Opening shared mem  %s\n", __FILE__, argv[1]);
   fd = shm_open(argv[1], flags, MODE);
   if(fd < 0)
   {
       perror("shm_open failed..");
   }

   iRet = fstat(fd, &stat);
   if(iRet < 0)
   {
       perror("fstat failed..");
   }

   ptr = mmap(NULL, stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   printf("%s: mmap value - %p\n", __FILE__, ptr);
   if((int)ptr == -1)
   {
       perror("mmap failed..");
   }  
   close(fd);

   // Write some value in the shared memory
   memset(ptr, 0x00, stat.st_size);
   for(i = 0; i < stat.st_size; i++)
   {
       ptr[i] = i;
   }
}
