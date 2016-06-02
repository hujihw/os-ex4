// CacheManager.cpp

#include <cmath>
#include <iostream>
#include "CacheManager.h"


/**
 * @brief constructor for the CacheManager class.
 */
CacheManager::CacheManager(int numberOfBlocks, int blockSize, double fOld,
                           double fNew)
        : numberOfBlocks(numberOfBlocks), blockSize(blockSize), blocksMap(),
          cacheChain(){
    int newSectionSize = (int) floor(fNew * numberOfBlocks);
    int oldSectionSize = (int) floor(fOld * numberOfBlocks);

    // this int can be zero
    int middleSectionSize = numberOfBlocks - newSectionSize - oldSectionSize;

    // fills the list with nullptr, newSectionSize +1 times.
    cacheChain.assign((unsigned long) newSectionSize + 1, nullptr);
    middleSectionIter = cacheChain.end();

    // fills the list with nullptr, middleSectionSize times. could be zero
    cacheChain.assign((unsigned long) middleSectionSize, nullptr);
    oldSectionIter = cacheChain.end();

    // fills the list with nullptr, oldSectionSize -1 times.
    cacheChain.assign((unsigned long) oldSectionSize - 1, nullptr);

    std::cout<<"the size of the sections are:" << newSectionSize << ", " <<
            middleSectionSize << ", "<<oldSectionSize<< std::endl;
}

/**
 * @brief destructor for the CacheManager class.
 */
CacheManager::~CacheManager() {
    cacheChain.clear();
}

/**
 * @brief Find the given block in cache
 * returns nullptr if the block was not found
 */
CacheChain::iterator CacheManager::findBlock(BlockID blockID) {
    auto got = blocksMap.find(blockID);

    if (got == blocksMap.end()){
        // the block is not in the cache
        return cacheChain.end();
    }
    auto blockIter = got->second;

    // move the block to the head and update its section attribute to newSection
    cacheChain.push_front(*blockIter); //put at head
    cacheChain.erase(blockIter); // erase from original location
    (*blockIter)->setSection(newSection); // set the section attribute to newSection
    // reset the iterator in the map to point at the block
    blocksMap[blockID] = cacheChain.begin();
    // increment the blocks refcount
    (*blockIter)->incrementRefCount();

    // correct the relevant bounds and update the new bounds section attribute
    switch ((*blockIter)->getSection()){
        case newSection:
            break;
        case middleSection:
            // add code
            break;
    }


    return got->second;
}

/**
 * @brief Retrieve a block's buffer from the cache
 * returns nullptr if the block was not found
 */
char *CacheManager::retrieveBuffer(BlockID blockID) {
    auto blockIter = findBlock(blockID);

    if (*blockIter == NULL){
        return nullptr;
    }

    return (*blockIter)->getBuff();
}

/**
 * @brief Insert a new block to the cache.
 */
void CacheManager::insertBlock(int fileDesc, int blockNumber, char *buff) {
    CacheBlock* block = new CacheBlock(fileDesc, blockNumber, buff);
    // remove the relevant block from the old section
    // correct the both boundaries and update section atrr
    cacheChain.emplace_front(block);
    blocksMap[block->getBlockId()] = cacheChain.begin();

}

/**
 * @brief returns an iterator to the end of the cache. for checking if
 * the find method was succesful
 */
CacheChain::iterator CacheManager::getCacheEnd() {
    return cacheChain.end();
}