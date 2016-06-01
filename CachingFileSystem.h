// CachingFileSystem.h

#ifndef EX4_CACHINGFILESYSTEM_H
#define EX4_CACHINGFILESYSTEM_H



void set_root_dir(char *root_dir, cfs_state *state);

void caching_full_path(char *absPath, const char *path);

int caching_getattr(const char *path, struct stat *statbuf);

int caching_fgetattr(const char *path, struct stat *statbuf,
                     struct fuse_file_info *fi);

int caching_open(const char *path, struct fuse_file_info *fi);

#endif //EX4_CACHINGFILESYSTEM_H
