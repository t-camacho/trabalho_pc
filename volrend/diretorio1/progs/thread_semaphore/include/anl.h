#pragma once
#include "pthread.h"
#include "const.h"
#include "semaphore.h"

#define PAD 256

struct GlobalMemory {
    long Index, Counter;
    long Queue[MAX_NUMPROC+1][PAD];
    pthread_barrier_t SlaveBarrier;
    pthread_barrier_t TimeBarrier;
    sem_t  IndexLock;
    sem_t  CountLock;
    sem_t  QLock[MAX_NUMPROC+1];
};
