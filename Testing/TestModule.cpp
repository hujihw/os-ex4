// TestModule.cpp

#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include "TestModule.h"
#include "../CachingFileSystem.cpp"

// todo save errno to another variable before printing

void TestModule::zeroErrno() {
    errno = 0;
}

void TestModule::getattrTest() {

    std::cout << " -- getattr test -- " << std::endl;

    zeroErrno();

    struct stat *stat_buf = new struct stat;

    // test on an existing file
    int res = stat("/", stat_buf);
    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    res = stat("/no_such_file", stat_buf);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void TestModule::accessTest() {

    std::cout << " -- access test -- " << std::endl;

    zeroErrno();

    // test on an existing file
    int res = access("/", F_OK);
    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    res = access("/no_such_file", F_OK);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

void TestModule::opendirTest() {

    std::cout << " -- opendir test -- " << std::endl;

    zeroErrno();

    // test on an existing file
    DIR *pdir;

    pdir = opendir("/");
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


int main() {
    TestModule *tester = new TestModule();

    // run tests
    tester->getattrTest();
    tester->accessTest();
    tester->opendirTest();
    tester->readdirTest();

    return 0;
}