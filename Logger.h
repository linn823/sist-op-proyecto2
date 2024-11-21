#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Logger {
public:
    Logger(const string& logFileName);  // Constructor to open the file
    ~Logger();                             // Destructor to close the file
    void log(const string& message);   // Method to write to the log file

private:
    ofstream logFile;
};

#endif // LOGGER_H
