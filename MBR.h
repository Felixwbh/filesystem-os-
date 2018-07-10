#ifndef _MBR_H
#define _MBR_H

#include <string>
#include "defin.h"

using namespace std;

struct MBR
{
    bool formatflag = 0;
    char mbrstring[blocksize-1] = "This is a mbr!";
};

#endif // _MBR_H
