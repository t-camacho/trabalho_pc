#include "barrier.h"

Barrier::Barrier(int n) {
    pthread_cond_init(&cond_var, NULL);
    pthread_mutex_init(&v_mutex, NULL);
    this->value = 0;
    this->size = n;
}

void Barrier::await() {
    int position = lockAndAcrement();
    if(position == this->size) {
        set(0);
        broadcast();
    }else {
        while(wait());
    }
    unlock();
}

int Barrier::lockAndAcrement() {
    pthread_mutex_lock(&v_mutex);
    this->value++;
    return this->value;
}

void Barrier::unlock() {
    pthread_mutex_unlock(&v_mutex);
}

int Barrier::get() {
    int buf;
    pthread_mutex_lock(&v_mutex);
    buf = this->value;
    pthread_mutex_unlock(&v_mutex);
    return buf;
}

void Barrier::set(int n) {
    this->value = n;
}

void Barrier::broadcast() {
    pthread_cond_broadcast(&cond_var);
}

int Barrier::wait() {
    return pthread_cond_wait(&cond_var, &v_mutex);
}
