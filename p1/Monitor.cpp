#include "Monitor.h"

using namespace std;

Monitor::Monitor() {
    if (pthread_mutex_init(&mutex, nullptr) != 0) {
        throw runtime_error("error al iniciar mutex");
    }
    
    if (pthread_cond_init(&cond, nullptr) != 0) {
        throw runtime_error("error al iniciar variable de condicion");
    }
}

Monitor::~Monitor() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void Monitor::lock() {
    pthread_mutex_lock(&mutex);
}

void Monitor::unlock() {
    pthread_mutex_unlock(&mutex);
}

void Monitor::wait() {
    pthread_cond_wait(&cond, &mutex);
}

void Monitor::notify() {
    pthread_cond_signal(&cond);
}