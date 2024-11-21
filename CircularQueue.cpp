#include "CircularQueue.h"

using namespace std;

CircularQueue::CircularQueue(size_t initCapacity, Monitor &mon) : monitor(mon) {
    queue = std::vector<int>(initCapacity);
    head = 0;
    tail = 0;
    size = 0;
    capacity = initCapacity;
}

void CircularQueue::resize(bool expand) { // deberia llamarse a resize solo si el monitor esta lock (desde enqueue/dequeue)
    size_t newCapacity = expand ? capacity * 2 : capacity / 2;
    vector<int> newQueue(newCapacity);

    for (size_t i = 0; i < size; ++i) {
        newQueue[i] = queue[(head + i) % capacity];
    }

    queue = std::move(newQueue);
    head = 0;
    tail = size;
    capacity = newCapacity;
}

void CircularQueue::enqueue(int value) {
    monitor.lock();
    if (size == capacity) {
        resize(true); // si ya se alcanzo el limite de capacidad, agrandar
    }
    queue[tail] = value;
    tail = (tail + 1) % capacity;
    size++;
    monitor.notify(); // notifica que hay almenos un elemento en la cola por si alguien espera a sacar
    monitor.unlock();
}

int CircularQueue::dequeue() {
    monitor.lock();
    while (size == 0) { // si se quiere sacar algo de la cola y esta vacia, se quedara esperando
        monitor.wait();
    }
    int value = queue[head];
    head = (head + 1) % capacity;
    size--;
    if (size < capacity / 4 && capacity > 1) {
        resize(false); // si el tamaño alcanzo 1/4 de la capacidad, achicar
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
