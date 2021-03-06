// LogManager.cpp

#include "logManager.h"

FILE *open_log(char *logfile) {
    FILE *logFile;

    logFile = fopen(logfile, "a");
    if (logFile == nullptr){
        perror("System Error: logfile was not created");
        exit(EXIT_FAILURE);
    }

    return logFile;
}

void log_call(const char *callName) {
    time_t callTime;
    time(&callTime);

    fprintf((CF_LOG->logfile), "%li %s\n", callTime, callName);
    fflush(CF_LOG->logfile);
}

void ioctl_log(const char *cached_files) {
    fprintf((CF_LOG->logfile), "%s", cached_files);
    fflush(CF_LOG->logfile);
}
