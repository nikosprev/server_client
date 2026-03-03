

#include "../headers/server.h"




int main(int argc,char ** argv){
    int i;

    Shared *shared=malloc(sizeof(Shared));

    shared->buffer=malloc(sizeof(int)*atoi(argv[3]));
    for(i=0; i<atoi(argv[3]); i++){
        shared->buffer[i]=-1;
    }
                                                          // Setup shared
    shared->count=0;
    shared->port=atoi(argv[1]);
    shared->thread_num=atoi(argv[2]);
    shared->isAvailable=1;
    shared->buff_size=atoi(argv[3]);
    shared->atleast=0;

    pthread_mutex_init(&(shared->mutex), NULL);                     // Initialise muteces
    pthread_mutex_init(&(shared->fileMutex), NULL);
    pthread_cond_init(&(shared->fileAvailable), NULL);
    pthread_cond_init(&(shared->bufferNotFull), NULL);
    pthread_cond_init(&(bufferNotEmpty), NULL);

    shared->hashtable=malloc(sizeof(HashTable));            // Create hash table
    for(i=0; i<BUCK; i++){
        shared->hashtable->buckets[i]=NULL;
    }

    shared->log=strdup(argv[4]);
    shared->stats=strdup(argv[5]);                     // Allocate memory and copy from argv


     stopFlag=0;







     if (pthread_create(&master, NULL, Master, shared)) {            // Make master thread
            perror("Failed to create master thread.");
            exit(EXIT_FAILURE);
        }




    if (pthread_join(master, NULL) != 0) {                // Wait for master thread
              perror("Failed to join master thread.");
            exit(EXIT_FAILURE);
        }

    if(shared->atleast)
    makeStats(argv[5],argv[4]);
    else printf("No connections were made\n");



                                                 // Cleanup
    free(shared->log);
    free(shared->stats);
    free(shared->buffer);
    pthread_mutex_destroy(&(shared->mutex));
    pthread_mutex_destroy(&(shared->fileMutex));
    pthread_cond_destroy(&(shared->fileAvailable));
    pthread_cond_destroy(&(shared->bufferNotFull));
    pthread_cond_destroy(&(bufferNotEmpty));
    destroyHashTable(shared->hashtable);

    free(shared);

    return 0;
}