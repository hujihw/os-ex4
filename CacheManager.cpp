// CacheManager.cpp

#include <cmath>
#include <iostream>
#include "CacheManager.h"

#define BLOCK_NOT_FOUND 0


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
 * returns cacheChain.end() if the block was not found
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
    (*cacheChain.begin())->setSection(newSection); // set the section attribute to newSection
    // reset the iterator in the map to point at the block
    blocksMap[blockID] = cacheChain.begin();


    // correct the relevant bounds and update the new bounds section attribute
    switch ((*blockIter)->getSection()){

        case newSection:
            break;

        case middleSection:
            // increment the blocks refcount
            (*blockIter)->incrementRefCount();

            middleSectionIter--;
            if ((*middleSectionIter) != nullptr) {
                (*middleSectionIter)->setSection(middleSection);
            }
            break;

        case oldSection:
            // increment the blocks refcount
            (*blockIter)->incrementRefCount();

            middleSectionIter--;
            if ((*middleSectionIter) != nullptr) {
                (*middleSectionIter)->setSection(middleSection);
            }
            oldSectionIter--;
            if ((*oldSectionIter) != nullptr) {
                (*oldSectionIter)->setSection(oldSection);
            }
            break;
    }

    cacheChain.erase(blockIter); // erase from original location
    return cacheChain.begin();
}

/**
 * @brief Retrieve a block's buffer from the cache
 * returns nullptr if the block was not found
 */
char *CacheManager::retrieveBuffer(BlockID blockID) {
    auto blockIter = findBlock(blockID);

    if (blockIter == cacheChain.end()){
        return nullptr;
    }

    return (*blockIter)->getBuff();
}


/**
 * @brief Retrieve a block's file id from the cache
 * returns zero if the block was not found
 */
int CacheManager::retrieveFileId(BlockID blockID) {
    auto blockIter = findBlock(blockID);

    if (blockIter == cacheChain.end()){
        return BLOCK_NOT_FOUND;
    }

    return (*blockIter)->getFileId();
}

/**
 * @brief Insert a new block to the cache.
 */
void CacheManager::insertBlock(int fileDesc, int blockNumber, char *buff) {
    CacheBlock* block = new CacheBlock(fileDesc, blockNumber, buff);

    // remove the last element if the list is not full
    if (cacheChain.back() == nullptr){
        cacheChain.pop_back();
    }
    else{ //find the least referenced block in oldSection and remove it
        int minimalReff = 0;
        CacheChain::iterator eraseCandidateBlockIter;
        for (CacheChain::iterator it=oldSectionIter; it != cacheChain.end();
             ++it){
            if ((*it)->getRefCount() <= minimalReff){
                eraseCandidateBlockIter = it;
                minimalReff = (*eraseCandidateBlockIter)->getRefCount();
            }
        }
        cacheChain.erase(eraseCandidateBlockIter);
    }

    // correct both boundaries and update the new bounds section attribute
    middleSectionIter--;
    if ((*middleSectionIter) != nullptr) {
        (*middleSectionIter)->setSection(middleSection);
    }
    oldSectionIter--;
    if ((*oldSectionIter) != nullptr) {
        (*oldSectionIter)->setSection(oldSection);
    }

    // put the new block at the top and update it in the map
    cacheChain.emplace_front(block);
    blocksMap[block->getBlockId()] = cacheChain.begin();

}

/**
 * @brief returns an iterator to the end of the cache. for checking if
 * the find method was successful
 */
CacheChain::iterator CacheManager::getCacheEnd() {
    return cacheChain.end();
}
