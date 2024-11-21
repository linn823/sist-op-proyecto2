#include "Logger.h"

using namespace std;

Logger::Logger(const string& logFileName) {
    logFile.open(logFileName, ios::out | ios::trunc);
    if (!logFile) {
        printf("No se pudo abrir archivo log\n");
        exit(1);
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::log(const string& message) {
    logFile << message << endl;
    logFile.flush(); 
}
