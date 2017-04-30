#include <stdio.h>
#include <mqueue.h>  // Message Queue

#define MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP

int main(int argc, char **argv)
{
   mqd_t   mqd    = {0};
   int     flags  = O_RDWR | O_CREAT;
   int     iRet   = -1;
   struct  mq_attr attr   = {0};

   //Set Queue Properties
   attr.mq_maxmsg  =  10;
   attr.mq_msgsize =  10;

   // Create a Msg Queue
   printf("%s: Creating Q %s\n", __FILE__, argv[1]);
   mqd = mq_open(argv[1], flags, MODE, &attr);
   if(mqd < 0)
   {
       perror("mq_open failed..");
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
