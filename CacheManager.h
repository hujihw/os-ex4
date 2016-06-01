// CacheManager.h

#ifndef EX4_CACHEMANAGER_H
#define EX4_CACHEMANAGER_H

#include <unordered_map>
#include <list>
#include <string>
#include <functional>
#include "CacheBlock.h"

typedef std::pair<char*, int> BlockID;
typedef std::unordered_map<std::hash<BlockID>, CacheBlock> blocks_map;
typedef std::list<CacheBlock> cache_chain;

class CacheManager {
    // functions to implement

    /**
     * @brief Find the given block in cache
     */
    CacheBlock *findBlock(BlockID *blockID); // todo should be (void *)?

    /**
     * @brief Retrieve a block from the cache (get it's value), should use findBlock.
     */
    CacheBlock *retrieveBlock(BlockID *blockID);

    /**
     * @brief Insert a new block to the cache. Should use moveToHead().
     */
    void insertBlock(BlockID *blockID, char *buf);

    /**
     * @brief Remove a block from the cache. Should use removeTail().
     */
    void removeBlock(BlockID *blockID);

    /**
     * @brief Move a block to the head of the list, and change relevant pointers.
     */
    void moveToHead(BlockID *blockID);

    /**
     * @brief Remove the block at the tail of the list, and change relevant pointers.
     */
    void removeTail();

    // data members
    cache_chain chain;
    CacheBlock *newBound;
    CacheBlock *oldBound;
};


#endif //EX4_CACHEMANAGER_H
