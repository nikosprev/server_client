
#include "../headers/server.h"



void Remove(int *buffer,int *count,int index){
    int i=0;
    for(i=index; i<count[0]-1; i++){
        buffer[i]=buffer[i+1];                // Move down elements of the buffer
    }
    buffer[count[0]-1]=-1;
    count[0]--;

    return;

}



void *Worker(void *arg) {
    int i,fd;
    FILE *file;
    Shared *shared=(Shared *)(arg);
    char message[MESS_SIZE];
    memset(message, 0, sizeof(message));  // Initialize the message buffer with zeros
    char *name;
    char result[MESS_SIZE+20],logged[MESS_SIZE+20];
     memset(result, 0, sizeof(result));
      memset(logged, 0, sizeof(logged));

      while (1) {

        pthread_mutex_lock(&(shared->mutex));                           // Get mutex

        while (shared->count == 0) {                                    // Wait for buffer to not be empty
            pthread_cond_wait(&(bufferNotEmpty), &(shared->mutex));
           if(stopFlag){                                                        // If the thread was awaken by handle ctrc it should break and exit
             pthread_mutex_unlock(&(shared->mutex));
            pthread_cond_signal(&(bufferNotEmpty));                               // Signal for the next thread to proceed

            break;
        }
        }
        if(stopFlag){
                                    // If ctrc was pressed but the threads are not blocked in the above loop
             pthread_mutex_unlock(&(shared->mutex));

            break;
        }


        for(i=0; i<shared->count; i++){               // Find earliest valid connection
            if(shared->buffer[i]!=-1) break;
        }

        fd=shared->buffer[i];                                              // Get the connection file descriptor

        Remove(shared->buffer,&(shared->count),i);


                                                   // Signal and unlock so the other thread can enter the buffer

        pthread_cond_signal(&(shared->bufferNotFull));
        pthread_mutex_unlock(&(shared->mutex));


                                                                // Begin communication

        memcpy(message,"SEND NAME PLEASE",MESS_SIZE);


        ssize_t num_bytes = send(fd, message, MESS_SIZE, 0);          // Send first message
        if (num_bytes < 0) {
            perror("Failed to send message");
            exit(-1);
         }





        num_bytes = recv(fd, message, MESS_SIZE, 0);           // Await response - name
        if (num_bytes < 0) {
         perror("Failed to receive message");
            exit(-1);
        }

        name=strdup(message);                                 // Copy name

        int voted=lookup(shared->hashtable,name);


        if(voted) memcpy(message,"ALREADY VOTED",MESS_SIZE);

        else memcpy(message,"SEND VOTE PLEASE",MESS_SIZE);



        num_bytes = send(fd, message, MESS_SIZE, 0);           // Send next message
        if (num_bytes < 0) {
         perror("Failed to send message");
            exit(-1);
        }
        if(voted){
            close(fd);          // Already voted
            continue;      // Go to next itereation
        }

        num_bytes = recv(fd, message, MESS_SIZE, 0);           // Await vote
        if (num_bytes < 0) {
         perror("Failed to receive message");
            exit(-1);
        }

        sprintf(result,"VOTE FOR %s RECORDED",message);           // Prepare confirmation message

        num_bytes = send(fd, result , MESS_SIZE+20, 0);           // Send confirmation
        if (num_bytes < 0) {
         perror("Failed to send message");
            exit(-1);
        }
        close(fd);



                                                                  // Communication finished - Start writing in the file

        pthread_mutex_lock(&(shared->fileMutex));                   // Mutex for the log file


            while (!(shared->isAvailable)) {                                // Condition variable

                     pthread_cond_wait(&(shared->fileAvailable), &(shared->fileMutex));
              }
            shared->isAvailable=0;


            sprintf(logged,"%s %s",name,message);            // Prepare log entry

            file=fopen(shared->log,"a+");                        // Open log file

            if (file<0){
                perror("Failed to open log file\n");

            }

            if (fprintf(file,"%s", logged) < 0){                    // Write to log file
                perror("Writing error\n");
            }

            fclose(file);
                shared->isAvailable = 1;                                 // Set available


            pthread_cond_signal(&(shared->fileAvailable));                    // Signal and unlock so the other threads can get access to the file


            pthread_mutex_unlock(&(shared->fileMutex));


            printf("Submitted a vote.\n");



           free(name);

    }

      fprintf(stderr,"Thread %lu: Exiting \n ",pthread_self());
    pthread_exit(NULL);
}



