/*
 * CachingFileSystem.cpp
 *
 *  Author: Netanel Zakay, HUJI, 67808  (Operating Systems 2015-2016).
 */

#define FUSE_USE_VERSION 26


#include <iostream>
#include <map>
#include <stdio.h>
#include <fuse.h>
#include <unistd.h>
#include <memory.h>
#include <dirent.h>
#include <cmath>
#include "logManager.h"
#include "CacheManager.h"

#define OPEN_FLAGS O_RDONLY | O_DIRECT | O_SYNC
#define SUCCESS 0

using namespace std;
static char logfile_name[PATH_MAX] = ".filesystem.log"; // todo ignore logfile in all functions

static CacheManager *cacheManager;
struct fuse_operations caching_oper;

// -------------------------------------------------------------------------- //
//                todo move this section to CacheManager.cpp                  //
// -------------------------------------------------------------------------- //

static std::map<ino_t, char*> *inotToPath;

/**
 * @brief add an ino_t to the map of paths
 */
void add_ino_t(ino_t ino, char *path)
{
    inotToPath->at(ino) = path;
}

/**
 * @brief remove a pair from the map
 */
void remove_ino_t(ino_t ino)
{
    inotToPath->erase(ino);
}

/**
 * @brief rename a path in the map values
 */
void rename_path(ino_t ino, char *path, char *npath)
{
    // convert parameters to cpp string
    string s_path = path;
    string s_npath = npath;

    // find 'path' in the values
    for (auto iterator = inotToPath->begin(); iterator != inotToPath->end(); iterator ++)
    {
        string p = iterator->second;

        if (p.find(s_path))
        {
            size_t p_size = s_path.size();
            p.replace(0, p_size, s_npath);
        }

        // replace 'path' with 'npath'
        inotToPath->at(ino) = (char *) p.c_str();
    }

}

// -------------------------------------------------------------------------- //

/**
 * @brief Check if trying to refer to the logfile from the filesystem
 */
int refering_logfile(char* fpath)
{
    // compare the given path with the logfile's path
    if (strcmp(fpath, CF_LOG->logfile_full_path) == 0)
    {
        // return no such file error when trying to refer to the logfile
        return -ENOENT;
    }

    return SUCCESS;
}

/**
 * @brief Sets the root dir of the current run
 */
void set_root_dir(char *root_dir, cfs_state *state)
{
    state->rootdir = realpath(root_dir, NULL);
}

/**
 * @brief Get the full path of the file,
 * related to the root of the FUSE filesystem
 */
void caching_full_path(char *absPath, const char *path) {
    strcpy(absPath, CF_LOG->rootdir);
    strncat(absPath, path, PATH_MAX);
}

/** Get file attributes.
 *
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int caching_getattr(const char *path, struct stat *statbuf){
    cout << "   -- getattr --" << endl; // todo remove

    char fpath[PATH_MAX];
    caching_full_path(fpath, path);


    int res = refering_logfile(fpath);

    // return erro if trying to refer to the logfile
    if (res)
    {
        return res;
    }

    // forward the call and log it
    log_call("lstat");
    res = lstat(fpath, statbuf);

    // return errno in case of error
    if (res < 0)
    {
        return -errno;
    }

    return res;
}

/**
 * Get attributes from an open file
 *
 * This method is called instead of the getattr() method if the
 * file information is available.
 *
 * Currently this is only called after the create() method if that
 * is implemented (see above).  Later it may be called for
 * invocations of fstat() too.
 *
 * Introduced in version 2.5
 */
int caching_fgetattr(const char *path, struct stat *statbuf,
                    struct fuse_file_info *fi){ // todo handle logfile
    cout << "-- fgetattr --" << endl;

    char full_path[PATH_MAX];
    caching_full_path(full_path, path);

    log_call("fstat");
    int res = fstat((int) fi->fh, statbuf);

    if (res < 0)
    {
        return -errno;
    }

    return res;
}

/**
 * Check file access permissions
 *
 * This will be called for the access() system call.  If the
 * 'default_permissions' mount option is given, this method is not
 * called.
 *
 * This method is not called under Linux kernel versions 2.4.x
 *
 * Introduced in version 2.5
 */
