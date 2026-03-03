#include "../headers/server.h"



void handleCtrlC(int sig) {
    printf("Ctrl+C received.\n");

                                                // Set the stop flag to stop workers
     stopFlag = 1;
     pthread_cancel(master);                       // Kill master thread
    pthread_cond_signal(&(bufferNotEmpty));         // Signal stop workers to proceed
    printf("Master thread have been terminated.\n");


}