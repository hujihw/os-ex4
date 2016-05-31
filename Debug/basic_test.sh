#!/usr/bin/env bash

#A simple test script for the CachingFileSystem

#define the path for the mount and root dir
BIN_PATH=/home/omri/Documents/ex4/Debug
ROOTDIR=$BIN_PATH/root_dir
MOUNTDIR=$BIN_PATH/mount_dir

# unmount the last mount
fusermount -u -q $MOUNTDIR

# $BIN_PATH/CachingFileSystem $ROOTDIR $MOUNTDIR
valgrind --leak-check=full --show-leak-kinds=all $BIN_PATH/CachingFileSystem $ROOTDIR $MOUNTDIR
#$BIN_PATH/TestModule

# make sure nothing stays mounted
fusermount -u $MOUNTDIR
echo "Unmounted FUSE"

echo "Done"
