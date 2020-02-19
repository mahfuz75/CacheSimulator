#include <vector>
#ifndef CACHE_H
#define CACHE_H

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

        unsigned int GetLEVEL() { return LEVEL; }
        void SetLEVEL(unsigned int val) { LEVEL = val; }

        unsigned int GetSIZE() { return SIZE; }
        void SetSIZE(unsigned int val) { SIZE = val; }

        unsigned int GetASSOC() { return ASSOC; }
        void SetASSOC(unsigned int val) { ASSOC = val; }

        unsigned int GetBLOCKSIZE() { return BLOCKSIZE; }
        void SetBLOCKSIZE(unsigned int val) { BLOCKSIZE = val; }

        unsigned int GetNumberOfSets() { return SIZE / (ASSOC * BLOCKSIZE) ; }

        ReplacementPolicies GetREPLACEMENT_POLICY() { return REPLACEMENT_POLICY; }
        void SetREPLACEMENT_POLICY(ReplacementPolicies val) { REPLACEMENT_POLICY = val; }

        void ResetCache() {
            MEMORY_SPACE.resize(SIZE);
            for(int i = 0; i < SIZE; i++)
                MEMORY_SPACE.push_back(-1);
        }

        //void Read(Cache cache, int address)

    protected:

    private:
        int LEVEL;
        int SIZE;
        int ASSOC;
        int BLOCKSIZE;
        std::vector<int> MEMORY_SPACE;
        ReplacementPolicies REPLACEMENT_POLICY;
        InclusionProperty INCLUSION_PROPERTY;
};

#endif // CACHE_H
