#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include <semaphore.h>
#include <pthread.h>

// the shared memory
struct SHM {	
	// barrier is initialized in BenchMain.cpp
	pthread_barrier_t barrier;

	// the semaphore used to sycronize threads
	sem_t sem;
};
// the pointer to shared memory
extern SHM* shm;

// the attribute to make barrier be shared between processes
extern pthread_barrierattr_t  barrier_attr;

extern key_t list_key;
extern key_t shm_key;
#endif

