#include "filesystem.h"

using namespace std;

bool create()
{
    ofstream filesystem;
    filesystem.open("test.bin",ios::out|ios::binary);
    if(!filesystem)
        return false;
    char *buff = new char[buffsize];
    memset (buff,0,buffsize);
    filesystem.write(buff,buffsize);
    if(!filesystem)
        return false;
    filesystem.close();
    delete[] buff;
    return true;
}

bool readblock(bid_t blockID, char *buffer, int length, fstream &myfile)
{
    if(blockID < blocknum && length <= blocksize)
    {
        myfile.seekg(blockID * blocksize);
        myfile.read(buffer, length);
        if(!myfile)
            return  false;
        else
            return true;
        return true;

    }
    else
        return false;
}

bool writeblock(bid_t blockID, char *buffer, int length,fstream &myfile)
{
    if(blockID < blocknum && length <= blocksize)
    {
        myfile.seekp(blockID * blocksize);
        myfile.write(buffer , length);
        if(!myfile)
            return false;
        else
            return true;
    }
    else
        return false;
}

bool exist()
{
    ifstream f("test.bin");
    return f.good();
}

bool load(fstream &myfile)
{
    myfile.open("test.bin", ios::in | ios::out | ios::binary);
    return myfile.good();
}

void testtime()
{
    time_t testtime = time(0);
    time_t testtime1 = testtime;
    char* test = ctime(&testtime1);
    cout << test << endl;
}

bool loadsuperblock(bid_t superblockid, fstream &myfile, SuperBlock &superblock)
{
    return readblock(superblockid, (char*)&superblock, sizeof(SuperBlock), myfile);
}

bool savesuperblock(bid_t superblockid, fstream &myfile, SuperBlock &superblock)
{
    return writeblock(superblockid, (char*)&superblock, sizeof(SuperBlock), myfile);
}

bool recycleblock(bid_t blockid, fstream &myfile, SuperBlock &superblock)
{
    if(superblock.stacklength == groupsize)
    {
        if(!savesuperblock(blockid, myfile, superblock))
            return false;
        memset((char*)&superblock, 0 ,sizeof(SuperBlock));
        superblock.stacklength = 1;
    }
    else
        superblock.stacklength++;
    superblock.superStack[superblock.stacklength - 1] = blockid;
    return true;
}

bool assignblock(bid_t* blockid, fstream &myfile, SuperBlock &superblock)
{
    if(superblock.superStack[superblock.stacklength-1] == 0)
        return false;
    *blockid = superblock.superStack[superblock.stacklength-1];
    if(superblock.stacklength == 1)
    {
        if(!loadsuperblock(*blockid, myfile, superblock))
            return false;
    }
    else
        superblock.stacklength--;
    return true;
}

bool format(fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    MBR mbr;
    mbr.formatflag = true;
    if(!writeblock(mbrblock, (char*)&mbr, sizeof(MBR), myfile))
        return false;

    superblock.superStack[0] = 0;
    superblock.stacklength = 1;
    for(int i = mindata; i <= maxdata; i++)
        if(!recycleblock(i, myfile, superblock))
            return false;
    if(!savesuperblock(SUPERBLOCK, myfile, superblock))
        return false;
    superinode.superStack[0] = 0;
    superinode.stacklength = 1;
    for(int i = mininode; i <= maxinode; i++)
        if(!recycleblock(i,myfile, superinode))
            return false;
    if(!savesuperblock(SUPERINODE, myfile, superinode))
        return false;

    if(!rootdir(myfile, superblock))
        return false;
    Inode root;
    char homename[] = "home";
    if(!getinode(rootinode, &root, myfile))
        return false;
    Inode homeinode;
    if(!createsondir(root, homename, (1<<6)|(1<<5)|(1<<4)|(1<<3)|(1<<1), rootid, &homeinode, myfile, superinode, superblock))
        return false;
    cout << "格式化成功" ;
    return true;
}

