#ifndef _SFD_H
#define _SFD_H

#include "defin.h"
#include <cstdio>

using namespace std;

struct SFD
{
    char name[30];
    bid_t inodeid;

    SFD()
    {
        memset(this->name, 0, sizeof(SFD));
    }
};

#endif  //_SFD_H
