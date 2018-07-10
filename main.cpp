#include <iostream>
#include <vector>
#include "user.h"
#include "filesystem.h"
#include "process.h"

using namespace std;
void lasttest(fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    /*cout << "================Test GetBIDByFOff=================" << endl;
    bool flag = true;
    // Create iNode
    Inode testiNode;
    testiNode.data[0] = 233;    // 2
    testiNode.data[5] = 666;    // 5120
    testiNode.data[9] = 22;     // 9300
    testiNode.data[10] = 33;    // 10250  271370
    // Create level one indirect index block
    char buf[blocksize];    //33
    memset(buf, 0, blocksize);
    bid_t target1 = 321;
    bid_t target2 = 123;
    memcpy(buf, &target1, sizeof(bid_t));
    memcpy(buf + 255 * sizeof(bid_t), &target2, sizeof(bid_t));
    flag &= writeblock(33, buf,blocksize, myfile);
    bid_t rst = 0;
    getblockid(testiNode, 511, &rst, myfile);
    cout << rst << endl;
    getblockid(testiNode, 2560, &rst, myfile);
    cout << rst << endl;
    getblockid(testiNode, 4650, &rst, myfile);
    cout << rst << endl;
    getblockid(testiNode, 5120, &rst, myfile);
    cout << rst << endl;
    cout << flag << endl;*/

     /*cout << "================Test ReadFileToBuf=================" << endl;
    // Create iNode
    Inode testiNode;
    testiNode.filesize = 512*6;
    testiNode.data[1] = 111;
    testiNode.data[2] = 222;
    char buf[blocksize];
    for (int i = 0; i < blocksize; ++i)
        buf[i] = '!';
    writeblock(111, buf, blocksize, myfile);
    writeblock(222, buf, blocksize, myfile);

    // start  len
    // 1030   10
    char testBuf1[10];
    readfile(testiNode, 518, 10, testBuf1, myfile);
    for (int i = 0; i < 10; ++i)
        cout << testBuf1[i];
    cout << endl;

    // start  len
    // 1024   1024
    int cnt = 0;
    char testBuf2[512];
    readfile(testiNode, 512, 512, testBuf2, myfile);
    for (int i = 0; i < 512; ++i)
        if (testBuf2[i] == '!') ++cnt;
    cout << cnt << endl;

    // start  len
    // 1030   1024
    cnt = 0;
    char testBuf3[512];
    readfile(testiNode, 518, 512, testBuf3, myfile);
    for (int i = 0; i < 512; ++i)
    {
        if (testBuf3[i] == '!') ++cnt;
    }
    cout << cnt << endl;

    // start  len
    // 1030   2042
    cnt = 0;
    char testBuf4[2042];
    readfile(testiNode, 1030, 2042, testBuf4, myfile);
    for (int i = 0; i < 2042; ++i)
    {
        if (testBuf3[i] == '!') ++cnt;
    }
    cout << cnt << endl;*/
    /*cout << "================Test AppendBlocksToFile=================" << endl;
    Inode testiNode;
    testiNode.filesize = 0;
    testiNode.iblock = 0;
    testiNode.ibytes = 0;
    assignblock(&testiNode.inodeid,myfile, superinode);
    cout << "testiNode.bid: " << testiNode.inodeid << endl;
    writeblock(testiNode.inodeid, (char*)&testiNode, sizeof(Inode),myfile);

    if (addblock(testiNode, 100, myfile , superblock))
        cout << "Successfully appended" << endl;
    else
        cout << "Failed to append" << endl;

    cout << "testiNode.blocks: " << testiNode.iblock << endl;
    for (int i = 0; i < 11; i++)
        cout << "testiNode.data[" << i << "]: " << testiNode.data[i] << endl;*/
     // Test GLP Distribute
    /*cout << "================Test GLP Distribute=================" << endl;
    bid_t bid;
    int blockCnt = 0;
    while (assignblock(&bid,myfile,superinode))
    {
        //cout << "Block " << bid << " assigned" << endl;
        blockCnt++;
    }
    cout << "Assigned block cnt: " << blockCnt << endl;
    cout << "Assigned block cnt expected: " << maxinode - mininode + 1 << endl;

    // Test GLP Recycle
    blockCnt = 0;
    for (int i = mininode; i <= maxinode; i++)
    {
        if (recycleblock(i,myfile,superinode))
        {
            //cout << "Block " << i << " recycled" << endl;
            blockCnt++;
        }
    }
    cout << "Recycled block cnt: " << blockCnt << endl;
    cout << "Recycled block cnt expected: " << maxinode - mininode + 1 << endl;*/
    /*cout << "================Test WriteFileFromBuf=================" << endl;
    Inode testiNode;
    testiNode.filesize = 0;
    testiNode.iblock = 0;
    testiNode.ibytes = 0;
    assignblock(&testiNode.inodeid, myfile, superblock);
    cout << "testiNode.bid: " << testiNode.inodeid << endl;
    int i = 0;
    char s[1024 * 270 + 10];
    for (i = 0; i < 1024 * 270 + 1; i++)
        s[i] = 'b';
    s[i] = '\0';
    if (writefile(testiNode, 0, strlen(s), s, myfile, superblock))
        cout << "Write successful" << endl;
    else
        cout << "Write failed" << endl;
    cout << "testiNode.blocks: " << testiNode.iblock << endl;
    cout << "testiNode.bytes: " << testiNode.ibytes << endl;
    cout << "testiNode.size: " << testiNode.filesize << endl;
    cout << "testiNode.atime: " << testiNode.atime << endl;
    cout << "testiNode.mtime: " << testiNode.ctime << endl;
    for (i = 0; i < 12; i++)
        cout << "testiNode.data[" << i << "]: " << testiNode.data[i] << endl;*/
       /* cout << "================Test CreateRootDir=================" << endl;
    if (!rootdir(myfile, superblock))
        cout << "Failed to Create /" << endl;
    else
        cout << "/ Created" << endl;
    Inode rootiNode;
    getinode(3, &rootiNode, myfile);
    cout << "rootiNode.size: " << rootiNode.filesize << endl;
    for (int i = 0; i < 11; i++)
        cout << "rootiNode.data[" << i << "]: " << rootiNode.data[i] << endl;*/
       /* cout << "================Test CreateSubDir=================" << endl;
    Inode rootiNode;
    getinode(3, &rootiNode, myfile);
    char dirname[] = "home";
    Inode rst;
    if (createsondir(rootiNode, dirname, (1<<6 | 1<<5 | 1<<4) | (1<<3 | 1<<2 | 1<<1), 0, &rst,myfile,superinode,superblock))
        cout << "Failed to Create /home" << endl;
    else
        cout << "/home Created" << endl;
    getinode(3, &rootiNode, myfile);
    cout << "rootiNode.size: " << rootiNode.filesize << endl;
    for (int i = 0; i < 11; i++)
        cout << "rootiNode.data[" << i << "]: " << rootiNode.data[i] << endl;*/
       /* cout << "================Test ParsePath=================" << endl;
    char path[128] = "/home/../../../../../../home";
    Inode rootiNode;
    Inode rstiNode;
    getinode(3, &rootiNode,myfile);
    if (!findpath(rootiNode, path, true, &rstiNode, myfile))
    {
        cout << "Failed to parse " << path << endl;
        return;
    }
    else
    {
        cout << "Parse " << path << " complete" << endl;
        cout << rstiNode.name << endl;
    }*/
    /*cout << "================Test GetAbsDir=================" << endl;
    char path[128] = "/home";
    Inode rootiNode;
    Inode rstiNode;
    getinode(3, &rootiNode, myfile);
    if (!findpath(rootiNode, path, true, &rstiNode, myfile))
    {
        cout << "Failed to parse " << path << endl;
        return;
    }
    char rstPath[128];
    getdir(rstiNode, rstPath,myfile);
    cout << "Abs Dir: " << rstPath << endl;*/
    /*cout << "================Test Touch=================" << endl;
    char path[128] = "/home";
    Inode rootiNode;
    Inode homeiNode;
    getinode(rootinode, &rootiNode, myfile);
    if (!findpath(rootiNode, path, true, &homeiNode, myfile))
    {
        cout << "Failed to parse " << path << endl;
        return;
    }
    // Touch /home/1.txt
    char fname[] = "1.txt";
    Inode rstiNode;
    if (!createfile(homeiNode, fname, (1<<6)|(1<<5)|(1<<3), rootid, &rstiNode,myfile, superinode, superblock))
    {
        cout << "Failed to touch " << fname << endl;
    }
    else
    {
        cout << "Touch " << fname << " success" << endl;
    }

    cout << "================Test GetContentInDir=================" << endl;
    char path1[128] = "/home";
    Inode rootiNode1;
    Inode homeiNode1;
    getinode(rootinode, &rootiNode1, myfile);
    if (!findpath(rootiNode1, path1, true, &homeiNode1, myfile))
    {
        cout << "Failed to parse " << path1 << endl;
        return;
    }

    SFD* SFDList = new SFD[homeiNode1.filesize / sizeof(SFD)];
    if (!getcontent(homeiNode1, SFDList,myfile))
    {
        cout << "Failed to ls /home" << endl;
    }
    else
    {
        for (int i = 0; i < (int)(homeiNode1.filesize / sizeof(SFD)); i++)
        {
            cout << SFDList[i].name << endl;
            cout << SFDList[i].inodeid <<endl;
        }
    }
    delete[] SFDList;*/
}