int caching_access(const char *path, int mask) // todo handle logfile
{
    cout << "    -- access --" << endl; // todo remove
    int res;
    char full_path[PATH_MAX];
    caching_full_path(full_path, path);

    res = refering_logfile(full_path);

    if (res)
    {
        return res;
    }

    log_call("access");
    res = access(full_path, mask);
    return res;
}


/** File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open().  Open should check if the operation
 * is permitted for the given flags.  Optionally open may also
 * initialize an arbitrary filehandle (fh) in the fuse_file_info
 * structure, which will be passed to all file operations.

 * pay attention that the max allowed path is PATH_MAX (in limits.h).
 * if the path is longer, return error.

 * Changed in version 2.2
 */
int caching_open(const char *path, struct fuse_file_info *fi){ // todo handle logfile
    cout << "    -- open -- " << endl;

    int retval = 0;

    char fpath[PATH_MAX];
    caching_full_path(fpath, path);

    // turn on read only todo
    fi->direct_io = 1;

    // todo check if the path is longer than PATH_MAX, return error if it is

    if (((fi->flags & 3) == O_WRONLY) || ((fi->flags & 3) == O_RDWR))
    {
        return -EACCES;
    }

    log_call("open");
    int fd = open(fpath, OPEN_FLAGS);

    if (fd < 0)
    {
        retval = -errno;
    }

    fi->fh = (uint64_t) fd;

    return retval;
}


/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error. For example, if you receive size=100, offest=0,
 * but the size of the file is 10, you will init only the first
   ten bytes in the buff and return the number 10.

   In order to read a file from the disk,
   we strongly advise you to use "pread" rather than "read".
   Pay attention, in pread the offset is valid as long it is
   a multipication of the block size.
   More specifically, pread returns 0 for negative offset
   and an offset after the end of the file
   (as long as the the rest of the requirements are fulfiiled).
   You are suppose to preserve this behavior also in your implementation.

 * Changed in version 2.2
 */
int caching_read(const char *path, char *buf, size_t size,
                off_t offset, struct fuse_file_info *fi){
    cout << "-- read --" << endl; // todo remove

    size_t res = 0;

    // get the full path of the file
    char fpath[PATH_MAX];
    caching_full_path(fpath, path);

    // if the file is the log, return no such file error todo

    // if size is negative, return error
    if (size < 0)
    {
        return 0;
    }

    // get file descriptor
    int fd = (int) fi->fh;

    // get blocksize and size
    struct stat st;
    fstat(fd, &st);
    int file_size = (int) st.st_size;
    int block_size = (int) st.st_blksize;

    // if the offset is negative or greater than size, return 0
    if (offset < 0 || file_size < offset)
    {
        return 0;
    }

    // calculate first block and number of blocks
    int first_block = (int) (offset / block_size);
    int number_of_blocks = (int) (size / block_size);
    int last_block = first_block + number_of_blocks;

    // declare pointer buffer to store block data
    char *block_buf;

    size_t bytes_to_read = size;

    // read data from file blocks
    for (int block = first_block; block < last_block; block++)
    {
        int block_offset = block * block_size;

        ssize_t read_bytes = block_size;

        // create a block pair, and hash key
        BlockID blockID;
        blockID.first = (int) st.st_ino;
        blockID.second = block;

        // search for the block in the cache and read it
        block_buf = (char *) cacheManager->retrieveBuffer(blockID);

        // couldn't find the block in the cache
        if (block_buf == nullptr)
        {
            // allocate space for data from the disk
            block_buf = (char *) aligned_alloc(block_size, block_size);

            // read the block from the disk
            read_bytes = pread(fd, block_buf, block_size, block_offset);

            // store block in the cache
            cacheManager->insertBlock((int) st.st_ino, block, block_buf, fpath);
        }

        // update the number of bytes to read if it's less than a block size
        if (bytes_to_read < read_bytes)
        {
            read_bytes = bytes_to_read;
        }

        // add data to the return buffer
        memcpy((buf + (block_size * (block - first_block))), block_buf, read_bytes);

        // add number of bytes retrieved from the block
        res += read_bytes;
        bytes_to_read -= read_bytes;
    }

    return (int) res;
}

/** Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync().  It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor.  So if a
 * filesystem wants to return write errors in close() and the file
 * has cached dirty data, this is a good place to write back data
 * and return any errors.  Since many applications ignore close()
 * errors this is not always useful.
 *
 * NOTE: The flush() method may be called more than once for each
 * open().  This happens if more than one file descriptor refers
 * to an opened file due to dup(), dup2() or fork() calls.  It is
 * not possible to determine if a flush is final, so each flush
 * should be treated equally.  Multiple write-flush sequences are
 * relatively rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will always be called
 * after some writes, or that if will be called at all.
 *
 * Changed in version 2.2
 */
int caching_flush(const char *path, struct fuse_file_info *fi)
{
    cout << "-- flush --" << endl;

    char fpath[PATH_MAX];
    caching_full_path(fpath, path);
    int res = refering_logfile(fpath);

    return res;
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 *
 * Changed in version 2.2
 */
int caching_release(const char *path, struct fuse_file_info *fi){
    cout << "-- release --" << endl;

    char fpath[PATH_MAX];
    caching_full_path(fpath, path);

    int res = refering_logfile(fpath);
    if (res)
    {
        return res;
    }

    (void) path;
    log_call("release");
    struct stat st;
    fstat(fi->fh, &st);

    res = close((int) fi->fh);

    return res;
}

/** Open directory
 *
 * This method should check if the open operation is permitted for
 * this  directory
 *
 * Introduced in version 2.3
 */
int caching_opendir(const char *path, struct fuse_file_info *fi){
    cout << "   -- opendir --" << endl; // todo remove

    // get full path from path
    char fullPath[PATH_MAX];
    caching_full_path(fullPath, path);

    // check reference to the log file
    int res = refering_logfile(fullPath);

    if (res)
    {
        return res;
    }

    // get DIR* from path, and log the call
    log_call("opendir");
    DIR *dp = opendir(fullPath);

    if (dp == NULL)
    {
        return -errno;
    }

    // init fi->fh with the DIR*
    fi->fh = (uint64_t) dp;

    return 0;
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * Introduced in version 2.3
 */
int caching_readdir(const char *path, void *buf,
                    fuse_fill_dir_t filler,
                    off_t offset, struct fuse_file_info *fi){
    cout << "-- readdir --" << endl;

    (void) offset;

    int res = 0;
    DIR *dirPointer;
    struct dirent *dirEnt;

    // get the file handler of the current dir
    dirPointer = (DIR *) fi->fh;

    // read each file in the dir
    while ((dirEnt = readdir(dirPointer)) != NULL)
    {
        if (dirEnt == 0)
        {
            res = -errno;
            return res;
        }

        char fpath[PATH_MAX];
        caching_full_path(fpath, path);

        // ignore the logfile when you find it
        if (strcmp(dirEnt->d_name, logfile_name) == 0)
        {
            continue;
        }

        // set the properties of every file in the dir
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = dirEnt->d_ino;

        if (filler(buf, dirEnt->d_name, &st, 0))
            break;
    }

    return res;
}

/** Release directory
 *
 * Introduced in version 2.3
 */
int caching_releasedir(const char *path, struct fuse_file_info *fi){
    cout << " -- releasedir -- " << endl;

    uint64_t dp = fi->fh;
    (void) path;
    closedir((DIR *) dp);

    return 0;
}

/** Rename a file */
int caching_rename(const char *path, const char *newpath){
    cout << "-- rename --" << endl; // todo remove

    int res = 0;
    char fpath[PATH_MAX];
    char fnewpath[PATH_MAX];

    // generate full path from the given paths
    caching_full_path(fpath, path);
    caching_full_path(fnewpath, newpath);

    // check if trying to rename the logfile
    res = refering_logfile(fpath);
    if (res)
    {
        return res;
    }

    log_call("rename");
    res = rename(fpath, fnewpath); // todo moove this line abobe the cache check?

    // todo chack if file is cached, and rename the relevant blocks if it is
    return res;
}

/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * destroy() method.
 *

If a failure occurs in this function, do nothing (absorb the failure
and don't report it).
For your task, the function needs to return NULL always
(if you do something else, be sure to use the fuse_context correctly).
 * Introduced in version 2.3
 * Changed in version 2.6
 */
void *caching_init(struct fuse_conn_info *conn){
    cout << "-- init --" << endl; // todo remove
    fuse_get_context();
    return CF_LOG;
}


/**
 * Clean up filesystem
 *
 * Called on filesystem exit.

If a failure occurs in this function, do nothing
(absorb the failure and don't report it).

 * Introduced in version 2.3
 */
void caching_destroy(void *userdata){ // todo what this function does? (destroys the returned value of the init function)
    cout << "-- destroy --" << endl; // todo remove
}


/**
 * Ioctl from the FUSE sepc:
 * flags will have FUSE_IOCTL_COMPAT set for 32bit ioctls in
 * 64bit environment.  The size and direction of data is
 * determined by _IOC_*() decoding of cmd.  For _IOC_NONE,
 * data will be NULL, for _IOC_WRITE data is out area, for
 * _IOC_READ in area and if both are set in/out area.  In all
 * non-NULL cases, the area is of _IOC_SIZE(cmd) bytes.
 *
 * However, in our case, this function only needs to print
 cache table to the log file .
 *
 * Introduced in version 2.8
 */
int caching_ioctl (const char *, int cmd, void *arg,
        struct fuse_file_info *, unsigned int flags, void *data){
    cout << "-- ioctl --" << endl; // todo remove
    return 0;
}


// Initialise the operations.
// You are not supposed to change this function.
void init_caching_oper()
{
    caching_oper.getattr = caching_getattr;
    caching_oper.access = caching_access;
    caching_oper.open = caching_open;
    caching_oper.read = caching_read;
    caching_oper.flush = caching_flush;
    caching_oper.release = caching_release;
    caching_oper.opendir = caching_opendir;
    caching_oper.readdir = caching_readdir;
    caching_oper.releasedir = caching_releasedir;
    caching_oper.rename = caching_rename;
    caching_oper.init = caching_init;
    caching_oper.destroy = caching_destroy;
    caching_oper.ioctl = caching_ioctl;
    caching_oper.fgetattr = caching_fgetattr;


    caching_oper.readlink = NULL;
    caching_oper.getdir = NULL;
    caching_oper.mknod = NULL;
    caching_oper.mkdir = NULL;
    caching_oper.unlink = NULL;
    caching_oper.rmdir = NULL;
    caching_oper.symlink = NULL;
    caching_oper.link = NULL;
    caching_oper.chmod = NULL;
    caching_oper.chown = NULL;
    caching_oper.truncate = NULL;
    caching_oper.utime = NULL;
    caching_oper.write = NULL;
    caching_oper.statfs = NULL;
    caching_oper.fsync = NULL;
    caching_oper.setxattr = NULL;
    caching_oper.getxattr = NULL;
    caching_oper.listxattr = NULL;
    caching_oper.removexattr = NULL;
    caching_oper.fsyncdir = NULL;
    caching_oper.create = NULL;
    caching_oper.ftruncate = NULL;
}


//basic main. You need to complete it.
int main(int argc, char* argv[]){

    // todo verify number of arguments and their correctness, print usage message if wrong

    struct cfs_state cfs_st;

//    int numberOfBlocks, int blockSize, int fOld, int fNew // todo remove
    // create a cache manager instance
    CacheManager *cm = new CacheManager(10000, 0.2, 0.2);
    cacheManager = cm;

    init_caching_oper();

    set_root_dir(argv[1], &cfs_st);

    char log_full_path[PATH_MAX];
    strcpy(log_full_path, cfs_st.rootdir);
    strcat(log_full_path, "/");
    strncat(log_full_path, logfile_name, PATH_MAX);
    cfs_st.logfile_full_path = log_full_path;

    cfs_st.logfile = open_log(log_full_path);


    argv[1] = argv[2];
    for (int i = 2; i< (argc - 1); i++){
        argv[i] = NULL;
    }
        argv[2] = (char*) "-s";
        argv[3] = (char*) "-f"; // todo remove before submission
    argc = 4;

    int fuse_stat = fuse_main(argc, argv, &caching_oper, &cfs_st);

    free(cfs_st.rootdir);
    delete cacheManager;

    // todo fix memory leek in open_log(char *). Should be in caching_destroy?

    return fuse_stat;
}
