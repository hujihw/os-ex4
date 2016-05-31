// CacheManager.h

#ifndef EX4_CACHEMANAGER_H
#define EX4_CACHEMANAGER_H

#include <unordered_map>
#include <list>
#include <string>
#include <functional>
#include "CacheBlock.h"

typedef std::unordered_map<std::hash<char*>, CacheBlock> blocks_map; // todo what is the exact type of the buffers used as keys?
typedef std::list<CacheBlock> cache_chain;

class CacheManager {
    cache_chain chain;

    // functions to implement

    /**
     * @brief Find the given block in cache
     */
    CacheBlock *findBlock(); // todo should be (void *)?

    /**
     * @brief Retrieve a block from the cache (get it's value), should use findBlock.
     */
    CacheBlock *retrieveBlock();

    /**
     * @brief Insert a new block to the cache. Should use moveToHead().
     */
    void insertBlock();

    /**
     * @brief Remove a block from the cache. Should use removeTail().
     */
    void removeBlock();

    /**
     * @brief Move a block to the head of the list, and change relevant pointers.
     */
    void moveToHead();

    /**
     * @brief Remove the block at the tail of the list, and change relevant pointers.
     */
    void removeTail();
};


#endif //EX4_CACHEMANAGER_H
