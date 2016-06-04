// LogManager.cpp

#include "logManager.h"

FILE *open_log(char *logfile) {
    FILE *logFile;

    logFile = fopen(logfile, "a");
    std::cout << "creating log file: " << logfile << std::endl; // todo remove
    if (logFile == nullptr){
        perror("Logfile not created"); // todo exit?
    }

    return logFile;
}

void log_call(const char *callName) {
    time_t callTime;
    time(&callTime);

    fprintf((CF_LOG->logfile), "%li %s\n", callTime, callName);
}

void ioctl_log(const char *cached_files) {
    fprintf((CF_LOG->logfile), "%s\n", cached_files);
}
