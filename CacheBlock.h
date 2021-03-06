// CacheBlock.h

#ifndef EX4_CACHEBLOCK_H
#define EX4_CACHEBLOCK_H

#include <limits.h>
#include <utility>
#include <string>

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
    CacheBlock(int fileId, int blockNumber, const char *buff,
               char *path);

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
    const char * getBuff() const;

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

    /**
     * @brief getter for the file path
     */
    char* getPath() const;

    /**
     * @brief setter for the file path
     */
    void setPath(char * path);

private:
    int refCount;
    int blockNumber;
    int fileId;
    const char* buff;
    Section section;
    std::string path;

};

#endif //EX4_CACHEBLOCK_H
