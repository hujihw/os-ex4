// CacheManager.cpp

#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include "CacheManager.h"

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
        CacheBlock* nullBlock = new CacheBlock(-1, -1, nullptr, nullptr);
        cacheChain.push_front(nullBlock);
    }

    // assigns the bounds iterators
    int i = 1;
    for (CacheChain::iterator it = cacheChain.begin(); it != cacheChain.end()
            ; it++){
        if (i == newSectionSize + 1){
            middleSectionIter = it;
        }
        if (i == newSectionSize + middleSectionSize + 1){
            oldSectionIter = it;
        }
        i++;
    }

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

    if ((*blockIter)->getSection() != newSection){
//         increment the blocks refcount
        (*blockIter)->incrementRefCount();
    }

    // correct the relevant bounds and update the new bounds section attribute
    switch ((*blockIter)->getSection()){

        case newSection:
            break;

        case middleSection:

            middleSectionIter--;
            (*middleSectionIter)->setSection(middleSection);
            break;

        case oldSection:

            middleSectionIter--;
            (*middleSectionIter)->setSection(middleSection);
            oldSectionIter--;
            (*oldSectionIter)->setSection(oldSection);
            break;
    }

    // move the block to the head and update its section attribute to newSection
    cacheChain.push_front(*blockIter); //put at head
    (*cacheChain.begin())->setSection(newSection); // set the section attribute to newSection
    // reset the iterator in the map to point at the block
    blocksMap[blockID] = cacheChain.begin();

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
 * @brief Insert a new block to the cache.
 */
void CacheManager::insertBlock(int fileId, int blockNumber, const char *buff,
                               char *path) {

    // find first the block and if it exists print the error and exit
    if ((findBlock(BlockID(fileId, blockNumber)) != cacheChain.end())){
        std::cerr<<"error: the block is already in the cache- can't insert "
                "it"<<std::endl;
        exit(1);
    }

    CacheBlock* block = new CacheBlock(fileId, blockNumber, buff, path);
    // put the new block at the top and update it in the map
    cacheChain.emplace_front(block);
    blocksMap[block->getBlockId()] = cacheChain.begin();

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
        blocksMap.erase((*eraseCandidateBlockIter)->getBlockId());
        cacheChain.erase(eraseCandidateBlockIter);
    }
    // correct both boundaries and update the new bounds section attribute
    middleSectionIter--;
    (*middleSectionIter)->setSection(middleSection);
    oldSectionIter--;
    (*oldSectionIter)->setSection(oldSection);
}

/**
 * @brief update the path of files in the given path prefix
 */
void CacheManager::updatePaths(const char* pathPrefix, const char * newPathPrefix) {

    // if pathPrefix have no '/', add it
    std::string pathPrefixStr(pathPrefix);
    if (pathPrefixStr[pathPrefixStr.length()-1] != '/'){
        pathPrefixStr.push_back('/');
    }

    for (CacheChain::iterator it = cacheChain.begin(); it !=
                                                       cacheChain.end(); it++){

        if ((*it)->getFileId() == NULL_BLOCK){
            continue;
        }

        // for each block, if path doesn't have '/' suffix - add it only for the comparison
        std::string pathStr(it.operator*()->getPath());
        if (pathStr[pathStr.length()-1] != '/'){
            pathStr.push_back('/');
        }

        // if prefix - replace the prefix with newPrefix (without '/')
        if (pathStr.compare(0, pathPrefixStr.size(), pathPrefixStr) == 0){
            std::string newPrefixStr = newPathPrefix;
            std::string suffix = pathStr.substr(pathPrefixStr.length(),
                                pathStr.length() - pathPrefixStr.length());
            if (newPrefixStr[newPrefixStr.length()-1] != '/' && suffix[0] != '/'){
                newPrefixStr.push_back('/');
            }

            std::string resPath = newPrefixStr.append(suffix);

            if (resPath[resPath.length()-1] == '/'){
                resPath.pop_back();
            }

//            pathStr.replace(0, newPrefixStr.length(), newPrefixStr);
            (*it)->setPath((char *) resPath.data());
        }
    }
}

/**
 * @brief prints the cache blocks from top to bottom
 */
std::string CacheManager::cacheToString() {
    std::stringstream cacheStrStream;
    for (CacheChain::reverse_iterator it = cacheChain.rbegin(); it != cacheChain
            .rend(); it++) {
        if ((*it)->getBlockNumber() != NULL_BLOCK) {
            std::string pathStr = (*it)->getPath();
            std::string path = pathStr.erase(0, 1);
            cacheStrStream<<(path)<<" "<<(std::to_string((*it)->
                    getBlockNumber() + 1))<<" "
            <<std::to_string((*it)->getRefCount())<<std::endl;
        }
    }
    return cacheStrStream.str();
}
