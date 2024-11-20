#include "Monitor.h"

using namespace std;

Monitor::Monitor() {
    if (pthread_mutex_init(&mutex, nullptr) != 0) {
        throw runtime_error("Failed to initialize mutex");
    }
    if (pthread_cond_init(&cond, nullptr) != 0) {
        pthread_mutex_destroy(&mutex);
        throw runtime_error("Failed to initialize condition variable");
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

void Monitor::notifyAll() {
    pthread_cond_broadcast(&cond);
}