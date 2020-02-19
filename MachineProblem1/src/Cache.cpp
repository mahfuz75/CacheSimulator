#include <string>
#include "Cache.h"

Cache::Cache(int LEVEL=1, int SIZE=1024, int ASSOC=1, int BLOCKSIZE=4, ReplacementPolicies POLICY=LRU, InclusionProperty PROPERTY=non_inclusive)
{
    //ctor
    this->LEVEL = LEVEL;
    this->SIZE = SIZE;
    this->ASSOC = ASSOC;
    this->BLOCKSIZE = BLOCKSIZE;
    this->REPLACEMENT_POLICY = POLICY;
    this->INCLUSION_PROPERTY = PROPERTY;
    ResetCache();
}

Cache::~Cache()
{
    //dtor
}
