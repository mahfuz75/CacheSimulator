#include <cstdlib>
#include "Block.h"

#ifndef SET_H
#define SET_H


class Set
{
    public:
        Set(int assoc);
        virtual ~Set();

        void ResetBlocks(int assoc){
            BLOCKS = (Block *)malloc(sizeof(Block) * assoc);
            for(int i=0;i<assoc;i++){
                BLOCKS[i] = Block();
            }
        }

    protected:

    private:
        Block *BLOCKS;
};

#endif // SET_H
