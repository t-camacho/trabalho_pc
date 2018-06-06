#pragma once
#include "pthread.h"
#include "const.h"
#include "tbb/tbb.h"
#include "barrier.h"

#define PAD 256

struct GlobalMemory {
    long Index, Counter;
    long Queue[MAX_NUMPROC+1][PAD];
    // pthread_barrier_t SlaveBarrier;
    // pthread_barrier_t TimeBarrier;
    Barrier *SlaveBarrier;
    Barrier *TimeBarrier;
    tbb::mutex IndexLock;
    tbb::mutex CountLock;
    tbb::mutex QLock[MAX_NUMPROC+1];
};
