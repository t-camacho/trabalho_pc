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

#ifndef BENCHMARK_HPP__
#define BENCHMARK_HPP__

#include <iostream>
#include <cstdlib>
#include <string>

inline void argError(std::string reason) {
    std::cerr << "Argument Error: " << reason << std::endl;
    exit(-1);
}

struct BenchmarkConfig {
    int datasetsize;                    // number of items
    int threads;
	int chunkSize;						// number of items which will be changed by each thread
    int verbosity;                      // in {0, 1, 2}, lower => less output
    std::string bm_name;
	
    BenchmarkConfig()
        : datasetsize(256), threads(2), chunkSize(datasetsize/threads),
          verbosity(1), bm_name("RWPathology")
    { }

    void verifyParameters();
};

// BMCONFIG is declared in BenchMain.cpp
extern BenchmarkConfig BMCONFIG;
// barrier is initialized in BenchMain.cpp
extern pthread_barrier_t barrier;

class Benchmark;
struct thread_args_t {
    int        id;
    Benchmark* b;
};

// all benchmarks have the same interface
class Benchmark {
  public:
    virtual void do_work(thread_args_t* args) = 0;
    void run();
    virtual ~Benchmark() { }
};

#endif // BENCHMARK_HPP__
