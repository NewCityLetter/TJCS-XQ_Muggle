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
    uint32_t dwLock0;           // Zobrist校验锁，第一部分
    uint16_t wmv;                      //最佳着法
    uint8_t depth;                     //深度
    uint8_t flag;                     //标志
    int16_t svl;                      //分值
    uint32_t dwLock1;           // Zobrist校验锁，第二部分
}; 
HashStruct hshItems[nHashMask];          // 置换表
void ClearHash(void) // 清空置换表
{         
    memset(hshItems, 0, nHashMask * sizeof(HashStruct));
}

bool HASH_POS_EQUAL(const HashStruct& hsh) 
{
    return hsh.dwLock0 == board.zobr.dwLock0 && hsh.dwLock1 == board.zobr.dwLock1;
}

/* 判断获取置换表要符合哪些条件，置换表的分值针对四个不同的区间有不同的处理：
 * 一、如果分值在"WIN_VALUE"以内(即介于"-WIN_VALUE"到"WIN_VALUE"之间，下同)，则只获取满足搜索深度要求的局面；
 * 二、如果分值在"WIN_VALUE"和"BAN_VALUE"之间，则不能获取置换表中的值(只能获取最佳着法仅供参考)，目的是防止由于长将而导致的“置换表的不稳定性”；
 * 三、如果分值在"BAN_VALUE"以外，则获取局面时不必考虑搜索深度要求，因为这些局面已经被证明是杀棋了；
 * 四、如果分值是"DrawValue()"(是第一种情况的特殊情况)，则不能获取置换表中的值(原因与第二种情况相同)。
 * 注意：对于第三种情况，要对杀棋步数进行调整！
 */
int ValueAdjust(bool& bBanNode, bool& bMateNode, int vl) 
{
    bBanNode = bMateNode = false;
    if (vl > WIN_VALUE)
    {
        if (vl <= BAN_VALUE)
            bBanNode = true;
        else
        {
            bMateNode = true;
            vl -= board.nowDepth;
        }
    }
    else if (vl < -WIN_VALUE) 
    {
        if (vl >= -BAN_VALUE) 
        {
            bBanNode = true;
        }
        else 
        {
            bMateNode = true;
            vl += board.nowDepth;
        }
    }
    else if (vl == board.DrawValue()) 
    {
        bBanNode = true;
    }
    return vl;
}

//单置换
//提取置换表项
int LookUpHash(int Alpha, int Beta, int nDepth, int& mv)
{
    HashStruct hsh;
    int vl;
    bool bBanNode, bMateNode;
    mv = 0;
    hsh = hshItems[(board.zobr.dwKey) & (nHashMask - 1)];

    if (HASH_POS_EQUAL(hsh))
        mv = hsh.wmv;
    else 
        return -MATE_VALUE;
    
    board.moveHash[board.nowDepth] = hsh.wmv;
    //返回经过处理的分值
    vl = ValueAdjust(bBanNode, bMateNode, hsh.svl);

    if (bBanNode||(hsh.depth < nDepth && !bMateNode))
        return -MATE_VALUE;

    //查看边界条件
    if (hsh.flag == HASH_BETA) 
        return (vl >= Beta) ? vl : -MATE_VALUE;
    else if (hsh.flag == HASH_ALPHA)
        return (vl <= Alpha) ? vl : -MATE_VALUE;
    else
        return vl;
}

//保存置换表项
void StoreHash(int flag, int mv, int vl, int nDepth)
{
    HashStruct hsh = hshItems[(board.zobr.dwKey) & (nHashMask - 1)];
    if (nDepth < hsh.depth) // 深度优先
        return; 
    //std::cout << "hashdepth:" << hsh.depth << " key:" << board.zobr.dwKey << '\n';
    hsh.flag = flag;
    hsh.depth = nDepth;
    
    //对分值做杀棋步数调整；
    if (vl > WIN_VALUE)
    {
        if (mv == 0 && vl <= BAN_VALUE)
            return; //导致长将的局面(不进行置换裁剪)如果连最佳着法也没有，那么没有必要写入置换表
        vl += board.nowDepth;
    }
    else if (vl < -WIN_VALUE)
    {
        if (mv == 0 && vl >= -BAN_VALUE)
            return; // 同理
        vl -= board.nowDepth;
    }
    else if (vl == board.DrawValue() && mv == 0)
    {
        return;   // 同理
    }

    // 记录置换表。
    hsh.svl = vl;
    hsh.dwLock0 = board.zobr.dwLock0;
    hsh.dwLock1 = board.zobr.dwLock1;
    hsh.wmv = mv;
    hshItems[(board.zobr.dwKey) & (nHashMask-1)] = hsh;/**/
}/**/
#endif