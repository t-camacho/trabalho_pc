#include "util.h"

void usage(version v, const char *name) {
    std::cout << "Usage: ./build/apps/main [OPTIONS]...\n\n";
    std::cout << "Options:\n";
    std::cout << "\t -i The input to use to run the benchmarks. Default: 'test'.\n";
    std::cout << "\t\t - simsmall\n";
    std::cout << "\t\t - simdev\n";
    std::cout << "\t\t - simlarge\n";
    std::cout << "\t\t - simmedium\n";
    std::cout << "\t\t - native\n";
    std::cout << "\t -n The number of threads to use. Default: '2'.\n";
    std::cout << "\t -h Displays this help message.\n";
    exit(EXIT_SUCCESS);
}

void fatal(const char *str) {
    if(str != nullptr) {
        std::cout << "fatal: " << str << std::endl;
    }
    exit(1);
}