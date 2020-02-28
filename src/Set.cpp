#include "Set.h"

Set::Set(unsigned int assoc)
{
    //ctor
    ResetBlocks(assoc);
    ResetRankArray(assoc);
}

Set::~Set()
{
    //dtor
}
