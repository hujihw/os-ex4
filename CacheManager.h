// CacheManager.h

#ifndef EX4_CACHEMANAGER_H
#define EX4_CACHEMANAGER_H

#include <unordered_map>
#include <map>
#include <string>
#include <functional>
#include "CacheBlock.h"

typedef std::unordered_map<std::hash<char*>, CacheBlock> blocks_map;
typedef std::map<std::string, blocks_map> cache_chain;



class CacheManager {


    cache_chain chain;
};


#endif //EX4_CACHEMANAGER_H
