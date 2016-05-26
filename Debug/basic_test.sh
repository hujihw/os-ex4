#!/usr/bin/env bash

#A simple test script for the CachingFileSystem

#define the path for the mount and root dir
BIN_PATH=/media/sf_huji/courses/os/hw/ex4/Debug
ROOTDIR=$BIN_PATH/root_dir
MOUNTDIR=$BIN_PATH/mount_dir

# unmount the last mount
fusermount -u -q /media/sf_huji/courses/os/hw/ex4/Debug/mount_dir

# $BIN_PATH/CachingFileSystem $ROOTDIR $MOUNTDIR
valgrind $BIN_PATH/CachingFileSystem $ROOTDIR $MOUNTDIR

# make sure nothing stays mounted
fusermount -u /media/sf_huji/courses/os/hw/ex4/Debug/mount_dir
echo "Unmounted FUSE"

echo "Done"
