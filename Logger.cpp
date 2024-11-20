#include "Logger.h"

using namespace std;

Logger::Logger(const string &filename) {
    logFile.open(filename, ios::out | ios::app);
    if (!logFile) {
        throw runtime_error("Unable to open log file");
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const string &message) {
    lock_guard<mutex> lock(logMutex);
    logFile << message << endl;
}