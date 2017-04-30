#include <stdio.h>
#include <mqueue.h>  // Message Queue

#define MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP

int main(int argc, char **argv)
{
   int iRet = 0;

   // Create a Msg Queue
   printf("%s: Unlinking Q %s\n", __FILE__, argv[1]);
   iRet = mq_unlink(argv[1]);
   if(iRet < 0)
   {
       perror("Unlinking failed..");
   }
}
