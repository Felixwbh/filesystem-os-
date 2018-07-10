#ifndef _DEFIN_H
#define _DEFIN_H

#include <iostream>

#define blocksize 512
#define buffsize 32*1024*1024
#define blocknum 64*1024
#define mbrblock 0
#define SUPERBLOCK 1
#define SUPERINODE 2
#define rootinode 3
#define mininode 4
#define maxinode 130
#define mindata 131
#define maxdata blocknum-1
#define groupsize 255
#define blockcnt 10 + 256 -1
#define rootid 0
#define SYSOPENFILE 40
typedef unsigned short bid_t;

/*
磁盘共32*1024*1024字节
每块512字节
共65536块
一个超级块512字节 栈深2字节 空闲块号 255*2字节
索引节点区
*/

#endif // _DEFIN_H
