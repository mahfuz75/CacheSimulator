#include <cstdlib>
#include <cstring>
#include "Block.h"

#ifndef SET_H
#define SET_H


class Set
{
    public:
        Set(unsigned int assoc);
        virtual ~Set();

        Block *BLOCKS;
        unsigned int *RANK_ARRAY;

        void ResetBlocks(unsigned int assoc){
            BLOCKS = (Block *)malloc(sizeof(Block) * assoc);
            for(int i=0;i<assoc;i++){
                BLOCKS[i] = Block();
            }
        }
        void ResetRankArray(unsigned int assoc){
            RANK_ARRAY = (unsigned int *)malloc(sizeof(unsigned int) * assoc);
            memset(RANK_ARRAY, 0, sizeof(unsigned int) * assoc);
        }

    protected:

    private:

};

#endif // SET_H
