// TestModule.h

#include <sys/stat.h>

#ifndef EX4_TESTMODULE_H
#define EX4_TESTMODULE_H

/**
 * @brief This class holds test functions for the Caching filesystem
 *        function implementations.
 */
class TestModule {
public:
    void zeroErrno();

    void getattrTest();

    void accessTest();

    void opendirTest();

    void readdirTest();

    void openTest();
};


#endif //EX4_TESTMODULE_H
