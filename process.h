#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <string>
#include "defin.h"

using namespace std;

struct process
{
    pid_t pid;
    string pname;
    unsigned int cnt;
    unsigned flist[20];
    bool valid;
};

#endif // PROCESS_H_INCLUDED
