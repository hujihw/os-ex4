// CacheManager.h

#ifndef EX4_CACHEMANAGER_H
#define EX4_CACHEMANAGER_H

#include <unordered_map>
#include <list>
#include <string>
#include <functional>
#include "CacheBlock.h"


template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
    template<typename S, typename T> struct hash<pair<S, T>>
    {
        inline size_t operator()(const pair<S, T> & v) const
        {
            size_t seed = 0;
            ::hash_combine(seed, v.first);
            ::hash_combine(seed, v.second);
            return seed;
        }
    };
}

// the file descriptor and the block number
typedef std::pair<int, int> BlockID;

typedef std::list<CacheBlock *> CacheChain; // TODO check necessity of *

typedef std::unordered_map<BlockID, CacheChain::iterator> BlocksMap;

class CacheManager {

public:
    /**
     * @brief constructor for the CacheManager class.
     */
    CacheManager(int numberOfBlocks, int blockSize, int fOld, int fNew);

    /**
     * @brief destructor for the CacheManager class.
     */
    ~CacheManager();

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
    void insertBlock(int fileDesc, int blockNumber, char *buff);


    /**
     * @brief Move a block to the head of the list, and change relevant pointers.
     */
//    void moveToHead(BlockID blockID);


private:
    // data members
    int numberOfBlocks;
    int blockSize;
    BlocksMap blocksMap;
    CacheChain cacheChain;
    CacheChain::iterator middleSection; // the first block in the middle section
    CacheChain::iterator oldSection; // the first block in te old section
};



#endif //EX4_CACHEMANAGER_H
