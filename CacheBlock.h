// CacheBlock.h

#ifndef EX4_CACHEBLOCK_H
#define EX4_CACHEBLOCK_H

#include <limits.h>

/**
 * @brief This class represents a block in the cache.
 */
class CacheBlock {
public:

    /**
     * @brief A default constructor, initializes the count to 1.
     */
    CacheBlock(char *fileName, int blockNum);

    /**
     * @brief A getter to the counter of times the block was called.
     * @return the number of times the counter was raised.
     */
    int getCount() const;

    /**
     * @brief Raises the counter of the block by 1.
     */
    void raiseCount();

private:
    int count;
    char *fileName;
    int blockNumber;
    int fileDescriptor; // todo remove?
};

#endif //EX4_CACHEBLOCK_H
