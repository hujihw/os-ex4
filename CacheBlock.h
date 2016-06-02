// CacheBlock.h

#ifndef EX4_CACHEBLOCK_H
#define EX4_CACHEBLOCK_H

#include <limits.h>
#include <utility>

enum Section {newSection, middleSection, oldSection};

// the fileId and the block number
typedef std::pair<int, int> BlockID;

/**
 * @brief This class represents a block in the cache.
 */
class CacheBlock {
public:

    /**
     * @brief constructor, initializes the refCount to 1.
     */
    CacheBlock(int fileId, int blockNumber, char *buff);

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
     * @brief getter for the file id
     */
    int getFileId() const;

    /**
     * @brief getter for the block id of the block
     */
    BlockID getBlockId() const;

    /**
     * @brief getter for the section type that the block is in
     */
    Section getSection() const;

    /**
     * @brief setter for the section type that the block is in
     */
    void setSection(Section section);

private:
    int refCount;
    int blockNumber;
    int fileId;
    char* buff;
    Section section;
};

#endif //EX4_CACHEBLOCK_H
