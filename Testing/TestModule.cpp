// TestModule.cpp

#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include "TestModule.h"

#define OPEN_FLAGS O_RDONLY | O_DIRECT | O_SYNC

// todo save errno to another variable before printing

void FuseTester::zeroErrno() {
    errno = 0;
}

void FuseTester::getattrTest() {

    std::cout << " -- getattr test -- " << std::endl;

    zeroErrno();

    struct stat *stat_buf = new struct stat;

    // test on an existing file
    std::cout << std::endl << "Test stat on an existing file" << std::endl;
    int res = stat("mount_dir", stat_buf);

    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    std::cout << std::endl << "Test stat on a non existing file" << std::endl;
    res = stat("mount_dir/no_such_file", stat_buf);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test logfile reference
    std::cout << std::endl << "Test stat on the logfile" << std::endl;
    res = stat("mount_dir/filesystem.log", stat_buf);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void FuseTester::accessTest() {

    std::cout << " -- access test -- " << std::endl;

    zeroErrno();

    // test on an existing file
    int res = access("mount_dir", F_OK);
    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    res = access("mount_dir/no_such_file", F_OK);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void FuseTester::opendirTest() {

    std::cout << " -- opendir test -- " << std::endl;

    zeroErrno();

    // test on an existing file
    DIR *pdir;

    pdir = opendir("mount_dir");
    std::cout << "expect a large integer: " << (uint64_t) pdir << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    pdir = opendir("/no_such_dir");
    std::cout << "expect NULL: " << (uint64_t) pdir << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void FuseTester::readdirTest() {
    std::cout << " -- readdir test -- " << std::endl;

    zeroErrno();
    DIR *pdir;

    // test on an existing file
    pdir = opendir("mount_dir");
    struct dirent *de;
    de = readdir(pdir);
    std::cout << "de->d_name: " << de->d_name << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    pdir = opendir("mount_dir/no_such_file");
    std::cout << "de->d_name: " << de->d_name << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void FuseTester::openTest() {
    std::cout << " -- open test -- " << std::endl;

    zeroErrno();

    int res;

    // test on an existing file
    std::cout << "Test opening an existing file" << std::endl;
    res = open("mount_dir/a_file.txt", OPEN_FLAGS); // todo check if the returned 5 is an error
    std::cout << "expect a small integer: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test on non existing file
    std::cout << "Test opening a non existing" << std::endl;
    res = open("mount_dir/open_a_file.txt", OPEN_FLAGS);
    std::cout << "expect -1 (no permissions to create a new file): " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test logfile reference
    std::cout << "Test opening the logfile" << std::endl;
    res = open("mount_dir/filesystem.log", OPEN_FLAGS);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();
}

void FuseTester::renamrTest() {

    std::cout << " -- rename test -- " << std::endl;

    int res = 0;
    zeroErrno();

    // test logfile reference
    std::cout << std::endl << "Rename the logfile" << std::endl;
    res = rename("mount_dir/filesystem.log", "mount_dir/new_name.log");
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test renaming existing file
    std::cout << std::endl << "Rename an existing file" << std::endl;
    res = rename("mount_dir/a_file.txt", "mount_dir/new_name.txt");
    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test renaming a non existing file
    std::cout << std::endl << "Rename a non existing file" << std::endl;
    res = rename("mount_dir/no_such_file", "mount_dir/no_file_new_name.txt");
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();
}

CacheManager CacheTester::initTest() {
    std::cout<<"construct a CacheManager with parameters:";
    CacheManager m = CacheManager(100, 10, 0.33, 0.33);

}


int main ()
{
    FuseTester fuseTester;

    fuseTester.accessTest();
    fuseTester.getattrTest();
    fuseTester.readdirTest();
    fuseTester.opendirTest();
    fuseTester.openTest();
    fuseTester.renamrTest();

    return 0;
}



