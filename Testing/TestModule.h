// TestModule.h

#include <sys/stat.h>
#include "../CacheManager.h"

#ifndef EX4_TESTMODULE_H
#define EX4_TESTMODULE_H

/**
 * @brief This class holds test functions for the Caching filesystem
 *        function implementations.
 */
class FuseTester {
public:
    void zeroErrno();

    void getattrTest();

    void accessTest();

    void opendirTest();

    void readdirTest();

    void openTest();

    void renamrTest();
};

class CacheTester {
public:
    CacheManager initTest();
};


#endif //EX4_TESTMODULE_H
