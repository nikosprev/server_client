
#include "../headers/cl_worker.h"



void *worker(void *arg){
    int port,sock;
    char **argv=shared->argv;                                           // Get the argv
    char *line=(char *)arg;
     char *first_name,*last_name,*vote;
     char *name,message[MESS_SIZE];




     first_name=strtok_r(line," ",&line);                           // Tokenise
     last_name=strtok_r(line," ",&line);
     vote=strtok_r(line," ",&line);


     name=malloc(strlen(first_name)+strlen(last_name)+2);



     sprintf(name,"%s %s",first_name,last_name);



    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;                 // Setup socket
    struct hostent *rem;

                                                                    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	perror("socket");
        exit(-1);
        }
	                                                                    //Find server address
    if ((rem = gethostbyname(argv[1])) == NULL) {
	   herror("gethostbyname"); exit(1);
    }
    port = atoi(argv[2]);                                                   //Convert port number to integer
    server.sin_family = AF_INET;                                            // Internet domain
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);
    if (connect(sock, serverptr, sizeof(server)) < 0){
	   perror("connect");
        exit(-1);
    }
    printf("Connecting to %s port %d\n", argv[1], port);


      ssize_t num_bytes = recv(sock, message, MESS_SIZE, 0);          // Recieve first message
        if (num_bytes < 0) {
            perror("Failed to send message");
            exit(-1);
         }


    num_bytes = send(sock, name, MESS_SIZE, 0);          // Send name
        if (num_bytes < 0) {
            perror("Failed to send message");
            exit(-1);
         }


   num_bytes = recv(sock, message, MESS_SIZE, 0);          // Receive response
        if (num_bytes < 0) {
            perror("Failed to send message");
            exit(-1);
         }


     num_bytes = send(sock, vote, MESS_SIZE, 0);          // Send vote
        if (num_bytes < 0) {
            perror("Failed to send message");
            exit(-1);
         }



     num_bytes = recv(sock, message, MESS_SIZE, 0);          // Receive confirmation
        if (num_bytes < 0) {
            perror("Failed to send message");
            exit(-1);
         }
         printf("Server: %s\n",message);
        fprintf(stderr,"Thread %lu : Finished\n ",pthread_self());                          // Exit
    free(name);
   pthread_exit(NULL);




}