#ifndef MONITOR_H
#define MONITOR_H

#include <pthread.h>
#include <stdexcept>

using namespace std;

class Monitor {
    pthread_mutex_t mutex;
    pthread_cond_t cond;

public:
    Monitor();
    ~Monitor();

    void lock();
    void unlock();
    void wait();
    void notify();
};

#endif // MONITOR_H
