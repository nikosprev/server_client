
#include "../headers/server.h"


 sig_atomic_t stopFlag;                      // Definitions -- Declarations are in globals.h
 pthread_t master;
 pthread_cond_t bufferNotEmpty;

void *Master(void *arg){

    Shared *shared=(Shared *)(arg);           // Get the shared struct
    int i;

    pthread_t threads[shared->thread_num];                              // Worker threads


      signal(SIGINT, handleCtrlC);                  // Handle ctrl c

     int sock, newsock;                                          // Socket setup
    struct sockaddr_in server, client;
    socklen_t clientlen;



    struct sockaddr *serverptr=(struct sockaddr *)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;

    socklen_t addrlen = sizeof(clientptr);
    memset(&clientptr, 0, sizeof(clientptr));

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {           // Using TCP
        perror("Failed creating a socket.");
        exit(-1);

}
    server.sin_family = AF_INET;       // Internet domain
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(shared->port);      // The given port

                                                    //Bind socket to address
    if (bind(sock, serverptr, sizeof(server)) < 0){
        perror("Failed to bind socket.");
        exit(-1);

    }
                                               // Listen for connections
    if (listen(sock, 100) < 0){
        perror("Failed to use listen()");
        exit(-1);

    }
    printf("Listening for connections to port %d\n", shared->port);


     pthread_mutex_lock(&(shared->mutex));                                  // Get mutex so that it is not locked by a worker thread before connections are accepted

    for (i = 0; i < shared->thread_num; i++) {                                      // Create worker threads

        if (pthread_create(&threads[i], NULL, Worker, shared)) {
            perror("Failed to create a thread.");
            exit(EXIT_FAILURE);
        }
    }



      while (!stopFlag) {                                                   // If ctrl+c is pressed after the accept call , the master thread will exit after the itereation of the loop

        if ((newsock = accept(sock, clientptr, &clientlen)) < 0){               // If ctrl+c is pressed while waiting for connections the thread has to be killed
             perror("Failed to use accept()");
            exit(-1);

        }
        shared->atleast=1;
        printf("Accepted a connection \n");

       while(shared->count>=shared->buff_size){

        pthread_cond_wait(&(shared->bufferNotFull), &(shared->mutex));      // Wait if buffer is full
        if(stopFlag){
            close(newsock);
            break;}

        }

        if(stopFlag){
            close(newsock);
             pthread_cond_broadcast(&(bufferNotEmpty));
        pthread_mutex_unlock(&(shared->mutex));
            break;
        }





         shared->buffer[shared->count]=newsock;   // Put descriptor in buffer

	    shared->count++;



        pthread_cond_signal(&(bufferNotEmpty));                         // Signal and unlock
        pthread_mutex_unlock(&(shared->mutex));


    }



                                                    // Wait for all threads to finish
    for (i = 0; i < shared->thread_num; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
              perror("Failed to join thread.");
            exit(EXIT_FAILURE);
        }


    }


     pthread_exit(NULL);

}

