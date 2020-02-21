#include <cmath>
#include <cstdlib>
#include "Set.h"
#ifndef CACHE_H
#define CACHE_H

#define MAX_ADDRESS_SIZE 32

enum ReplacementPolicies {
    LRU,
    FIFO,
    Optimal
};
enum InclusionProperty{
    non_inclusive,
    inclusive
};
class Cache
{
    public:
        Cache(int LEVEL, int SIZE, int ASSOC, int BLOCKSIZE, ReplacementPolicies POLICY, InclusionProperty PROPERTY);//, std::string WRITE_POLICY);
        virtual ~Cache();

        /*unsigned int GetLEVEL() { return LEVEL; }
        void SetLEVEL(unsigned int val) { LEVEL = val; }

        unsigned int GetSIZE() { return SIZE; }
        void SetSIZE(unsigned int val) { SIZE = val; }

        unsigned int GetASSOC() { return ASSOC; }
        void SetASSOC(unsigned int val) { ASSOC = val; }

        unsigned int GetBLOCKSIZE() { return BLOCKSIZE; }
        void SetBLOCKSIZE(unsigned int val) { BLOCKSIZE = val; }*/

        int GetBlockOffsetBitWidth() { return BLOCKSIZE == 0 ? 0 : log2(BLOCKSIZE);}
        int GetNumberOfSets() { return SIZE / (ASSOC * BLOCKSIZE); }
        int GetIndexBitWidth() { return NUMBER_OF_SETS == 0 ? 0 : log2(NUMBER_OF_SETS); }
        int GetTagBitWidth() { return MAX_ADDRESS_SIZE - INDEX_BIT_WIDTH - BLOCK_OFFSET_BIT_WIDTH; }

        /*ReplacementPolicies GetREPLACEMENT_POLICY() { return REPLACEMENT_POLICY; }
        void SetREPLACEMENT_POLICY(ReplacementPolicies val) { REPLACEMENT_POLICY = val; }

        /*void ResetCache() {
            MEMORY_SPACE.resize(SIZE);
            for(int i = 0; i < SIZE; i++)
                MEMORY_SPACE.push_back(-1);
        }*/
        void ResetCache(){
            SETS = (Set *)malloc(sizeof(Set) * this->NUMBER_OF_SETS);
            for(int i=0;i<NUMBER_OF_SETS;i++){
                SETS[i] = Set(ASSOC);
                //SETS[i].BLOCKS = (Block *)malloc(sizeof(Block) * ASSOC);
            }
            ACCESSES = 0;
            READS = 0;
            WRITES = 0;
            READ_MISSES = 0;
            WRITE_MISSES = 0;
            WRITE_BACKS = 0;
            BLOCK_TRANSFERS = 0;
        }

        //void Read(Cache cache, int address)

    protected:

    private:
        int LEVEL;
        int SIZE;
        int ASSOC;
        int BLOCKSIZE;
        //std::vector<int> MEMORY_SPACE;

        ReplacementPolicies REPLACEMENT_POLICY;
        InclusionProperty INCLUSION_PROPERTY;

        int NUMBER_OF_SETS;
        int BLOCK_OFFSET_BIT_WIDTH;
        int TAG_BIT_WIDTH;
        int INDEX_BIT_WIDTH;

        Set *SETS;

        int ACCESSES;
        int READS;
        int WRITES;
        int READ_MISSES;
        int WRITE_MISSES;
        int WRITE_BACKS;
        int BLOCK_TRANSFERS;
};

#endif // CACHE_H
