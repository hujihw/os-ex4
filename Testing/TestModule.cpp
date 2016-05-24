// TestModule.cpp

#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include "TestModule.h"

// todo save errno to another variable before printing

void TestModule::getattrTest() {

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
    // test on an existing file
    DIR *pdir;

    pdir = opendir("/");
//    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    pdir = opendir("/no_such_dir");
//    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}


int main() {
    TestModule *tester = new TestModule();

    // run tests
    tester->getattrTest();
    tester->accessTest();

    return 0;
}