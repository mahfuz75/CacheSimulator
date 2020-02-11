#ifndef CACHE_H
#define CACHE_H


class Cache
{
    public:
        Cache(unsigned int SIZE, unsigned int ASSOC, unsigned int BLOCKSIZE, std::string REPLACEMENT_POLICY);
        virtual ~Cache();

        unsigned int GetSIZE() { return SIZE; }
        void SetSIZE(unsigned int val) { SIZE = val; }
        unsigned int GetASSOC() { return ASSOC; }
        void SetASSOC(unsigned int val) { ASSOC = val; }
        unsigned int GetBLOCKSIZE() { return BLOCKSIZE; }
        void SetBLOCKSIZE(unsigned int val) { BLOCKSIZE = val; }
        unsigned int GetNumberOfSets() { return SIZE / (ASSOC * BLOCKSIZE) ; }
        std::string GetREPLACEMENT_POLICY() { return REPLACEMENT_POLICY; }
        void SetREPLACEMENT_POLICY(std::string val) { REPLACEMENT_POLICY = val; }

    protected:

    private:
        unsigned int SIZE;
        unsigned int ASSOC;
        unsigned int BLOCKSIZE;
        std::string REPLACEMENT_POLICY;
};

#endif // CACHE_H