bool isformat(fstream &myfile)
{
    MBR mbr;
    return readblock(mbrblock, (char*)&mbr, sizeof(MBR), myfile) && mbr.formatflag;
}

void testformat(fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    if(isformat(myfile) == false)
        format(myfile,superblock, superinode);
    else
        cout << "已格式化" ;
}

bool getblockid(const Inode &inode, int offset, bid_t* result, fstream &myfile)
{
    int startblock = offset / blocksize;  //求起始块
    if(startblock <= 9)  //直接索引
        *result = inode.data[startblock];
    else if(startblock <= 265)   //一级索引
    {
        char index[blocksize];
        if(!readblock(inode.data[10], index, blocksize, myfile))
            return false;
        char *blockoffset = index + (startblock -10) * sizeof(bid_t);
        memcpy(result, blockoffset, sizeof(bid_t));
    }
    return true;
}

bool readfile(const Inode &inode, int start, int length, char* buffer, fstream &myfile)
{
    if(start > (int)inode.filesize - 1 || start + length - 1 > (int)inode.filesize - 1)
        return false;
    char* tbuffer = new char[length + 1024];
    int fpointer = start;
    char* bpointer = tbuffer;
    bid_t tblockid;
    int offset = (start + length) % blocksize;
    while (fpointer < start + length + blocksize - offset)
    {
        if(!getblockid(inode, fpointer, &tblockid, myfile))
            return false;
        if(!readblock(tblockid, bpointer, blocksize, myfile))
            return false;
        fpointer += blocksize;
        bpointer += blocksize;
    }
    char* result = tbuffer + (start % blocksize);
    memcpy(buffer, result, length);
    delete[] tbuffer;
    return true;
}

bool getcontent(const Inode &inode, SFD* result, fstream &myfile)
{
    if(!(inode.privilege) & (1<<7))
        return false;
    if(!readfile(inode, 0, inode.filesize, (char*)result, myfile))
        return false;
    return true;
}

bool findcontent(const SFD* directory, const int length, const char* name, int* result, fstream &myfile)
{
    for(int i = 0; i < length; i++)
    {
        if(strcmp(directory[i].name, name) == 0)
        {
            *result = i;
            return true;
        }
    }
    return false;
}

bool findpath(const Inode &inode, char* path, bool last, Inode* result, bool followlast, fstream &myfile)
{
    Inode tempInode;
    Inode presentInode;
    int pp = 0, linknum = 0;
    if(path[0] == '/')
    {
        if(!getinode(rootinode, &presentInode, myfile))
            return false;
        pp++;
    }
    else
        memcpy((char*)&presentInode, (char*)&inode ,sizeof(Inode));
    char buffer[40];
    int bpointer, target;
    while(1)
    {
        bpointer = 0;
        while (path[pp] == '/')
            pp++;
        while (path[pp] != '/')
        {
            if(path[pp] == '\0')
                break;
            buffer[bpointer] = path[pp];
            bpointer++;
            pp++;
        }
        if((!last) && (path[pp] == '\0'))
            break;
        buffer[bpointer] = '\0';
        if(bpointer == 0)
            break;
        int num = presentInode.filesize / sizeof(SFD);
        SFD* dir = new SFD[num];
        if(!getcontent(presentInode, dir, myfile))
        {
            delete[] dir;
            return false;
        }
        if(findcontent(dir, num, buffer, &target, myfile))
        {
            if(!getinode(dir[target].inodeid, &tempInode, myfile))
            {
                delete[] dir;
                return false;
            }
            if((tempInode.privilege & 1) && ((path[pp] != '\0') || followlast))
            {
                linknum++;
                if(linknum > 40)
                    return false;
                char* link = new char[tempInode.filesize + 1];
                link[tempInode.filesize] = '\0';
                if(!readfile(tempInode, 0, tempInode.filesize, link, myfile))
                {
                    delete[] link;
                    return false;
                }
                if(link[0] == '/')
                {
                    if(!readblock(rootinode, (char*)&presentInode, sizeof(Inode), myfile))
                    {
                        delete[] link;
                        return false;
                    }
                }
                int length = strlen(path);
                char* newpath = new char[length + tempInode.filesize + 10];
                int newlength = 0;
                for(int i = 0; i < length; i++)
                {
                    if(path[i] != '\0')
                        newpath[newlength++] = path[i];
                    if (i == pp)
                    {
                        for(int j = 0; j < (int)tempInode.filesize; j++)
                        {
                            newpath[newlength++] = link[j];
                        }
                        newpath[newlength++] = '/';
                    }
                }
                newpath[newlength] = '\0';
                strcpy(path, newpath);
                path[newlength] ='\0';
                delete[] link;
                delete[] newpath;
                continue;
            }
            if(!(tempInode.privilege & (1<<7)) && (path[pp] != '\0'))
                return false;
            memcpy((char*)&presentInode, (char*)&tempInode, sizeof(Inode));
        }
        else
        {
            delete[] dir;
            return false;
        }
        delete[] dir;
    }
    memcpy((char*)result, (char*)&presentInode, sizeof(Inode));
    return true;
}

