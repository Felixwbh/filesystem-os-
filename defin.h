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
���̹�32*1024*1024�ֽ�
ÿ��512�ֽ�
��65536��
һ��������512�ֽ� ջ��2�ֽ� ���п�� 255*2�ֽ�
�����ڵ���
*/

#endif // _DEFIN_H
