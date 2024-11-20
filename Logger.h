#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <string>

using namespace std;

class Logger {
    ofstream logFile;
    mutex logMutex;

public:
    explicit Logger(const string &filename);
    ~Logger();

    void log(const string &message);
};

#endif // LOGGER_H
