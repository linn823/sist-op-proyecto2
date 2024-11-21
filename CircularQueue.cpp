#include "CircularQueue.h"

using namespace std;

CircularQueue::CircularQueue(size_t initCapacity, Monitor &mon) : monitor(mon) {
    queue = std::vector<int>(initCapacity);
    head = 0;
    tail = 0;
    size = 0;
    capacity = initCapacity;
}

void CircularQueue::resize(bool expand) {
    monitor.lock();
    size_t newCapacity = expand ? capacity * 2 : capacity / 2;
    vector<int> newQueue(newCapacity);

    for (size_t i = 0; i < size; ++i) {
        newQueue[i] = queue[(head + i) % capacity];
    }

    queue = move(newQueue);
    head = 0;
    tail = size;
    capacity = newCapacity;
    monitor.unlock();
}

void CircularQueue::enqueue(int value) {
    monitor.lock();
    if (size == capacity) {
        resize(true);
    }
    queue[tail] = value;
    tail = (tail + 1) % capacity;
    size++;
    monitor.notify();
    monitor.unlock();
}

int CircularQueue::dequeue() {
    monitor.lock();
    while (size == 0) {
        monitor.wait();
    }
    int value = queue[head];
    head = (head + 1) % capacity;
    size--;
    if (size < capacity / 4 && capacity > 1) {
        resize(false);
    }
    monitor.unlock();
    return value;
}

bool CircularQueue::isEmpty() const {
    return size == 0;
}

size_t CircularQueue::getSize() const {
    return size;
}