void test(fstream &myfile, SuperBlock &superblock, SuperBlock &superinode)
{
    cout << "================Test Touch=================" << endl;
    char path[128] = "/home";
    Inode rootiNode;
    Inode homeiNode;
    getinode(rootinode, &rootiNode, myfile);
    if (!findpath(rootiNode, path, true, &homeiNode,true,  myfile))
    {
        cout << "Failed to parse " << path << endl;
        return;
    }
    // Touch /home/1.txt
    char fname[] = "1.txt";
    Inode rstiNode;
    if (!createfile(homeiNode, fname, (1<<6)|(1<<5)|(1<<3), rootid, &rstiNode,myfile, superinode, superblock))
    {
        cout << "Failed to touch " << fname << endl;
    }
    else
    {
        cout << "Touch " << fname << " success" << endl;
    }


    cout << "================Test WriteFileFromBuf2=================" << endl;
    char path1[128] = "/home/1.txt";
    Inode rootiNode1;
    Inode txtiNode;
    getinode(rootinode, &rootiNode1, myfile);
    if (!findpath(rootiNode, path1, true, &txtiNode, true, myfile))
    {
        cout << "Failed to parse " << path1 << endl;
        return;
    }

    char s[512 * 3];
    for (int i = 0; i < 512 * 3; i++)
        s[i] = 'y';
    if (!writefile(txtiNode, 0, 512 * 3, s, myfile, superblock))
    {
        cout << "Failed to write " << path1 << endl;
    }
    else
    {
        cout << "Write " << path << " success" << endl;
        cout << "txtiNode.size: " << txtiNode.filesize << endl;
        for (int i = 0; i < 12; i++)
            cout << "txtiNode.data[" << i << "]: " << txtiNode.data[i] << endl;
    }
/*cout << "================Test ParsePath=================" << endl;
    char path[128] = "/home/../../../../../../home";
    Inode rootiNFSControllerode;
    Inode rstiNode;
    getinode(3, &rootiNode,myfile);
    if (!findpath(rootiNode, path, true, &rstiNode, true, myfile))
    {
        cout << "Failed to parse " << path << endl;
        return;
    }
    else
    {
        cout << "Parse " << path << " complete" << endl;
        cout << rstiNode.name << endl;
    }*/

}

int main()
{
    if(exist()!=1)
        {
            create();
        }
    fstream myfile;
    SuperBlock superblock;
    SuperBlock superinode;
    load(myfile);
    testformat(myfile, superblock, superinode);
    testtime();
    loadsuperblock(SUPERBLOCK, myfile, superblock);
    loadsuperblock(SUPERINODE, myfile, superinode);
    test(myfile, superblock, superinode);
    savesuperblock(SUPERBLOCK, myfile, superblock);
    savesuperblock(SUPERINODE, myfile, superinode);
    int uid;
    Inode presentiNode;
    Inode uinode;
    vector<user> userlist;
    InodeMem ilist[128];
    process plist[10];
    pid_t pidp = 0;

    /*char month[12][5]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
            "Aug", "Sep", "Oct", "Nov", "Dec"};*/
    return 0;
}

