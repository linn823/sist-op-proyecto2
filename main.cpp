#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include "Monitor.h"
#include "CircularQueue.h"
#include "Logger.h"

using namespace std;

ofstream logFile;
mutex logMutex;
Logger logger("queue_log.txt");

void parseArguments(int argc, char *argv[], int &producers, int &consumers, int &initQueueSize, int &maxWaitTime) {
    int i = 1;
    while (i < argc) {
        string arg = argv[i];
        if (arg == "-p" && i + 1 < argc) {
            producers = stoi(argv[++i]);
        } else if (arg == "-c" && i + 1 < argc) {
            consumers = stoi(argv[++i]);
        } else if (arg == "-s" && i + 1 < argc) {
            initQueueSize = stoi(argv[++i]);
        } else if (arg == "-t" && i + 1 < argc) {
            maxWaitTime = stoi(argv[++i]);
        } else {
            printf("Error en los argumentos\n");
            exit(1);
        }
        i++;
    }
}

void producerFunction(CircularQueue &queue, int id) {
    for (int i = 0; i < 5; ++i) { // produce 5 elementos
        queue.enqueue(i);
        logger.log("Productor " + to_string(id+1) + " añadio " + to_string(i + (id+1)*1000));
    }
}

void consumerFunction(CircularQueue &queue, int id, int maxWaitTime) {
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    while (true) {
        int value = queue.dequeue();
        if(value != -1){
            logger.log("Consumidor " + to_string(id+1) + " quito " + to_string(value));
            startTime = std::chrono::steady_clock::now();
        }
        else{ // si el valor es -1 entonces no saco nada de la cola
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            int tiempo_pasado = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

            if (tiempo_pasado >= maxWaitTime) {
                log("El consumidor " + to_string(id+1) + " espero " + to_string(maxWaitTime) + " segundos y termino.");
                break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int producers = 0, consumers = 0, initQueueSize = 0, maxWaitTime = 0;
    parseArguments(argc, argv, producers, consumers, initQueueSize, maxWaitTime);
    
    Monitor monitor;
    CircularQueue queue(initQueueSize, monitor, logger);

    vector<thread> producerThreads, consumerThreads;

    for (int i = 0; i < consumers; i++) {
        consumerThreads.emplace_back(consumerFunction, ref(queue), i, maxWaitTime);
    }

    for (int i = 0; i < producers; i++) {
        producerThreads.emplace_back(producerFunction, ref(queue), i);
    }

    for (size_t i = 0; i < consumerThreads.size(); i++) {
        consumerThreads[i].join();
    }

    for (size_t i = 0; i < producerThreads.size(); i++) {
        producerThreads[i].join();
    }

    return 0;
}