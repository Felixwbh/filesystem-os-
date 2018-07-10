#ifndef SHOWSYSTEM_H_INCLUDED
#define SHOWSYSTEM_H_INCLUDED

#include "defin.h"
#include "filesystem.h"

bool showmenu(Inode &nowinode, fstream &myfile);
void showprivilege(const char privilege, char* result);
void getlastseg(char* cmd, int length, char* dirname, int &dirnamecnt);
bool getprocessid(char* cmd, int length, pid_t pid);


#endif // SHOWSYSTEM_H_INCLUDED
