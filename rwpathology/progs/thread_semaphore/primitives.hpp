#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include <semaphore.h>

// barrier is initialized in BenchMain.cpp
extern pthread_barrier_t barrier;

// the semaphore used to sycronize threads
extern sem_t sem;
#endif

