#include <iostream>
#include <thread>
#include <vector>
#include "Monitor.h"
#include "CircularQueue.h"
#include "Logger.h"

using namespace std;

void parseArguments(int argc, char *argv[], int &producers, int &consumers, int &initQueueSize, int &maxWaitTime) {
    int opt;
    while ((opt = getopt(argc, argv, "p:c:s:t:")) != -1) {
        switch (opt) {
            case 'p': producers = stoi(optarg); break;
            case 'c': consumers = stoi(optarg); break;
            case 's': initQueueSize = stoi(optarg); break;
            case 't': maxWaitTime = stoi(optarg); break;
            default: cerr << "Invalid arguments!\n"; exit(EXIT_FAILURE);
        }
    }
}

void producerFunction(CircularQueue &queue, int id, Logger &logger) {
    for (int i = 0; i < 10; ++i) {
        queue.enqueue(i);
        logger.log("Producer " + to_string(id) + " added " + to_string(i));
    }
}

void consumerFunction(CircularQueue &queue, int id, Logger &logger, int maxWaitTime) {
    while (true) {
        try {
            int value = queue.dequeue();
            logger.log("Consumer " + to_string(id) + " removed " + to_string(value));
        } catch (const runtime_error &) {
            // Handle max wait time if needed
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    // Parse command-line arguments
    int producers = 0, consumers = 0, initQueueSize = 0, maxWaitTime = 0;
    parseArguments(argc, argv, producers, consumers, initQueueSize, maxWaitTime);
    
    Monitor monitor;
    CircularQueue queue(initQueueSize, monitor);
    Logger logger("queue_log.txt");

    vector<thread> producerThreads, consumerThreads;

    for (int i = 0; i < producers; ++i) {
        producerThreads.emplace_back(producerFunction, ref(queue), i, ref(logger));
    }

    for (int i = 0; i < consumers; ++i) {
        consumerThreads.emplace_back(consumerFunction, ref(queue), i, ref(logger), maxWaitTime);
    }

    for (auto &t : producerThreads) t.join();
    for (auto &t : consumerThreads) t.join();

    return 0;
}