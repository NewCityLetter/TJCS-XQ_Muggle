#ifndef HASH_H
#define HASH_H
#pragma once

#include "base.h"
#include "board.h"

extern boardStruct board;

const int nHashMask = 1 << 20; // 置换表大小
// 置换表标志
const int HASH_BETA = 1;
const int HASH_ALPHA = 2;
const int HASH_PV = 3;

//单置换表的置换表结构
struct HashStruct
{
    uint32 dwLock0;           // Zobrist校验锁，第一部分
    int32 move;                //最佳着法
    int32 depth;              //深度
    int32 flag;               //标志
    int32 val;                //分值
    uint32 dwLock1;           // Zobrist校验锁，第二部分
}; 
HashStruct hshItems[nHashMask];          // 置换表
void ClearHash(void) // 清空置换表
{         
    memset(hshItems, 0, nHashMask * sizeof(HashStruct));
}


//提取置换表项
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

    //查看边界条件
    if (hsh.flag == HASH_BETA) 
        return (hsh.val >= Beta) ? hsh.val : -MATE_VALUE;
    else if (hsh.flag == HASH_ALPHA)
        return (hsh.val <= Alpha) ? hsh.val : -MATE_VALUE;
    else
        return hsh.val;
}

//保存置换表项
void StoreHash(int32 flag, int32 mv, int32 vl, int32 nDepth)
{
    HashStruct hsh = hshItems[(board.zobr.dwKey) & (nHashMask - 1)];
    if (nDepth < hsh.depth) // 深度优先
        return; 
    
    hsh.flag = flag;
    hsh.depth = nDepth;
    
    //对分值做杀棋步数调整；
    if (vl > WIN_VALUE)
    {
        if (mv == 0 && vl <= BAN_VALUE)
            return;                //导致长将的局面如果连最佳着法也没有，没有必要写入置换表
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

    // 记录置换表
    hsh.val = vl;
    hsh.dwLock0 = board.zobr.dwLock0;
    hsh.dwLock1 = board.zobr.dwLock1;
    hsh.move = mv;
    hshItems[(board.zobr.dwKey) & (nHashMask-1)] = hsh;

}
#endif