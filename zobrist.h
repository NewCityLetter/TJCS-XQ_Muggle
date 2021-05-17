#pragma once
#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "base.h"
/*
zobrist hash
1) 将棋盘分为最小单位(如果将9X9围棋盘分为81个交叉点),求出每个单位上不同状态数(如围棋盘上的 1 个交叉点有 3 个状态)。

2) 为每个单位上的每种状态生成一个一定范围内(如64位整数)随机数。

3) 对于特定的棋局,将每个单位上的状态对应的随机数作异或运算,所得即为哈希值。
*/

struct RC4Struct
{
    uint8 s[256];
    int x, y;

    void InitZero(void) //为棋盘每个位置生成一个伪随机值
    {
        int i, j;
        uint8 uc;

        x = y = j = 0;
        for (i = 0; i < 256; i++)
            s[i] = i;
        for (i = 0; i < 256; i++)
        {
            j = (j + s[i]) & 255;
            uc = s[i];
            s[i] = s[j];
            s[j] = uc;
        }
    }
    uint8 NextByte(void)
    {
        x = (x + 1) & 255;
        y = (y + s[x]) & 255;
        std::swap(s[x], s[y]);
        return s[(s[x] + s[y]) & 255];
    }
    uint32 NextLong(void)
    {
        union //union占据同一空间，四个uc赋值即为dw赋值
        {
            uint8 uc[4];
            uint32 dw;
        } Ret;
        Ret.uc[0] = NextByte();
        Ret.uc[1] = NextByte();
        Ret.uc[2] = NextByte();
        Ret.uc[3] = NextByte();
        return Ret.dw;
    }
};
//
struct ZobristStruct
{
    uint64_t dwKey;
    uint32_t dwLock0, dwLock1;
    void InitZero(void)
    {
        dwKey = dwLock0 = dwLock1 = 0;
    }
    void InitRC4(RC4Struct& rc4)
    {
        //dwKey = rc4.NextLong();
        dwLock0 = rc4.NextLong();
        dwLock1 = rc4.NextLong();
    }
    void Xor(const ZobristStruct& zobr, const unsigned __int64 info)
    {
        dwKey ^= info;
        dwLock0 ^= zobr.dwLock0;
        dwLock1 ^= zobr.dwLock1;
    }
}; // zobr

// Zobrist表
struct ZobristTable
{
    ZobristStruct Player;
    ZobristStruct Table[14][256];
};

extern ZobristTable Zobrist;

// 初始化Zobrist表
void InitZobrist(void);


#endif