bool addblock(Inode &inode, bid_t blockcount, fstream &myfile, SuperBlock &superblock)
{
    if(inode.iblock + blockcount > blockcnt)
        return false;
    bid_t datablockid = 0, directblockid = 0, indexblockid = 0;
    char indexbuffer[blocksize];
    bid_t indexbufferloadblockid = 0;
    memset(indexbuffer, 0, blocksize);
    for(int i = (int)inode.iblock; i < (int)inode.iblock + blockcount; i++)
    {
        if(i < 10)
        {
            if(!assignblock(&directblockid, myfile, superblock))
                return false;
            inode.data[i] = directblockid;
        }
        else if (i == 10)
        {
            if(!assignblock(&indexblockid, myfile, superblock))
                return false;
            inode.data[10] = indexblockid;
            indexbufferloadblockid = indexblockid;
            if(!assignblock(&datablockid, myfile, superblock))
                return false;
            memcpy(indexbuffer, (char*)&datablockid, sizeof(bid_t));
            if(!writeblock(indexblockid, indexbuffer, blocksize, myfile))
                return false;
        }
        else if (i < (10 + 256))
        {
            if(indexbufferloadblockid != inode.data[10])
            {
                indexblockid = inode.data[10];
                if(!readblock(indexblockid, indexbuffer, blocksize, myfile))
                    return false;
                indexbufferloadblockid = inode.data[10];
            }
            if(!assignblock(&datablockid, myfile, superblock))
                return false;
            memcpy(indexbuffer + (i-10) * sizeof(bid_t), (char*)&datablockid, sizeof(bid_t));
            if(!writeblock(indexblockid, indexbuffer, blocksize, myfile))
                return false;
        }
    }
    inode.iblock += blockcount;
    if(!writeblock(inode.inodeid, (char*)&inode, sizeof(Inode),myfile))
        return false;
    return true;
}

bool getinode(bid_t inodeid, Inode* result, fstream &myfile)
{
    return readblock(inodeid, (char*)result, sizeof(Inode), myfile);
}

bool saveinode(bid_t inodeid, const Inode &inode, fstream &myfile)
{
    return writeblock(inodeid, (char*)&inode, sizeof(Inode), myfile);
}

