#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include <signal.h>

extern sig_atomic_t stopFlag;          // Flag updated when ctrl+c is pressed
extern pthread_t master;                // Master thread
extern pthread_cond_t bufferNotEmpty;       // Conditional variable for when the buffer is not empty


#endif