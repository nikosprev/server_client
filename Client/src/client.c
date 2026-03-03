
#include "../headers/cl_worker.h"

 Shared *shared;


int main(int argc, char *argv[]) {
    int port, sock, i;
    char **lines,line[2*MESS_SIZE];
    int count=0;
    pthread_t *workers;


    shared=malloc(sizeof(Shared));                // copy argv in shared
    shared->argv=malloc(argc*sizeof(char *));
    for(i=0; i<argc; i++) shared->argv[i]=strdup(argv[i]);



    if (argc != 4) {
    	printf("Please give host name and port number\n");
       	exit(1);}



    FILE *file=fopen(argv[3],"r");                      // Open file
    if(file==NULL){
      printf("File not found\n");
      exit(1);
    }
    lines=malloc(sizeof(char *));

    while(fgets(line,sizeof(line),file)){           // Copy lines
      if(count==0){
        lines[count]=strdup(line);

      }
      else{

        lines=realloc(lines,(count+1)*sizeof(char *));
        lines[count]=strdup(line);
    }
    count++;
}

    workers=malloc(count*sizeof(pthread_t));                  // Create workers

    printf("%d threads to be created.\n ",count);
    for(i=0; i<count; i++){

     if (pthread_create(&workers[i], NULL, worker, lines[i])) {
            perror("Failed to create  thread.");
            exit(EXIT_FAILURE);
        }


    }

    for(i=0; i<count; i++){
    if (pthread_join(workers[i], NULL) != 0) {                // Wait thread to finish
              perror("Failed to join master thread.");
            exit(EXIT_FAILURE);
        }



    }
     printf("All threads have finished\n");


    for(i=0; i<argc; i++){                      // Cleanup
      free(shared->argv[i]);
    }
    free(shared->argv);
    free(shared);

    for(i=0; i<count; i++) free(lines[i]);
    free(lines);


    close(sock);                 // Close socket and exit
    return 0;
}

