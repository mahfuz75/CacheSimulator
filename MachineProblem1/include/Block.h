#ifndef BLOCK_H
#define BLOCK_H


class Block
{
    public:
        Block();
        virtual ~Block();

        void ResetBlock(){
            TAG = 0;
            DIRTY = 0;
            VALID = 0;
        }

    protected:

    private:
        int TAG;
        char DIRTY;
        char VALID;
};

#endif // BLOCK_H
