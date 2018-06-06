#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

// barrier is initialized in BenchMain.cpp
extern pthread_barrier_t barrier;

// the spinlock used to sycronize threads
extern pthread_spinlock_t spinlock;
#endif