bool writefile(Inode &inode, bid_t start, bid_t length, char* buffer, fstream &myfile, SuperBlock &superblock)
{
    if((ceil(((float)start + length) / (float)blocksize) - inode.iblock) > 0 )
    {
        if(!addblock(inode, (ceil(((float)start + length) / (float)blocksize) - inode.iblock), myfile, superblock))
            return false;
    }
    bid_t startblock = start / blocksize ;
    bid_t lastblock = (start + length) / blocksize;
    bid_t presentblock = startblock;
    char* bpointer = buffer;
    bid_t inodeid = 0;
    bid_t fpointer = start;
    char rbuffer[blocksize];
    if(startblock == lastblock)
    {
        if(!getblockid(inode, start, &inodeid, myfile))
            return false;
        if(!readblock(inodeid, rbuffer, blocksize, myfile))
            return false;
        memcpy(rbuffer + start % blocksize, buffer, length);
        if(!writeblock(inodeid, rbuffer, blocksize, myfile))
            return false;
    }
    else
    {
        while(fpointer < start + length + blocksize - lastblock)
        {
            if(!getblockid(inode, fpointer, &inodeid, myfile))
                return false;
            if(!readblock(inodeid, rbuffer, blocksize, myfile))
                return false;
            if(presentblock == startblock)
            {
                int offset = start % blocksize;
                int targetlength = blocksize - offset;
                memcpy(rbuffer + offset, bpointer, targetlength);
                bpointer += targetlength;
            }
            else if (presentblock == lastblock)
            {
                int targelength = (start + length) % blocksize;
                memcpy(rbuffer, bpointer, targelength);
                bpointer += length;
            }
            else
            {
                memcpy(rbuffer, bpointer, blocksize);
                bpointer += blocksize;
            }
            if(!writeblock(inodeid, rbuffer, blocksize, myfile))
                return false;
            fpointer += blocksize;
            presentblock++;
        }
    }
    inode.atime = time(0);
    inode.ctime = time(0);
    if(start + length > inode.filesize)
    {
        inode.filesize = start + length;
        inode.ibytes = lastblock;
    }
    if(!saveinode(inode.inodeid, inode, myfile))
        return false;
    return true;
}

bool setSFDlist(Inode &inode, bid_t parentid, fstream &myfile, SuperBlock &superblock)
{
    SFD sfdlist[2];
    strcpy(sfdlist[0].name, ".");
    sfdlist[0].inodeid = inode.inodeid;
    strcpy(sfdlist[1].name, "..");
    sfdlist[1].inodeid = parentid;
    return writefile(inode, 0, sizeof(SFD) * 2, (char*)sfdlist, myfile, superblock);
}

bool rootdir(fstream &myfile, SuperBlock &superblock)
{
    Inode rootInode;
    rootInode.privilege = (1<<7) | (1<<6) | (1<<5) | (1<<4) | (1<<3) | (1<<1);
    strcpy(rootInode.name, "/");
    rootInode.linknum = 1;
    rootInode.uid = rootid;
    rootInode.filesize = 0;
    rootInode.atime = rootInode.ctime = time(0);
    rootInode.iblock = 0;
    rootInode.ibytes = 0;
    rootInode.parent = rootinode;
    rootInode.inodeid = rootinode;
    if(!setSFDlist(rootInode, rootinode, myfile, superblock))
        return false;
    if(!saveinode(rootinode, rootInode, myfile))
        return false;
    return true;
}

bool createfile(Inode &inode, char* name, char privilege, int uid, Inode* result, fstream &myfile, SuperBlock &superinode, SuperBlock &superblock)
{
    SFD* sfddir = new SFD[inode.filesize / sizeof(SFD)];
    if(!getcontent(inode, sfddir, myfile))
    {
        delete[] sfddir;
        return false;
    }
    int contentresult;
    if(findcontent(sfddir, inode.filesize / sizeof(SFD) , name, &contentresult, myfile))
    {
        delete[] sfddir;
        return false;
    }
    Inode newinode;
    if(!assignblock(&newinode.inodeid, myfile, superinode))
    {
        delete[] sfddir;
        return false;
    }

    newinode.privilege = privilege;
    strcpy(newinode.name, name);
    newinode.linknum = 1;
    newinode.uid = uid;
    newinode.filesize = 0;
    newinode.atime = newinode.ctime = time(0);
    newinode.iblock = 0;
    newinode.ibytes = 0;
    newinode.parent = inode.inodeid;
    if(!saveinode(newinode.inodeid, newinode, myfile))
    {
        delete[] sfddir;
        return false;
    }
    memcpy((char*)result, (char*)&newinode, sizeof(Inode));
    SFD newSFD;
    strcpy(newSFD.name, name);
    newSFD.inodeid = newinode.inodeid;
    if(!writefile(inode, inode.filesize, sizeof(SFD), (char*)&newSFD, myfile, superblock))
    {
        delete[] sfddir;
        return false;
    }
    delete[] sfddir;
    return true;
}

