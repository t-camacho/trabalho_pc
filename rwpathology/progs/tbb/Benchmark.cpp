///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2005, 2006, 2007, 2008, 2009
// University of Rochester
// Department of Computer Science
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the University of Rochester nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <unistd.h>
#include <pthread.h>

#include <iostream>
#include <cassert>
#include <signal.h>
#include <vector>
#include <pthread.h>
#include <tbb/task_group.h>

#ifdef SPARC
#include <sys/types.h>
#endif

#include "Benchmark.hpp"

using std::cout;
using std::endl;
using namespace tbb;

static void* run_benchmark(void* arg) {
    thread_args_t* args    = (thread_args_t*)arg;
    int            id      = args->id;
    Benchmark* b = args->b;

    // everyone waits here, and then we run the experiment
    pthread_barrier_wait(&barrier);

	b->do_work(args);

}

class Task {
	thread_args_t *args;
	
	public:
	
		Task(thread_args_t *a) : args(a) {}
		void operator() () const {
			run_benchmark((void*) args);
		}
};

void Benchmark::run() {
    std::vector<thread_args_t> args;
    args.resize(BMCONFIG.threads);

    task_group g;

    // set up configuration structs for the threads we'll create
    for (int i = 0; i < BMCONFIG.threads; i++) {
        args[i].id = i;
        args[i].b = this;
    }
	
	// create the threads
	for(int j = 1; j < BMCONFIG.threads; j++) {
		g.run(Task(&args[j]));
	}
	
	// this flow also needs to do some work
	g.run(Task(&args[0]));

    // everyone should be done.  Join all threads so we don't leave anything
    // hanging around
    g.wait();
}

// make sure all the parameters make sense
void BenchmarkConfig::verifyParameters() {
    if (datasetsize <= 0)
        argError("m must be positive");
    if (threads <= 0)
        argError("p must be positive");
	if (chunkSize <= 0)
        argError("c must be positive");
}

