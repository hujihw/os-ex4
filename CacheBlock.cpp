// CacheBlock.cpp

#include "CacheBlock.h"

CacheBlock::CacheBlock(char *fileName, int blockNum) : count(1),
                                                       fileName(fileName),
                                                       blockNumber(blockNum){ }

int CacheBlock::getCount() const {
    return count;
}

void CacheBlock::raiseCount() {
    count += 1;
}