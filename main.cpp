#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "Monitor.h"
#include "CircularQueue.h"

using namespace std;

ofstream logFile;
mutex logMutex;

void iniciarLog(const string &filename) {
    logFile.open(filename, ios::out | ios::app);
    if (!logFile) {
        cerr << "Error: Unable to open log file" << endl;
        exit(EXIT_FAILURE);
    }
}

void log(const string &message) {
    lock_guard<mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile << message << endl;
    }
}

void cerrarLog() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void parseArguments(int argc, char *argv[], int &producers, int &consumers, int &initQueueSize, int &maxWaitTime) {
    int opt;
    while ((opt = getopt(argc, argv, "p:c:s:t:")) != -1) {
        switch (opt) {
            case 'p': producers = stoi(optarg); break;
            case 'c': consumers = stoi(optarg); break;
            case 's': initQueueSize = stoi(optarg); break;
            case 't': maxWaitTime = stoi(optarg); break;
            default: cerr << "Error en los argumentos\n"; exit(1);
        }
    }
}

void producerFunction(CircularQueue &queue, int id) {
    for (int i = 0; i < 10; ++i) {
        queue.enqueue(i);
        log("Productor " + to_string(id) + " añadio " + to_string(i));
    }
}

void consumerFunction(CircularQueue &queue, int id, int maxWaitTime) {
    while (true) {
        try {
            int value = queue.dequeue();
            log("Consumidor " + to_string(id) + " quito " + to_string(value));
        } catch (const runtime_error &) {
            
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    int producers = 0, consumers = 0, initQueueSize = 0, maxWaitTime = 0;
    parseArguments(argc, argv, producers, consumers, initQueueSize, maxWaitTime);
    
    Monitor monitor;
    CircularQueue queue(initQueueSize, monitor);
    iniciarLog("queue_log.txt");

    vector<thread> producerThreads, consumerThreads;

    for (int i = 0; i < producers; ++i) {
        producerThreads.emplace_back(producerFunction, ref(queue), i);
    }

    for (int i = 0; i < consumers; ++i) {
        consumerThreads.emplace_back(consumerFunction, ref(queue), i, maxWaitTime);
    }

    for (size_t i = 0; i < producerThreads.size(); ++i) {
        producerThreads[i].join();
    }

    for (size_t i = 0; i < consumerThreads.size(); ++i) {
        consumerThreads[i].join();
    }

    cerrarLog();

    return 0;
}