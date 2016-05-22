// TestModule.cpp

#include <iostream>
#include "TestModule.h"

void TestModule::getattrTest() {

    struct stat *stat_buf = new struct stat;

    // test on an existing file
    int res = stat("/", stat_buf);
    std::cout << "expect 0: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;

    // test file that doesn't exist
    stat("no_such_file", stat_buf);
    std::cout << "expect -1: " << res << std::endl;
    std::cout << "errno: " << errno << std::endl;
}

int main() {
    TestModule *tester = new TestModule();

    // test getattr
    tester->getattrTest();

    return 0;
}