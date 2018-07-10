#ifndef _SUPERBLOCK_H
#define _SUPERBLOCK_H

#include "defin.h"

using namespace std;

struct SuperBlock
{
    unsigned short stacklength = 0;
    bid_t superStack[groupsize];
    SuperBlock()
    {
        memset(this->superStack, 0, sizeof(superStack));
    }
};

#endif // _SUPERBLOCK_H
