#include <stdio.h>
#include <sys/mman.h>

#define MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP

int main(int argc, char **argv)
{
   int     iRet   = -1;

   // Create a Msg Queue
   printf("%s: Unlinking shared mem  %s\n", __FILE__, argv[1]);
   iRet = shm_unlink(argv[1]);
   if(iRet < 0)
   {
       perror("shm_open failed..");
   }
}
