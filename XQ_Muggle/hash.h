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
    uint32_t dwLock0;           // ZobristУ��������һ����
    uint16_t wmv;                      //����ŷ�
    uint8_t depth;                     //���
    uint8_t flag;                     //��־
    int16_t svl;                      //��ֵ
    uint32_t dwLock1;           // ZobristУ�������ڶ�����
}; 
HashStruct hshItems[nHashMask];          // �û���
void ClearHash(void) // ����û���
{         
    memset(hshItems, 0, nHashMask * sizeof(HashStruct));
}

bool HASH_POS_EQUAL(const HashStruct& hsh) 
{
    return hsh.dwLock0 == board.zobr.dwLock0 && hsh.dwLock1 == board.zobr.dwLock1;
}

/* �жϻ�ȡ�û���Ҫ������Щ�������û���ķ�ֵ����ĸ���ͬ�������в�ͬ�Ĵ���
 * һ�������ֵ��"WIN_VALUE"����(������"-WIN_VALUE"��"WIN_VALUE"֮�䣬��ͬ)����ֻ��ȡ�����������Ҫ��ľ��棻
 * ���������ֵ��"WIN_VALUE"��"BAN_VALUE"֮�䣬���ܻ�ȡ�û����е�ֵ(ֻ�ܻ�ȡ����ŷ������ο�)��Ŀ���Ƿ�ֹ���ڳ��������µġ��û���Ĳ��ȶ��ԡ���
 * ���������ֵ��"BAN_VALUE"���⣬���ȡ����ʱ���ؿ����������Ҫ����Ϊ��Щ�����Ѿ���֤����ɱ���ˣ�
 * �ġ������ֵ��"DrawValue()"(�ǵ�һ��������������)�����ܻ�ȡ�û����е�ֵ(ԭ����ڶ��������ͬ)��
 * ע�⣺���ڵ����������Ҫ��ɱ�岽�����е�����
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

//���û�
//��ȡ�û�����
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
    //���ؾ�������ķ�ֵ
    vl = ValueAdjust(bBanNode, bMateNode, hsh.svl);

    if (bBanNode||(hsh.depth < nDepth && !bMateNode))
        return -MATE_VALUE;

    //�鿴�߽�����
    if (hsh.flag == HASH_BETA) 
        return (vl >= Beta) ? vl : -MATE_VALUE;
    else if (hsh.flag == HASH_ALPHA)
        return (vl <= Alpha) ? vl : -MATE_VALUE;
    else
        return vl;
}

//�����û�����
void StoreHash(int flag, int mv, int vl, int nDepth)
{
    HashStruct hsh = hshItems[(board.zobr.dwKey) & (nHashMask - 1)];
    if (nDepth < hsh.depth) // �������
        return; 
    //std::cout << "hashdepth:" << hsh.depth << " key:" << board.zobr.dwKey << '\n';
    hsh.flag = flag;
    hsh.depth = nDepth;
    
    //�Է�ֵ��ɱ�岽��������
    if (vl > WIN_VALUE)
    {
        if (mv == 0 && vl <= BAN_VALUE)
            return; //���³����ľ���(�������û��ü�)���������ŷ�Ҳû�У���ôû�б�Ҫд���û���
        vl += board.nowDepth;
    }
    else if (vl < -WIN_VALUE)
    {
        if (mv == 0 && vl >= -BAN_VALUE)
            return; // ͬ��
        vl -= board.nowDepth;
    }
    else if (vl == board.DrawValue() && mv == 0)
    {
        return;   // ͬ��
    }

    // ��¼�û���
    hsh.svl = vl;
    hsh.dwLock0 = board.zobr.dwLock0;
    hsh.dwLock1 = board.zobr.dwLock1;
    hsh.wmv = mv;
    hshItems[(board.zobr.dwKey) & (nHashMask-1)] = hsh;/**/
}/**/
#endif