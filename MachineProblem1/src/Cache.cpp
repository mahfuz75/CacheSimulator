#include <string>
#include "Cache.h"

Cache::Cache(unsigned int SIZE=32*1024, unsigned int ASSOC=1, unsigned int BLOCKSIZE=4, std::string REPLACEMENT_POLICY="LRU")
{
    //ctor
    this->SIZE = SIZE;
    this->ASSOC = ASSOC;
    this->BLOCKSIZE = BLOCKSIZE;
    this->REPLACEMENT_POLICY = REPLACEMENT_POLICY;
}

Cache::~Cache()
{
    //dtor
}
