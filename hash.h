#ifndef HASH_H
#define HASH_H
#pragma once

#include "base.h"
#include "board.h"

extern boardStruct board;

const int WIN_VALUE = MATE_VALUE - 200; // 搜索出胜负的分值界限，超出此值就说明已经搜索出杀棋了
const int BAN_VALUE = MATE_VALUE - 100;  // 长将判负的分值，用于判断结点是否要写入置换表

// 置换表标志，只用在"RecordHash()"函数中
const int HASH_BETA = 1;
const int HASH_ALPHA = 2;
const int HASH_PV = 3;

const int HASH_LAYERS = 2;   // 置换表的层数

// 置换表结构，置换表信息夹在两个Zobrist校验锁中间，可以防止存取冲突
struct HashStruct 
{
    uint32_t dwLock0;           // Zobrist校验锁，第一部分
    uint16_t wmv;                      // 最佳着法
    uint8_t ucAlphaDepth, ucBetaDepth; // 深度(上边界和下边界)
    int16_t svlAlpha, svlBeta;         // 分值(上边界和下边界)
    uint32_t dwLock1;           // Zobrist校验锁，第二部分
}; // hsh
void ClearHash(void);
void NewHash(int nHashScale);
void DelHash(void);
bool HASH_POS_EQUAL(const HashStruct& hsh);
// 按局面和层数获取置换表项(返回一个引用，可以对其赋值)
HashStruct& HASH_ITEM(int nLayer);
inline void StoreHash(int nFlag, int vl, int nDepth, int mv);                    // 存储置换表局面信息
inline int LookUpHash(int vlAlpha, int vlBeta, int nDepth, bool bNoNull, int& mv); // 获取置换表局面信息

int nHashMask;
HashStruct* hshItems;
/* 判断获取置换表要符合哪些条件，置换表的分值针对四个不同的区间有不同的处理：
 * 一、如果分值在"WIN_VALUE"以内(即介于"-WIN_VALUE"到"WIN_VALUE"之间，下同)，则只获取满足搜索深度要求的局面；
 * 二、如果分值在"WIN_VALUE"和"BAN_VALUE"之间，则不能获取置换表中的值(只能获取最佳着法仅供参考)，目的是防止由于长将而导致的“置换表的不稳定性”；
 * 三、如果分值在"BAN_VALUE"以外，则获取局面时不必考虑搜索深度要求，因为这些局面已经被证明是杀棋了；
 * 四、如果分值是"DrawValue()"(是第一种情况的特殊情况)，则不能获取置换表中的值(原因与第二种情况相同)。
 * 注意：对于第三种情况，要对杀棋步数进行调整！
 */
void ClearHash(void) 
{         // 清空置换表
    memset(hshItems, 0, (nHashMask + 1) * sizeof(HashStruct));
}
void NewHash(int nHashScale) 
{ // 分配置换表，大小是 2^nHashScale 字节
    nHashMask = ((1 << nHashScale) / sizeof(HashStruct)) - 1;
    hshItems = new HashStruct[nHashMask + 1];
    ClearHash();
}

void DelHash(void)
 {           // 释放置换表
    delete[] hshItems;
}
bool HASH_POS_EQUAL(const HashStruct& hsh) 
{
    return hsh.dwLock0 == board.zobr.dwLock0 && hsh.dwLock1 == board.zobr.dwLock1;
}
// 按局面和层数获取置换表项(返回一个引用，可以对其赋值)
HashStruct& HASH_ITEM(int nLayer) 
{
    return hshItems[(board.zobr.dwKey + nLayer) & nHashMask];
}

