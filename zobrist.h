#pragma once
#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "base.h"
/*
zobrist hash
1) �����̷�Ϊ��С��λ(�����9X9Χ���̷�Ϊ81�������),���ÿ����λ�ϲ�ͬ״̬��(��Χ�����ϵ� 1 ��������� 3 ��״̬)��

2) Ϊÿ����λ�ϵ�ÿ��״̬����һ��һ����Χ��(��64λ����)�������

3) �����ض������,��ÿ����λ�ϵ�״̬��Ӧ����������������,���ü�Ϊ��ϣֵ��
*/

struct RC4Struct
{
    uint8 s[256];
    int x, y;

    void InitZero(void) //Ϊ����ÿ��λ������һ��α���ֵ
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
        union //unionռ��ͬһ�ռ䣬�ĸ�uc��ֵ��Ϊdw��ֵ
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

// Zobrist��
struct ZobristTable
{
    ZobristStruct Player;
    ZobristStruct Table[14][256];
};

extern ZobristTable Zobrist;

// ��ʼ��Zobrist��
void InitZobrist(void);


#endif