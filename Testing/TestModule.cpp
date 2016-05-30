// TestModule.cpp

#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include "TestModule.h"

#define OPEN_FLAGS O_RDONLY | O_DIRECT | O_SYNC

// todo save errno to another variable before printing

void TestModule::zeroErrno() {
    errno = 0;
}

void TestModule::getattrTest() {

    std::cout << " -- getattr test -- " << std::endl;

    zeroErrno();

    struct stat *stat_buf = new struct stat;

    // test on an existing file
    int res = stat("mount_dir", stat_buf);

    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    res = stat("no_such_file", stat_buf);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void TestModule::accessTest() {

    std::cout << " -- access test -- " << std::endl;

    zeroErrno();

    // test on an existing file
    int res = access("mount_dir", F_OK);
    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    res = access("no_such_file", F_OK);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void TestModule::opendirTest() {

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

void TestModule::readdirTest() {
    std::cout << " -- readdir test -- " << std::endl;

    zeroErrno();
    DIR *pdir;

    // test on an existing file
    pdir = opendir("/");
    struct dirent *de;
    de = readdir(pdir);
    std::cout << "de->d_name: " << de->d_name << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    pdir = opendir("/no_such_file");
    std::cout << "de->d_name: " << de->d_name << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void TestModule::openTest() {
    std::cout << " -- open test -- " << std::endl;

    zeroErrno();

    int res;

    // test on an existing file
    res = open("mount_dir/a_file.txt", OPEN_FLAGS);
    std::cout << "expect a small integer: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test on non existing file
    res = open("mount_dir/open_a_file.txt", OPEN_FLAGS);
    std::cout << "expect -1 (no permissions to create a new file): " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    zeroErrno();

    // test on file that cannot be created
    res = open("/cant_open_a_file.txt", OPEN_FLAGS);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}


int main ()
{
    TestModule testModule;

    testModule.accessTest();
    testModule.getattrTest();
    testModule.readdirTest();
    testModule.opendirTest();
    testModule.openTest();

    return 0;
}