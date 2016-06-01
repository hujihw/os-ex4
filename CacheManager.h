// CacheManager.h

#ifndef EX4_CACHEMANAGER_H
#define EX4_CACHEMANAGER_H

#include <unordered_map>
#include <list>
#include <string>
#include <functional>
#include "CacheBlock.h"

// the file descriptor and the block number
typedef std::pair<int, int> BlockID;

typedef std::list<CacheBlock*> CacheChain; // TODO check necessity of *

typedef std::unordered_map<BlockID, CacheChain::iterator> BlocksMap;

class CacheManager {

public:
    /**
     * @brief constructor for the CacheManager class.
     */
    CacheManager(int numberOfBlocks, int blockSize, int fOld, int fNew);

    /**
     * @brief Find the given block in cache
     * returns nullptr if the block was not found
     */
    CacheChain::iterator findBlock(BlockID blockID);

    /**
     * @brief Retrieve a block's buffer from the cache
     * returns nullptr if the block was not found
     */
    char *retrieveBuffer(BlockID blockID); // TODO should i return a pointer
    //  TODO to the char * ??

    /**
     * @brief Insert a new block to the cache.
     */
    void insertBlock(CacheBlock *block);


    /**
     * @brief Move a block to the head of the list, and change relevant pointers.
     */
    void moveToHead(BlockID blockID);


private:
    // data members
    int numberOfBlocks;
    int blockSize;
    BlocksMap blocksMap;
    CacheChain cacheChain;
    CacheChain::iterator middleSection; // the first block in the middle section
    CacheChain::iterator oldsection; // the first block in te old section
};



#endif //EX4_CACHEMANAGER_H
