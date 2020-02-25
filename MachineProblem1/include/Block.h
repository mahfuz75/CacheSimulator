#ifndef BLOCK_H
#define BLOCK_H


class Block
{
    public:
        Block();
        virtual ~Block();

        unsigned int TAG;
        bool DIRTY_BIT;
        bool VALID_BIT;

        void ResetBlock(){
            TAG = 0;
            DIRTY_BIT = 0;
            VALID_BIT = 0;
        }

    protected:

    private:

};

#endif // BLOCK_H
