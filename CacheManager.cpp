// CacheManager.cpp

#include <sys/stat.h>
#include "CacheManager.h"


/**
 * @brief constructor for the CacheManager class.
 */
CacheManager::CacheManager(int numberOfBlocks, int blockSize, int fOld, int fNew)
        : numberOfBlocks(numberOfBlocks), blockSize(blockSize), blocksMap(),
          cacheChain(), { }

/**
 * @brief Find the given block in cache
 * returns nullptr if the block was not found
 */
CacheChain::iterator CacheManager::findBlock(BlockID blockID) {
    auto got = blocksMap.find(blockID);

    if (got == blocksMap.end()){
        // the block is not in the map
        return nullptr;
    }
    return got->second;
}

/**
 * @brief Retrieve a block's buffer from the cache
 * returns nullptr if the block was not found
 */
char *CacheManager::retrieveBuffer(BlockID blockID) {
    auto blockIter = findBlock(blockID);

    if (blockIter == nullptr){
        return nullptr;
    }

    return (*blockIter)->getBuff();
}

/**
 * @brief Insert a new block to the cache.
 */
void CacheManager::insertBlock(CacheBlock *block) {


    // todo remove tail, and move the boundaries

    cacheChain.emplace_front(block);
    blocksMap[block->getBlockId()] = cacheChain.begin();

}













