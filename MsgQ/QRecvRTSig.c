#include <stdio.h>
#include <mqueue.h>  // Message Queue
#include <string.h>  // memset
#include <malloc.h>  // malloc
#include <signal.h>  // signal handler

#define PRIO 60
#define SIZE 10

mqd_t   mqd    = {0};
struct sigevent sigev = {0};
volatile int mq_flag;

static void HandleSignal();
static void handler(int signo);

static void handler(int signo)
{
}

int main(int argc, char **argv)
{
   int     flags  = O_RDONLY | O_NONBLOCK;
   int     iRet   = -1;
   struct  mq_attr attr   = {0};
   int     signo  =  0;
   sigset_t blockmask, emptymask;
   struct  sigaction sa = {0};

   // Create a Msg Queue
   printf("%s: Opening Q %s\n", __FILE__, argv[1]);
   mqd = mq_open(argv[1], flags);
   if(mqd < 0)
   {
       perror("mq_open failed..");
   }

   sigemptyset(&blockmask);
   sigaddset(&blockmask, SIGUSR1);
   sigprocmask(SIG_BLOCK, &blockmask, NULL);

   // Register a signal
   sigemptyset(&sa.sa_mask);
   sa.sa_handler = &handler;
   sa.sa_flags = 0;
   sigaction(SIGUSR1, &sa, NULL);

   // Add the signal to mq_notify
   sigev.sigev_notify = SIGEV_SIGNAL;
   sigev.sigev_signo  = SIGUSR1;
   iRet = mq_notify(mqd, &sigev);
   if(iRet < 0)
   {
       perror("mq_notify failed");
   }

   sigemptyset(&emptymask);
   while(1)
   {
       sigsuspend(&emptymask);
       HandleSignal();
   }

   printf("%s: Closing Q %s\n", __FILE__, argv[1]);
   iRet = mq_close(mqd);
   if(iRet < 0)
   {
       perror("mq_close failed..");
   }
}

static void HandleSignal()
{
   int     iRet   = -1;
   struct  mq_attr attr   = {0};
   char   *precvbuff = NULL;
   int     prio = 0;

   iRet = mq_getattr(mqd, &attr);
   if(iRet < 0)
   {
       perror("mq_getattr failed");
   }
   precvbuff = malloc(attr.mq_msgsize);
   
   // Reregister first
   iRet = mq_notify(mqd, &sigev);
   if(iRet < 0)
   {
       perror("mq_notify failed");
   }

   // Receive a Msg
   iRet = mq_receive(mqd, precvbuff, attr.mq_msgsize, &prio);
   if(iRet < 0)
   {
       perror("mq_receive failed..");
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
   return;
}
