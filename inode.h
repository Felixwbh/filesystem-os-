#ifndef _INODE_H
#define _INODE_H

#include "defin.h"
#include <string>
#include <ctime>

using namespace std;

struct Inode  //100
{
    char privilege;  //1
    char name[30];    //30
    unsigned int linknum;  //4
    unsigned short uid;    //2
    unsigned int filesize; //4
    time_t ctime;          //4
    time_t atime;          //1
    unsigned int iblock;   //4
    unsigned short ibytes; //2
    bid_t inodeid;         //2
    bid_t parent;          //2
    unsigned int data[11]; //44

    Inode()
    {
        memset(&this->privilege, 0, sizeof(Inode));
    }
};

struct InodeMem
{
    Inode inode;
    unsigned int cnt;
    bid_t plist[SYSOPENFILE];
    bool valid;
    bool xlock;
    bid_t xpid;

    InodeMem()
    {
        valid = false;
        cnt = 0;
        xlock = false;
    }

    InodeMem(const Inode& i)
    {
        valid = true;
        memcpy((char*)&inode, (char*)&i, sizeof(Inode));
        cnt = 0;
        xlock = false;
    }
};

#endif // _INODE_H
