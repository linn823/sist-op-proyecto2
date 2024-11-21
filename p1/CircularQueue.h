#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <vector>
#include "Monitor.h"
#include "Logger.h"

using namespace std;

class CircularQueue {
    vector<int> queue;
    size_t head, tail, size, capacity;
    Monitor &monitor;
    Logger &logger;

    void resize(bool expand);

public:
    CircularQueue(size_t initCapacity, Monitor &mon, Logger &log);
    void enqueue(int value);
    int dequeue();
    bool isEmpty() const;
    size_t getSize() const;
};

#endif // CIRCULAR_QUEUE_H
