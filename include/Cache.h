#include <stdio.h>

#include <cmath>
#include <cstdlib>
#include "Set.h"
#ifndef CACHE_H
#define CACHE_H

#define MAX_ADDRESS_SIZE 32

#define HIT 1
#define MISS 0

#define VALID 1
#define INVALID 0

#define CLEAN 0
#define DIRTY 1

#define LRU 0
#define FIFO 1
#define Optimal 2

#define non_inclusive 0
#define inclusive 1
#define exclusive 2

class Cache
{
    public:
        Cache(unsigned int LEVEL, unsigned int SIZE, unsigned int ASSOC, unsigned int BLOCKSIZE, unsigned int POLICY, unsigned int PROPERTY);//, std::string WRITE_POLICY);
        virtual ~Cache();

        unsigned int LEVEL;
        unsigned int SIZE;
        unsigned int ASSOC;
        unsigned int BLOCKSIZE;

        unsigned int REPLACEMENT_POLICY;
        unsigned int INCLUSION_PROPERTY;

        unsigned int NUMBER_OF_SETS;
        unsigned int BLOCK_OFFSET_BIT_WIDTH;
        unsigned int TAG_BIT_WIDTH;
        unsigned int INDEX_BIT_WIDTH;

        Set *SETS;

        unsigned int ACCESSES;
        unsigned int READS;
        unsigned int WRITES;
        unsigned int READ_MISSES;
        unsigned int WRITE_MISSES;
        unsigned int WRITE_BACKS;
        unsigned int BLOCK_TRANSFERS;

        unsigned int GetBlockOffsetBitWidth() { return BLOCKSIZE == 0 ? 0 : log2(BLOCKSIZE);}
        unsigned int GetNumberOfSets() { return ASSOC == 0 ? 0 : SIZE / (ASSOC * BLOCKSIZE); }
        unsigned int GetIndexBitWidth() { return NUMBER_OF_SETS == 0 ? 0 : log2(NUMBER_OF_SETS); }
        unsigned int GetTagBitWidth() { return MAX_ADDRESS_SIZE - INDEX_BIT_WIDTH - BLOCK_OFFSET_BIT_WIDTH; }

        unsigned int GetTagBits(unsigned int address) { return address >> (MAX_ADDRESS_SIZE - TAG_BIT_WIDTH); }
        unsigned int GetIndexBits(unsigned int address) { return (address << TAG_BIT_WIDTH) >> (TAG_BIT_WIDTH + BLOCK_OFFSET_BIT_WIDTH); }

        void ResetCache(){
            SETS = (Set *)malloc(sizeof(Set) * this->NUMBER_OF_SETS);
            for(unsigned int i=0;i<NUMBER_OF_SETS;i++){
                SETS[i] = Set(ASSOC);
            }
            ACCESSES = 0;
            READS = 0;
            WRITES = 0;
            READ_MISSES = 0;
            WRITE_MISSES = 0;
            WRITE_BACKS = 0;
            BLOCK_TRANSFERS = 0;
        }

    protected:

    private:
};

#endif // CACHE_H
