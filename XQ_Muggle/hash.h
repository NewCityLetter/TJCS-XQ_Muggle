#ifndef HASH_H
#define HASH_H
#pragma once

#include "base.h"
#include "board.h"

extern boardStruct board;

const int nHashMask = 1 << 20; // �û����С
// �û����־
const int HASH_BETA = 1;
const int HASH_ALPHA = 2;
const int HASH_PV = 3;

//���û�����û���ṹ
struct HashStruct
{
    uint32 dwLock0;           // ZobristУ��������һ����
    int32 move;                //����ŷ�
    int32 depth;              //���
    int32 flag;               //��־
    int32 val;                //��ֵ
    uint32 dwLock1;           // ZobristУ�������ڶ�����
}; 
HashStruct hshItems[nHashMask];          // �û���
void ClearHash(void) // ����û���
{         
    memset(hshItems, 0, nHashMask * sizeof(HashStruct));
}


//��ȡ�û�����
int LookUpHash(int32 Alpha, int32 Beta, int32 nDepth, int32& mv)
{
    HashStruct hsh;
    
    bool bMateNode = false;
    mv = 0;
    hsh = hshItems[(board.zobr.dwKey) & (nHashMask - 1)];
    
    if (hsh.dwLock0 == board.zobr.dwLock0 && hsh.dwLock1 == board.zobr.dwLock1)
        mv = hsh.move;
    else 
        return -MATE_VALUE;

    //board.moveHash[board.nowDepth] = hsh.move;

    if (hsh.val > WIN_VALUE)
    {
        if (hsh.val <= BAN_VALUE)
            return -MATE_VALUE;
        else
        {
            bMateNode = true;
            hsh.val -= board.nowDepth;
        }
    }
    else if (hsh.val < -WIN_VALUE)
    {
        if (hsh.val >= -BAN_VALUE)
        {
            return -MATE_VALUE;
        }
        else 
        {
            bMateNode = true;
            hsh.val += board.nowDepth;
        }
    }
    else if (hsh.val == board.DrawValue())
    {
        return -MATE_VALUE;
    }
    
    if (hsh.depth < nDepth && !bMateNode)
        return -MATE_VALUE;

    //�鿴�߽�����
    if (hsh.flag == HASH_BETA) 
        return (hsh.val >= Beta) ? hsh.val : -MATE_VALUE;
    else if (hsh.flag == HASH_ALPHA)
        return (hsh.val <= Alpha) ? hsh.val : -MATE_VALUE;
    else
        return hsh.val;
}

//�����û�����
void StoreHash(int32 flag, int32 mv, int32 vl, int32 nDepth)
{
    HashStruct hsh = hshItems[(board.zobr.dwKey) & (nHashMask - 1)];
    if (nDepth < hsh.depth) // �������
        return; 
    
    hsh.flag = flag;
    hsh.depth = nDepth;
    
    //�Է�ֵ��ɱ�岽��������
    if (vl > WIN_VALUE)
    {
        if (mv == 0 && vl <= BAN_VALUE)
            return;                //���³����ľ������������ŷ�Ҳû�У�û�б�Ҫд���û���
        vl += board.nowDepth;
    }
    else if (vl < -WIN_VALUE)
    {
        if (mv == 0 && vl >= -BAN_VALUE)
            return;
        vl -= board.nowDepth;
    }
    else if (vl == board.DrawValue() && mv == 0)
    {
        return;
    }

    // ��¼�û���
    hsh.val = vl;
    hsh.dwLock0 = board.zobr.dwLock0;
    hsh.dwLock1 = board.zobr.dwLock1;
    hsh.move = mv;
    hshItems[(board.zobr.dwKey) & (nHashMask-1)] = hsh;

}
#endif