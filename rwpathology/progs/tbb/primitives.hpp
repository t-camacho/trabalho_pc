#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include <tbb/mutex.h>

// barrier is initialized in BenchMain.cpp
extern pthread_barrier_t barrier;

// the mutex used to sycronize threads
extern tbb::mutex mutex;
#endif

