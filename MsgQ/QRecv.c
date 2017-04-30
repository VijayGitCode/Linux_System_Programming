#include <stdio.h>
#include <mqueue.h>  // Message Queue
#include <string.h>  // memset
#include <malloc.h>  // malloc

#define PRIO 60
#define SIZE 10

int main(int argc, char **argv)
{
   mqd_t   mqd    = {0};
   int     flags  = O_RDONLY;
   int     iRet   = -1;
   struct  mq_attr attr   = {0};
   char   *precvbuff = NULL;
   int     prio = 0;

   // Create a Msg Queue
   printf("%s: Opening Q %s\n", __FILE__, argv[1]);
   mqd = mq_open(argv[1], flags);
   if(mqd < 0)
   {
       perror("mq_open failed..");
   }

   iRet = mq_getattr(mqd, &attr);
   if(iRet < 0)
   {
       perror("mq_getattr failed");
   }

   precvbuff = malloc(attr.mq_msgsize);

   iRet = mq_receive(mqd, precvbuff, attr.mq_msgsize, &prio);
   if(iRet < 0)
   {
       perror("mq_send failed..");
   }
   printf("%s: Received Msg is %s\n", __FILE__, precvbuff);

   iRet = mq_getattr(mqd, &attr);
   if(iRet < 0)
   {
       perror("mq_getattr failed");
   }

   // Get Q attributes
   printf("%s: Q attributes:\n", __FILE__);
   printf("%s: Max msgs is %ld, Max msg size is %ld, no. of msgs is %ld\n",\
           __FILE__, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

   printf("%s: Closing Q %s\n", __FILE__, argv[1]);
   iRet = mq_close(mqd);
   if(iRet < 0)
   {
       perror("mq_close failed..");
   }
}