int ValueAdjust(bool& bBanNode, bool& bMateNode, int vl) 
{
    bBanNode = bMateNode = false;
    if (vl > WIN_VALUE) 
    {
        if (vl <= BAN_VALUE) 
        {
            bBanNode = true;
        }
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
//提取置换表项
int LookUpHash(int Alpha, int Beta, int nDepth, bool bNoNull, int& mv)
{
    HashStruct hsh;
    int i, vl;
    bool bBanNode, bMateNode;
    // 获取置换表局面信息的过程包括以下几个步骤：

    // 1. 逐层获取置换表项
    mv = 0;
    for (i = 0; i < HASH_LAYERS; i++) 
    {
        hsh = HASH_ITEM(i);
        if (HASH_POS_EQUAL(hsh)) 
        {
            mv = hsh.wmv;
            break;
        }
    }
    if (i == HASH_LAYERS) {
        return -MATE_VALUE;
    }

    // 2. 判断是否符合Beta边界
    if (hsh.ucBetaDepth > 0) 
    {
        vl = ValueAdjust(bBanNode, bMateNode, hsh.svlBeta);
        if (!bBanNode && !(bNoNull) && (hsh.ucBetaDepth >= nDepth || bMateNode) && vl >= Beta) 
        {
            if (hsh.wmv == 0) 
            {
                return vl;
            }
        }
    }

    // 3. 判断是否符合Alpha边界
    if (hsh.ucAlphaDepth > 0) 
    {
        vl = ValueAdjust(bBanNode, bMateNode, hsh.svlAlpha);
        if (!bBanNode && (hsh.ucAlphaDepth >= nDepth || bMateNode) && vl <= Alpha) 
        {
            if (hsh.wmv == 0) 
            {
                return vl;
            }
        }
    }
    return -MATE_VALUE;
}

//保存置换表项
void StoreHash(int flag, int mv, int vl, int nDepth)
{
    HashStruct hsh;
    int i, nHashDepth, nMinDepth, nMinLayer;
    // 存储置换表局面信息的过程包括以下几个步骤：

    // 1. 对分值做杀棋步数调整；
    if (vl > WIN_VALUE) 
    {
        if (mv == 0 && vl <= BAN_VALUE) 
        {
            return; // 导致长将的局面(不进行置换裁剪)如果连最佳着法也没有，那么没有必要写入置换表
        }
        vl += board.nowDepth;
    }
    else if (vl < -WIN_VALUE) 
    {
        if (mv == 0 && vl >= -BAN_VALUE) 
        {
            return; // 同理
        }
        vl -= board.nowDepth;
    }
    else if (vl == board.DrawValue() && mv == 0) 
    {
        return;   // 同理
    }

    // 2. 逐层试探置换表；
    nMinDepth = 512;
    nMinLayer = 0;
    for (i = 0; i < HASH_LAYERS; i++) 
    {
        hsh = HASH_ITEM(i);

        // 3. 如果试探到一样的局面，那么更新置换表信息即可；
        if (HASH_POS_EQUAL(hsh)) 
        {
            // 如果深度更深，或者边界缩小，都可更新置换表的值
            if ((flag & HASH_ALPHA) != 0 && (hsh.ucAlphaDepth <= nDepth || hsh.svlAlpha >= vl)) 
            {
                hsh.ucAlphaDepth = nDepth;
                hsh.svlAlpha = vl;
            }
            // Beta结点要注意：不要用Null-Move的结点覆盖正常的结点
            if ((flag & HASH_BETA) != 0 && (hsh.ucBetaDepth <= nDepth || hsh.svlBeta <= vl) && (mv != 0 || hsh.wmv == 0)) 
            {
                hsh.ucBetaDepth = nDepth;
                hsh.svlBeta = vl;
            }
            // 最佳着法是始终覆盖的
            if (mv != 0) 
            {
                hsh.wmv = mv;
            }
            HASH_ITEM(i) = hsh;
            return;
        }

        // 4. 如果不是一样的局面，那么获得深度最小的置换表项；
        nHashDepth = MAX((hsh.ucAlphaDepth == 0 ? 0 : hsh.ucAlphaDepth + 256),
            (hsh.wmv == 0 ? hsh.ucBetaDepth : hsh.ucBetaDepth + 256));
        if (nHashDepth < nMinDepth) 
        {
            nMinDepth = nHashDepth;
            nMinLayer = i;
        }
    }

    // 5. 记录置换表。
    hsh.dwLock0 = board.zobr.dwLock0;
    hsh.dwLock1 = board.zobr.dwLock1;
    hsh.wmv = mv;
    hsh.ucAlphaDepth = hsh.ucBetaDepth = 0;
    hsh.svlAlpha = hsh.svlBeta = 0;
    if ((flag & HASH_ALPHA) != 0) 
    {
        hsh.ucAlphaDepth = nDepth;
        hsh.svlAlpha = vl;
    }
    if ((flag & HASH_BETA) != 0) 
    {
        hsh.ucBetaDepth = nDepth;
        hsh.svlBeta = vl;
    }
    HASH_ITEM(nMinLayer) = hsh;
}

#endif