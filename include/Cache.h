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
//enum ReplacementPolicies {
//    LRU,
//    FIFO,
//    Optimal
//};
#define non_inclusive 0
#define inclusive 1
#define exclusive 2
//enum InclusionProperty{
//    non_inclusive,
//    inclusive,
//    exclusive
//};
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

        /*unsigned int GetLEVEL() { return LEVEL; }
        void SetLEVEL(unsigned int val) { LEVEL = val; }

        unsigned int GetSIZE() { return SIZE; }
        void SetSIZE(unsigned int val) { SIZE = val; }

        unsigned int GetASSOC() { return ASSOC; }
        void SetASSOC(unsigned int val) { ASSOC = val; }

        unsigned int GetBLOCKSIZE() { return BLOCKSIZE; }
        void SetBLOCKSIZE(unsigned int val) { BLOCKSIZE = val; }*/

        unsigned int GetBlockOffsetBitWidth() { return BLOCKSIZE == 0 ? 0 : log2(BLOCKSIZE);}
        unsigned int GetNumberOfSets() { return ASSOC == 0 ? 0 : SIZE / (ASSOC * BLOCKSIZE); }
        unsigned int GetIndexBitWidth() { return NUMBER_OF_SETS == 0 ? 0 : log2(NUMBER_OF_SETS); }
        unsigned int GetTagBitWidth() { return MAX_ADDRESS_SIZE - INDEX_BIT_WIDTH - BLOCK_OFFSET_BIT_WIDTH; }

        unsigned int GetTagBits(unsigned int address) { return address >> (MAX_ADDRESS_SIZE - TAG_BIT_WIDTH); }
        unsigned int GetIndexBits(unsigned int address) { if (INDEX_BIT_WIDTH == 0) return 0; return (address << TAG_BIT_WIDTH) >> (TAG_BIT_WIDTH + BLOCK_OFFSET_BIT_WIDTH); }

        //unsigned int GetAddress(unsigned int tag, unsigned int index) { return (tag << (INDEX_BIT_WIDTH + BLOCK_OFFSET_BIT_WIDTH)) | (index << BLOCK_OFFSET_BIT_WIDTH); }

        /*unsigned int Read(unsigned int tag, unsigned int index){
            ACCESSES++;
            READS++;
            unsigned int way_num = LookUp(tag, index);
            if(way_num < 0){
                READ_MISSES++;
            }
            return way_num;
        }*/

        /*void UpdateRankArray(int result, unsigned int index, unsigned int way, unsigned int rank_val){
            if(REPLACEMENT_POLICY == FIFO){
                if(result == MISS){
                    SETS[index].RANK_ARRAY[way] = rank_val;
                }
            }
        }*/

        /*unsigned int Evict(unsigned int index){
            //unsigned int way_to_place = GetWayToPlace(index);
            if(SETS[index].BLOCKS[way_to_place].VALID_BIT == VALID){
                unsigned int address = GetAddress(SETS[index].BLOCKS[way_to_place].TAG, index)
                if(SETS[index].BLOCKS[way_to_place].DIRTY_BIT == DIRTY){
                    //write first
                    WRITE_BACKS++;
                    //Write to next level
                }
                else{
                    if(INCLUSION_PROPERTY == exclusive){
                        //Write to next level
                    }
                }
            }
            //return way_to_place;
        }*/

        /*unsigned int GetWayToPlace(unsigned int index){
            // If there is any invalid block
            for(unsigned int i=0; i<ASSOC; i++){
                if(SETS[index].BLOCKS[i].VALID_BIT == INVALID)
                    return i;
            }
            if(REPLACEMENT_POLICY == Optimal){
                unsigned int r = 0;
                for (unsigned int i = 0; i < ASSOC; i++)
                    if (SETS[index].RANK_ARRAY[i] > SETS[index].RANK_ARRAY[r])
                        r = i;
                return r;
            }
            else{   //For LRU and FIFO
                unsigned int r = 0;
                for (unsigned int i = 0; i < ASSOC; i++)
                    if (SETS[index].RANK_ARRAY[i] < SETS[index].RANK_ARRAY[r])
                        r = i;
                return r;
            }
        }*/

        /*unsigned int LookUp(unsigned int tag, unsigned int index){
            for(int i=0; i<ASSOC; i++){
                if(SETS[index].BLOCKS[i].VALID_BIT == VALID && SETS[index].BLOCKS[i].TAG == tag)
                    return i;
            }
            return -1;
        }*/

        /*ReplacementPolicies GetREPLACEMENT_POLICY() { return REPLACEMENT_POLICY; }
        void SetREPLACEMENT_POLICY(ReplacementPolicies val) { REPLACEMENT_POLICY = val; }

        /*void ResetCache() {
            MEMORY_SPACE.resize(SIZE);
            for(int i = 0; i < SIZE; i++)
                MEMORY_SPACE.push_back(-1);
        }*/
        void ResetCache(){
            SETS = (Set *)malloc(sizeof(Set) * this->NUMBER_OF_SETS);
            for (unsigned int i=0;i<NUMBER_OF_SETS;i++){
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

        //void Read(Cache cache, unsigned int address)

    protected:

    private:

        //std::vector<int> MEMORY_SPACE;


};

#endif // CACHE_H
