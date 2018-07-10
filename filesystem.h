#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include "defin.h"
#include "MBR.h"
#include "inode.h"
#include "superblock.h"
#include "SFD.h"


bool create();
bool readblock(bid_t blockID, char *buffer, int length, fstream &myfile);
bool writeblock(bid_t blockID, char *buffer, int length,fstream &myfile);
bool exist();
bool load(fstream &myfile);
void testtime();
bool loadsuperblock(bid_t superblockid, fstream &myfile, SuperBlock &superblock);
bool savesuperblock(bid_t superblockid, fstream &myfile, SuperBlock &superblock);
bool recycleblock(bid_t blockid, fstream &myfile, SuperBlock &superblock);
bool assignblock(bid_t* blockid, fstream &myfile, SuperBlock &superblock);
bool format(fstream &myfile, SuperBlock &superblock, SuperBlock &superinode);
bool isformat(fstream &myfile);
void testformat(fstream &myfile, SuperBlock &superblock, SuperBlock &superinode);
bool getblockid(const Inode &inode, int offset, bid_t* result, fstream &myfile);
bool readfile(const Inode &inode, int start, int length, char* buffer, fstream &myfile);
bool getcontent(const Inode &inode, SFD* result, fstream &myfile);
bool findcontent(const SFD* directory, const int length, const char* name, int* result, fstream &myfile);
bool findpath(const Inode &inode, char* path, bool last, Inode* result, bool followlast, fstream &myfile);
bool addblock(Inode &inode, bid_t blockcount, fstream &myfile, SuperBlock &superblock);
bool getinode(bid_t inodeid, Inode* result, fstream &myfile);
bool saveinode(bid_t inodeid, const Inode &inode, fstream &myfile);
bool writefile(Inode &inode, bid_t start, bid_t length, char* buffer, fstream &myfile, SuperBlock &superblock);
bool setSFDlist(Inode &inode, bid_t parentid, fstream &myfile, SuperBlock &superblock);
bool rootdir(fstream &myfile, SuperBlock &superblock);
bool createfile(Inode &inode, char* name, char privilege, int uid, Inode* result, fstream &myfile, SuperBlock &superinode, SuperBlock &superblock);
bool createsondir(Inode &inode, char* name, char privilege, int uid, Inode *result, fstream &myfile, SuperBlock &superinode, SuperBlock &superblock);
void getdir(const Inode &inode, char* dir, fstream &myfile);
bool deletedirectblock(const Inode &inode, fstream &myfile, SuperBlock &superblock);
bool deleteindexblock(const Inode &inode, fstream &myfile, SuperBlock &superblock);
bool deleteSFD(const Inode &inode, fstream &myfile);
bool addSFD(Inode &parentinode, const SFD &newsfd, fstream &myfile, SuperBlock &superblock);
bool deletefile(const Inode& inode, fstream &myfile, SuperBlock &superblock, SuperBlock &superinode);
bool deletedir(const Inode &inode, fstream &myfile, SuperBlock &superblock, SuperBlock &superinode);
bool changeprivilege(Inode &inode, char privililege, fstream &myfile);
bool copyfile(const Inode &from, Inode &to, char* name, int uid, fstream &myfile, SuperBlock &superblock, SuperBlock &superinode);
bool copydir(const Inode &from, Inode &to, char* name, int uid,fstream &myfile, SuperBlock &superblock, SuperBlock &superinode);
bool ccopy(const Inode &from, Inode &to, char* name, int uid,fstream &myfile, SuperBlock &superblock, SuperBlock &superinode);
bool movefile(Inode &from, Inode &to, char* name, fstream &myfile, SuperBlock &superblock);
bool hardlink(Inode &from, Inode &to, char* name, fstream &myfile, SuperBlock &superinode, SuperBlock &superblock);
bool softlink(char* from, Inode &to, char* name, int uid, fstream &myfile, SuperBlock &superinode, SuperBlock &superblock);

#endif // _FILESYSTEM_H
