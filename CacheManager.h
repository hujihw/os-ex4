// CacheManager.h

#ifndef EX4_CACHEMANAGER_H
#define EX4_CACHEMANAGER_H

#include <unordered_map>
#include <list>
#include <string>
#include <functional>
#include <string.h>
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

typedef std::list<CacheBlock *> CacheChain;

typedef std::unordered_map<BlockID, CacheChain::iterator> BlocksMap;

class CacheManager {

public:
    /**
     * @brief constructor for the CacheManager class.
     */
    CacheManager(int numberOfBlocks, double fOld, double fNew);

    /**
     * @brief destructor for the CacheManager class.
     */
    ~CacheManager();

    /**
     * @brief Retrieve a block's buffer from the cache
     * returns nullptr if the block was not found
     */
    const char * retrieveBuffer(BlockID blockID);

    /**
     * @brief Retrieve a block's file id from the cache
     * returns zero if the block was not found
     */
    char * retrieveFilePath(BlockID blockID);

    /**
     * @brief constructs a new block and adds it to the cache.
     */
    void insertBlock(int fileDesc, int blockNumber, const char *buff, char *path);

    /**
     * @brief returns an iterator to the end of the cache. for checking if
     * the find method was successful
     */
    CacheChain::iterator getCacheEnd();

    void updatePaths(const char* pathPrefix, const char * newPathPrefix);

    CacheChain cacheChain; //todo move to private
private:
    /**
     * @brief Find the given block in cache
     * returns cacheChain.end() if the block was not found
     */
    CacheChain::iterator findBlock(BlockID blockID);

    // data members
    int numberOfBlocks;
    BlocksMap blocksMap;

    CacheChain::iterator middleSectionIter; // the first block in the middle section
    CacheChain::iterator oldSectionIter; // the first block in te old section
};

#endif //EX4_CACHEMANAGER_H
