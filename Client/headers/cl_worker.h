#ifndef CL_WORKER_H
#define CL_WORKER_H


#include <stdio.h>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */
#include <pthread.h>

#define MESS_SIZE 100

typedef struct sh{
    char **argv;
}Shared;

extern Shared *shared;
void *worker(void *);

#endif