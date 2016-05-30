
// LogManager.h

#ifndef EX4_LOGMANAGER_H
#define EX4_LOGMANAGER_H

#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <fuse.h>

/**
 * @brief A struct to maintain the state of the system.
 * Important but not documented: MUST initialize in the init function in order
 * to use this struct during the filesystem operation.
 *
 * @ logFile - A pointer to the log file of the file system.
 * @ rootDir - A pointer to the name of the root directory.
 */
struct cfs_state {
    char *logfile_full_path;
    FILE *logfile;
    char *rootdir;
};
#define CF_LOG ((struct cfs_state *) fuse_get_context()->private_data)

/**
 * @brief Open the log file to write the operations done
 */
FILE *open_log(char *logfile);

/**
 * @brief A function to log the system calls done by the filesystem
 */
void log_call(const char *callName);

#endif //EX4_LOGMANAGER_H