bool createsondir(Inode &inode, char* name, char privilege, int uid, Inode *result, fstream &myfile, SuperBlock &superinode, SuperBlock &superblock)
{
    if(!createfile(inode, name, privilege | (1<<7), uid, result, myfile, superinode, superblock))
        return false;
    if(!setSFDlist(*result, inode.inodeid, myfile, superblock))
        return false;
    return saveinode(inode.inodeid, inode, myfile);
}

void getdir(const Inode &inode, char* dir, fstream &myfile)
{
    char rootname[] = "/";
    if (strcmp(inode.name, rootname) == 0)
    {
        strcpy(dir, rootname);
        return;
    }
    string sdir;
    Inode presentinode;
    memcpy((char*)&presentinode, (char*)&inode, sizeof(Inode));
    while(strcmp(presentinode.name, "/") != 0)
    {
        string presentname(presentinode.name);
        sdir = "/" + presentname + sdir;
        getinode(presentinode.parent, &presentinode, myfile);
    }
    strcpy(dir, sdir.c_str());
    return;
}

bool deletedirectblock(const Inode &inode, fstream &myfile, SuperBlock &superblock)
{
    int maxInode;
    if(inode.iblock > 10)
        maxInode = 10;
    else
        maxInode = inode.iblock;
    for(int i = 0; i < maxInode; i++)
    {
        if(!recycleblock(inode.data[i], myfile, superblock))
            return false;
    }
    return true;
}

bool deleteindexblock(const Inode &inode, fstream &myfile, SuperBlock &superblock)
{
    bid_t indexbuffer[256];
    if(!readblock(inode.data[10], (char*)indexbuffer, blocksize, myfile))
        return false;
    int maxInode;
    if(inode.iblock - 10 > 256)
        maxInode = 256;
    else
        maxInode = inode.iblock - 10;
    for(int i = 0; i < maxInode; i++)
    {
        if(!recycleblock(indexbuffer[i], myfile, superblock))
            return false;
    }
    return recycleblock(inode.data[10], myfile, superblock);
}

bool deleteSFD(const Inode &inode, fstream &myfile, SuperBlock &superblock)
{
    Inode parentinode;
    if(!getinode(inode.parent, &parentinode, myfile))
        return false;
    if(!(parentinode.privilege & (1<<7)))
        return false;
    SFD* sfdlist = new SFD[parentinode.filesize / sizeof(SFD)];
    if(!readfile(parentinode, 0, parentinode.filesize, (char*)sfdlist, myfile))
    {
        delete[] sfdlist;
        return false;
    }
    int newp = 0;
    SFD* newsfdlist = new SFD[parentinode.filesize / sizeof(SFD) - 1];
    bool result = false;
    for(int i = 0; i < (int)(parentinode.filesize / sizeof(SFD)); i++)
    {
        if(strcmp(sfdlist[i].name, inode.name) == 0)
        {
            result = true;
            continue;
        }
        else
        {
            memcpy((char*)&newsfdlist[newp], (char*)&sfdlist[i], sizeof(SFD));
            newp++;
        }
    }
    if(!result)
    {
        delete[] sfdlist;
        delete[] newsfdlist;
        return false;
    }
    if(!writefile(parentinode, 0, parentinode.filesize - sizeof(SFD), (char*)newsfdlist, myfile, superblock))
    {
        delete[] sfdlist;
        delete[] newsfdlist;
        return false;
    }
    parentinode.filesize -= sizeof(SFD);
    if(!saveinode(parentinode.inodeid, parentinode, myfile))
    {
        delete[] sfdlist;
        delete[] newsfdlist;
        return false;
    }
    delete[] sfdlist;
    delete[] newsfdlist;
    return true;
}

