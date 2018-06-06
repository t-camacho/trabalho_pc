#pragma once
#include "pthread.h"
#include "const.h"

#define PAD 256

struct GlobalMemory {
    long Index, Counter;
    long Queue[MAX_NUMPROC+1][PAD];
    pthread_barrier_t SlaveBarrier;
    pthread_barrier_t TimeBarrier;
    pthread_mutex_t IndexLock;
    pthread_mutex_t CountLock;
    pthread_mutex_t QLock[MAX_NUMPROC+1];
    // ALOCKDEC(QLock,MAX_NUMPROC+1)
};
