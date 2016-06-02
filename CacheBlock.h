// CacheBlock.h

#ifndef EX4_CACHEBLOCK_H
#define EX4_CACHEBLOCK_H

#include <limits.h>
#include <utility>
//#include "CacheManager.h"

// the file descriptor and the block number
typedef std::pair<int, int> BlockID;

/**
 * @brief This class represents a block in the cache.
 */
class CacheBlock {
public:

    /**
     * @brief constructor, initializes the refCount to 1.
     */
    CacheBlock(int fileDesc, int blockNumber, char* buff);

    /**
     * @brief getter to the counter of times the block was called.
     * @return the number of times the counter was raised.
     */
    int getRefCount() const;

    /**
     * @brief increments the counter of the block by 1.
     */
    void incrementRefCount();

    /**
     * @brief getter for the block number
     */
    int getBlockNumber() const;

    /**
     * @ brief getter for the buffer that holds the file contents
     */
    char * getBuff() const;

    /**
     * @brief getter for the file descriptor
     */
    int getFileDescriptor() const;

    /**
     * @brief getter for the block id of the block
     */
    BlockID getBlockId() const;

private:
    int refCount;
    int blockNumber;
    int fileDesc;
    char* buff;
};

#endif //EX4_CACHEBLOCK_H
