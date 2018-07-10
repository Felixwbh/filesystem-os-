#include "showsystem.h"
#include "inode.h"
#include "filesystem.h"
#include "usersystem.h"

using namespace std;

bool showmenu(Inode &nowinode,int &uid ,fstream &myfile)
{
    if(!getinode(nowinode.inodeid, &nowinode, myfile))
        return false;
    char dir[128];
    getdir(nowinode, dir, myfile);
    char username[32];
    if(!getusername(uid, username))
        return false;
    cout << "username:" << " " << username << "-------" << endl;
    return true;
}

void showprivilege(const char privilege, char* result)
{
    if(privilege & 1<<7) result[0] = 'd';
    if(privilege & 1<<6) result[1] = 'r';
    if(privilege & 1<<5) result[2] = 'w';
    if(privilege & 1<<4) result[3] = 'x';
    if(privilege & 1<<3) result[4] = 'r';
    if(privilege & 1<<2) result[5] = 'w';
    if(privilege & 1<<1) result[6] = 'x';
    if(privilege & 1) result[7] = 's';
}

void getlastseg(char* cmd, int length, char* dirname, int &dirnamecnt)
{
    if(cmd[length - 1] == '/')
    {
        while(cmd[length - 1] == '/')
            length--;
    }
    for(int i = length - 1; i >= 0; i--)
    {
        if(cmd[i] == '/')
            break;
        dirname[dirnamecnt++] = cmd[i];
    }
    for(int i = 0; i <= (dirnamecnt - 1) / 2; i++)
        swap(dirname[i], dirname[dirnamecnt - 1 - i]);
    dirname[dirnamecnt] = '\0';
}

bool getprocessid(char* cmd, int length, pid_t pid)
{
    for(int i = 0; i < length; i++)
    {
        if(cmd[i] < '0' && cmd[i] > '9')
            return false;
        pid = pid * 10 + cmd[i] - '0';
    }
    return true;
}












