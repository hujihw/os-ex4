// CacheBlock.cpp

#include "CacheBlock.h"

/**
 * @brief constructor, initializes the refCount to 1.
 */
CacheBlock::CacheBlock(int fileDesc, int blockNumber, char *buff) {
    refCount = 1;
    this->fileDesc = fileDesc;
    this->blockNumber = blockNumber;
    this->buff = buff;
}

/**
 * @brief getter to the counter of times the block was called.
 * @return the number of times the counter was raised.
 */
int CacheBlock::getRefCount() const {
    return refCount;
}

/**
 * @brief increments the counter of the block by 1.
 */
void CacheBlock::incrementRefCount() {
    refCount += 1;
}

/**
* @brief getter for the block number
*/
int CacheBlock::getBlockNumber() const {
    return blockNumber;
}

/**
 * @ brief getter for the buffer that holds the file contents
 */
char *CacheBlock::getBuff() const {
    return buff;
}

/**
 * @brief getter for the file descriptor
 */
int CacheBlock::getFileDescriptor() const {
    return fileDesc;
}

/**
 * @brief getter for the block id of the block
 */
BlockID CacheBlock::getBlockId() const {
    return std::pair<int, int>(fileDesc, blockNumber);
}





