// LogManager.h

#ifndef EX4_LOGMANAGER_H
#define EX4_LOGMANAGER_H

//#include "CachingFileSystem.h"
#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdio.h>

/**
 * @brief A struct to maintain the state of the system
 */
struct cfs_state {
    FILE *logfile;
    char *rootdir;
};
#define CF_LOG ((struct cfs_state *) fuse_get_context()->private_data)

//struct bb_state {
//    FILE *logfile;
//    char *rootdir;
//};
//#define BB_DATA ((struct bb_state *) fuse_get_context()->private_data) // todo remove

/**
 * @brief Open the log file to write the operations done
 */
FILE *open_log(char *logfile);

/**
 * @brief A function to log the system calls done by the filesystem
 */
void log_call(const char *callName, ...);

//class LogManager {
//public:
//
//};


#endif //EX4_LOGMANAGER_H
