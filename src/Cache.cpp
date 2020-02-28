#include <string>
#include "Cache.h"


Cache::Cache(unsigned int LEVEL=1, unsigned int SIZE=1024, unsigned int ASSOC=1, unsigned int BLOCKSIZE=4, unsigned int POLICY=LRU, unsigned int PROPERTY=non_inclusive)
{
    //ctor
    this->LEVEL = LEVEL;
    this->SIZE = SIZE;
    this->ASSOC = ASSOC;
    this->BLOCKSIZE = BLOCKSIZE;
    this->REPLACEMENT_POLICY = POLICY;
    this->INCLUSION_PROPERTY = PROPERTY;

    this->BLOCK_OFFSET_BIT_WIDTH = this->GetBlockOffsetBitWidth();
    this->NUMBER_OF_SETS = this->GetNumberOfSets();
    this->INDEX_BIT_WIDTH = this->GetIndexBitWidth();
    this->TAG_BIT_WIDTH = this->GetTagBitWidth();

    ResetCache();
}

Cache::~Cache()
{
    //dtor
}
