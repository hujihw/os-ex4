// CacheBlock.cpp

#include "CacheBlock.h"

/**
 * @brief constructor, initializes the refCount to 1.
 */
CacheBlock::CacheBlock(int fileId, int blockNumber, const char *buff) {
    refCount = 1;
    this->fileId = fileId;
    this->blockNumber = blockNumber;
    this->buff = buff;
    this->section = Section(newSection);
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
const char * CacheBlock::getBuff() const {
    return buff;
}

/**
 * @brief getter for the fileId
 */
int CacheBlock::getFileId() const {
    return fileId;
}

/**
 * @brief getter for the block id of the block
 */
BlockID CacheBlock::getBlockId() const {
    return std::pair<int, int>(fileId, blockNumber);
}


/**
 * @brief setter for the section type that the block is in
 */
void CacheBlock::setSection(Section section) {
    CacheBlock::section = section;
}

/**
 * @brief getter for the section type that the block is in
 */
Section CacheBlock::getSection() const {
    return section;
}