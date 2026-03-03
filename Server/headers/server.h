#ifndef SERVER_H
#define SERVER_H


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 #include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
 #include <fcntl.h>



#include "hash.h"
#include "sig.h"
#include "stats.h"
#include "globals.h"


 #define MESS_SIZE 100                // Size of message

void handleCtrlC(int sig);



typedef struct shared{
    int *buffer;                             // Buffer for connections
    int count;                                // Num of connections in buffer
    int isAvailable;                           // Flag for availability of log file
    pthread_mutex_t fileMutex;                  // Mutex for access to the log file
     pthread_cond_t fileAvailable;               // Condition variable for access to the log file
    pthread_cond_t bufferNotFull;                   // Condition variable to signify that the buffer is not full
    pthread_mutex_t mutex;                         // Mutex for access to the buffer
    char * log;                               // Name of log file
    char * stats;                               // Name of stats file
    HashTable *hashtable;                         // Hash table to lookup names
    int port;
    int thread_num;
    int buff_size;
    int atleast;                                            // Flag to see if the server had at least one connection before exiting , in order to skip making stats.

} Shared;


void Remove(int *,int *,int );
void *Master(void *);
void *Worker(void *);
#endif