#ifndef BLOCK_H
#define BLOCK_H


class Block
{
    public:
        Block();
        virtual ~Block();

    protected:

    private:
        int TAG;
        char DIRTY;
        char VALID;
};

#endif // BLOCK_H
