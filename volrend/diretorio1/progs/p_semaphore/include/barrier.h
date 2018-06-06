#pragma once

#include <iostream>

class Barrier {
private:
    int value;
    pthread_mutex_t v_mutex;
    pthread_cond_t cond_var;
    int size;
    int lockAndAcrement();
    void unlock();
    int get();
    void set(int n);
    void broadcast();
    int wait();
public:
    Barrier(int n);
    void await();
};
