// CacheManager.cpp

#include <cmath>
#include <iostream>
#include <limits>
#include "CacheManager.h"

#define BLOCK_NOT_FOUND 0
#define NULL_BLOCK -1

/**
 * @brief constructor for the CacheManager class.
 */
CacheManager::CacheManager(int numberOfBlocks, double fOld, double fNew)
        : numberOfBlocks(numberOfBlocks), blocksMap(), cacheChain(){

    int newSectionSize = (int) floor(fNew * numberOfBlocks);
    int oldSectionSize = (int) floor(fOld * numberOfBlocks);

    // this int can be zero
    int middleSectionSize = numberOfBlocks - newSectionSize - oldSectionSize;

    // fills the list with nullBlocks, numberOfBlocks times
    for (int i = 1; i <= numberOfBlocks; i++){
        CacheBlock* nullBlock = new CacheBlock(-1, -1, nullptr); // todo maybe-not new
        cacheChain.push_front(nullBlock);
    }

    // assigns the bounds iterators
    int i = 1;
    for (CacheChain::iterator it = cacheChain.begin(); it != cacheChain.end()
            ; it++){
        if (i == newSectionSize + 1){
            std::cout<<i<<std::endl;
            middleSectionIter = it;
        }
        if (i == newSectionSize + middleSectionSize + 1){
            std::cout<<i<<std::endl;
            oldSectionIter = it;
        }
        i++;
    }

    std::cout<<"the size of the sections are:" << newSectionSize << ", " <<
            middleSectionSize << ", "<<oldSectionSize<< std::endl;
    std::cout<<"the first elements fileId of the sections are:" <<
            middleSectionIter.operator*()->getFileId()
    << ", " << oldSectionIter.operator*()->getFileId() << std::endl;

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
            (*middleSectionIter)->setSection(middleSection);
            break;

        case oldSection:
            // increment the blocks refcount
            (*blockIter)->incrementRefCount();

            middleSectionIter--;
            (*middleSectionIter)->setSection(middleSection);
            oldSectionIter--;
            (*oldSectionIter)->setSection(oldSection);
            break;
    }

    cacheChain.erase(blockIter); // erase from original location
    return cacheChain.begin();
}

/**
 * @brief Retrieve a block's buffer from the cache
 * returns nullptr if the block was not found
 */
const char * CacheManager::retrieveBuffer(BlockID blockID) {
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
void CacheManager::insertBlock(int fileDesc, int blockNumber, const char *buff){
    // tODO find first the block and if it exists print the error and exit

    CacheBlock* block = new CacheBlock(fileDesc, blockNumber, buff);
    // put the new block at the top and update it in the map
    cacheChain.emplace_front(block);
    blocksMap[block->getBlockId()] = cacheChain.begin();

    // correct both boundaries and update the new bounds section attribute
    middleSectionIter--;
    (*middleSectionIter)->setSection(middleSection);
    oldSectionIter--;
    (*oldSectionIter)->setSection(oldSection);

    // remove the last element if the list is not full
    if (cacheChain.back()->getBlockNumber() == NULL_BLOCK){
        cacheChain.pop_back();
    }
    else{ //find the least referenced block in oldSection and remove it

        int minimalReff = std::numeric_limits<int>::max(); // like inf
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



}

/**
 * @brief returns an iterator to the end of the cache. for checking if
 * the find method was successful
 */
CacheChain::iterator CacheManager::getCacheEnd() {
    return cacheChain.end();
}