bool addSFD(Inode &parentinode, const SFD &newsfd, fstream &myfile, SuperBlock &superblock)
{
    if(!(parentinode.privilege & (1<<7)))
        return false;
    SFD* sfdlist = new SFD[parentinode.filesize / sizeof(SFD)];
    if(!readfile(parentinode, 0, parentinode.filesize, (char*)sfdlist, myfile))
    {
        delete[] sfdlist;
        return false;
    }
    int result = 0;
    if(findcontent(sfdlist, parentinode.filesize / sizeof(SFD), newsfd.name, &result, myfile))
    {
        delete[] sfdlist;
        return false;
    }
    if(!writefile(parentinode, parentinode.filesize, sizeof(SFD), (char*)&newsfd, myfile, superblock))
    {
        delete[] sfdlist;
        return false;
    }
    delete[] sfdlist;
    return true;
}

bool deletefile(const Inode& inode, fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    if(inode.privilege & (1<<7))
        return false;
    if(inode.linknum == 1)
    {
        if(inode.iblock <= 10)
        {
            if(!deletedirectblock(inode, myfile, superblock))
                return false;
        }
        else
        {
            if(!deletedirectblock(inode, myfile, superblock) & deleteindexblock(inode, myfile, superblock))
                return false;
        }
    }

    if(!deleteSFD(inode, myfile, superblock))
        return false;
    if(!recycleblock(inode.inodeid, myfile, superinode))
        return false;
    return true;
}

bool deletedir(const Inode &inode, fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    if(!(inode.privilege & (1<<7)))
        return false;
    Inode presentinode;
    SFD* sfdlist = new SFD[inode.filesize / sizeof(SFD)];
    if(!getcontent(inode, sfdlist, myfile))
    {
        delete[] sfdlist;
        return false;
    }
    for(int i = 0; i <(int)(inode.filesize / sizeof(SFD)); i++)
    {
        if(strcmp(sfdlist[i].name, ".") == 0 || strcmp(sfdlist[i].name, "..") == 0)
            continue;
        if(!getinode(sfdlist[i].inodeid, &presentinode, myfile))
        {
            delete[] sfdlist;
            return false;
        }
        if(presentinode.privilege & (1<<7))
        {
            if(!deletedir(presentinode, myfile, superblock, superinode))
            {
                delete[] sfdlist;
                return false;
            }
        }
        else
        {
            if(!deletefile(presentinode, myfile, superblock, superinode))
            {
                delete[] sfdlist;
                return false;
            }
        }
    }
    if(!deleteSFD(inode, myfile, superblock))
    {
        delete[] sfdlist;
        return false;
    }
    if(!recycleblock(inode.inodeid, myfile, superinode))
    {
        delete[] sfdlist;
        return false;
    }
    delete[] sfdlist;
    return true;
}

bool changeprivilege(Inode &inode, char privilege, fstream &myfile)
{
    inode.privilege = privilege;
    return saveinode(inode.inodeid, inode, myfile);
}

bool copyfile(const Inode &from, Inode &to, char* name, int uid, fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    if(from.privilege & (1<<7))
        return false;
    Inode newinode;
    if(!createfile(to, name, from.privilege, uid, &newinode, myfile, superinode, superblock))
       return false;
    char* buffer = new char[from.filesize];
    if(!readfile(from, 0, from.filesize, buffer, myfile))
    {
        delete[] buffer;
        return false;
    }
    delete[] buffer;
    return true;
}

