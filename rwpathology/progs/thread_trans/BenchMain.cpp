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
#include <cstdlib>
#include <string>
#include <iostream>
#include <pthread.h>

#include "Benchmark.hpp"
#include "RWPathology.hpp"

using namespace bench;
using std::cerr;
using std::cout;
using std::endl;
using std::string;

BenchmarkConfig BMCONFIG;
pthread_barrier_t barrier;

static void usage() {
    cerr << "Usage: prog_name [flags]" << endl << endl;
    cerr << "  Flags:" << endl;
    cerr << "    -m: number of distinct elements (default 256)" << endl;
    cerr << "    -p: number of threads (default 2)" << endl;
	cerr << "    -c: number of elements which each thread will change (default m/p)" << endl;
    cerr << "    -h: print help (this message)" << endl;
    cerr << "    -q: quiet mode" << endl;
    cerr << "    -v: verbose mode" << endl;
    cerr << endl;
}

int main(int argc, char** argv) {
    int opt;

    // parse the command-line options
    while ((opt = getopt(argc, argv, "m:p:c:hqv")) != -1) {
        switch(opt) {
          case 'm':
            BMCONFIG.datasetsize = atoi(optarg);
            break;
          case 'p':
            BMCONFIG.threads = atoi(optarg);
            break;
		case 'c':
            BMCONFIG.chunkSize = atoi(optarg);
            break;
          case 'h':
            usage();
            return 0;
          case 'q':
            BMCONFIG.verbosity = 0;
            break;
          case 'v':
            BMCONFIG.verbosity = 2;
            break;
        }
    }

    // make sure that the parameters all make sense
    BMCONFIG.verifyParameters();
	
	// initialize the barrier which will be used to run the benchmark
	pthread_barrier_init(&barrier, NULL, BMCONFIG.threads);
	
    RWPathology *B = new RWPathology(BMCONFIG.datasetsize, BMCONFIG.chunkSize);
    B->run();
	B->list.print();
}
