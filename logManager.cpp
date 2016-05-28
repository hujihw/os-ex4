// LogManager.cpp

#include "logManager.h"
#include <time.h>
#include <fuse.h>

FILE *open_log(char *logfile) {
    FILE *logFile;


    logFile = fopen(logfile, "w");
    std::cout << "creating log file: " << logfile << std::endl;
    if (logFile == nullptr){
        perror("Logfile not created");
        // todo exit?
    }

    return logFile;
}

void log_call(const char *callName) {
    time_t callTime;
    time(&callTime);
    fprintf((CF_LOG->logfile), "%li %s", callTime, callName);
}