bool copydir(const Inode &from, Inode &to, char* name, int uid,fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    if(!(from.privilege & (1<<7)))
        return false;
    Inode newinode;
    if(!createsondir(to, name, from.privilege, uid, &newinode, myfile, superinode, superblock))
        return false;
    Inode nowinode;
    SFD* sfdlist = new SFD[from.filesize / sizeof(SFD)];
    if(!getcontent(from, sfdlist, myfile))
    {
        delete[] sfdlist;
        return false;
    }
    for (int i = 0; i < (int)(from.filesize / sizeof(SFD)); i++)
    {
        if(strcmp(sfdlist[i].name, ".") == 0 || strcmp(sfdlist[i].name, "..") == 0)
            continue;
        if(!getinode(sfdlist[i].inodeid, &nowinode, myfile))
        {
            delete[] sfdlist;
            return false;
        }
        if(nowinode.privilege & (1<<7))
        {
            if(!copydir(nowinode, newinode, nowinode.name, uid, myfile, superblock, superinode))
            {
                delete[] sfdlist;
                return false;
            }
        }
        else
        {
            if(!copyfile(nowinode, newinode, nowinode.name, uid, myfile, superblock, superinode))
            {
                delete[] sfdlist;
                return false;
            }
        }
    }
    delete[] sfdlist;
    return true;
}

bool ccopy(const Inode &from, Inode &to, char* name, int uid,fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    if(from.privilege & (1<<7))
        return copydir(from, to, name, uid, myfile, superblock, superinode);
    else
        return copyfile(from, to, name, uid, myfile, superblock, superinode);
}

bool movefile(Inode &from, Inode &to, char* name, fstream &myfile, SuperBlock &superblock)
{
    if(!deleteSFD(from, myfile, superblock))
        return false;
    SFD newsfd;
    strcpy(newsfd.name, name);
    newsfd.inodeid = from.inodeid;
    if(!addSFD(to, newsfd, myfile, superblock))
       return false;
    strcpy(from.name, name);
    from.parent = to.inodeid;
    if(!saveinode(from.inodeid, from, myfile))
        return false;
    SFD* sfdlist = new SFD[from.filesize / sizeof(SFD)];
    if(!getcontent(from, sfdlist, myfile))
    {
        delete[] sfdlist;
        return false;
    }
    int result;
    if(!findcontent(sfdlist, from.filesize / sizeof(SFD), "..", &result, myfile))
    {
        delete[] sfdlist;
        return false;
    }
    sfdlist[result].inodeid = to.inodeid;
    if(!writefile(from, 0, from.filesize, (char*)sfdlist, myfile, superblock))
    {
        delete[] sfdlist;
        return false;
    }
    delete[] sfdlist;
    return true;
}

bool hardlink(Inode &from, Inode &to, char* name, fstream &myfile, SuperBlock &superinode, SuperBlock &superblock)
{
    if(from.privilege & (1<<7))
        return false;
    from.linknum++;
    if(!saveinode(from.inodeid, from, myfile))
        return false;
    Inode newinode;
    if(!createfile(to, name, from.privilege, from.inodeid, &newinode, myfile, superinode, superblock))
        return false;
    newinode.filesize = from.filesize;
    newinode.atime = from.atime;
    newinode.ctime = from.atime;
    newinode.iblock = from.iblock;
    newinode.ibytes = from.ibytes;
    memcpy(newinode.data, from.data, 11);
    if(!saveinode(newinode.inodeid, newinode, myfile))
        return false;
    return true;
}

bool softlink(char* from, Inode &to, char* name, int uid, fstream &myfile, SuperBlock &superinode, SuperBlock &superblock)
{
    Inode newinode;
    if(!createfile(to, name, (1<<6)|(1<<5)|(1<<3)|1, uid, &newinode, myfile, superinode, superblock))
        return false;
    if(!writefile(newinode, 0, strlen(from), from, myfile, superblock))
        return false;
    return true;
}

