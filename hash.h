#ifndef HASH_H
#define HASH_H
#pragma once

#include "base.h"
#include "board.h"

extern boardStruct board;

const int WIN_VALUE = MATE_VALUE - 200; // ������ʤ���ķ�ֵ���ޣ�������ֵ��˵���Ѿ�������ɱ����
const int BAN_VALUE = MATE_VALUE - 100;  // �����и��ķ�ֵ�������жϽ���Ƿ�Ҫд���û���

// �û����־��ֻ����"RecordHash()"������
const int HASH_BETA = 1;
const int HASH_ALPHA = 2;
const int HASH_PV = 3;

const int HASH_LAYERS = 2;   // �û���Ĳ���

// �û���ṹ���û�����Ϣ��������ZobristУ�����м䣬���Է�ֹ��ȡ��ͻ
struct HashStruct 
{
    uint32_t dwLock0;           // ZobristУ��������һ����
    uint16_t wmv;                      // ����ŷ�
    uint8_t ucAlphaDepth, ucBetaDepth; // ���(�ϱ߽���±߽�)
    int16_t svlAlpha, svlBeta;         // ��ֵ(�ϱ߽���±߽�)
    uint32_t dwLock1;           // ZobristУ�������ڶ�����
}; // hsh
void ClearHash(void);
void NewHash(int nHashScale);
void DelHash(void);
bool HASH_POS_EQUAL(const HashStruct& hsh);
// ������Ͳ�����ȡ�û�����(����һ�����ã����Զ��丳ֵ)
HashStruct& HASH_ITEM(int nLayer);
inline void StoreHash(int nFlag, int vl, int nDepth, int mv);                    // �洢�û��������Ϣ
inline int LookUpHash(int vlAlpha, int vlBeta, int nDepth, bool bNoNull, int& mv); // ��ȡ�û��������Ϣ

int nHashMask;
HashStruct* hshItems;
/* �жϻ�ȡ�û���Ҫ������Щ�������û���ķ�ֵ����ĸ���ͬ�������в�ͬ�Ĵ���
 * һ�������ֵ��"WIN_VALUE"����(������"-WIN_VALUE"��"WIN_VALUE"֮�䣬��ͬ)����ֻ��ȡ�����������Ҫ��ľ��棻
 * ���������ֵ��"WIN_VALUE"��"BAN_VALUE"֮�䣬���ܻ�ȡ�û����е�ֵ(ֻ�ܻ�ȡ����ŷ������ο�)��Ŀ���Ƿ�ֹ���ڳ��������µġ��û���Ĳ��ȶ��ԡ���
 * ���������ֵ��"BAN_VALUE"���⣬���ȡ����ʱ���ؿ����������Ҫ����Ϊ��Щ�����Ѿ���֤����ɱ���ˣ�
 * �ġ������ֵ��"DrawValue()"(�ǵ�һ��������������)�����ܻ�ȡ�û����е�ֵ(ԭ����ڶ��������ͬ)��
 * ע�⣺���ڵ����������Ҫ��ɱ�岽�����е�����
 */
void ClearHash(void) 
{         // ����û���
    memset(hshItems, 0, (nHashMask + 1) * sizeof(HashStruct));
}
void NewHash(int nHashScale) 
{ // �����û�����С�� 2^nHashScale �ֽ�
    nHashMask = ((1 << nHashScale) / sizeof(HashStruct)) - 1;
    hshItems = new HashStruct[nHashMask + 1];
    ClearHash();
}

void DelHash(void)
 {           // �ͷ��û���
    delete[] hshItems;
}
bool HASH_POS_EQUAL(const HashStruct& hsh) 
{
    return hsh.dwLock0 == board.zobr.dwLock0 && hsh.dwLock1 == board.zobr.dwLock1;
}
// ������Ͳ�����ȡ�û�����(����һ�����ã����Զ��丳ֵ)
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
//��ȡ�û�����
int LookUpHash(int Alpha, int Beta, int nDepth, bool bNoNull, int& mv)
{
    HashStruct hsh;
    int i, vl;
    bool bBanNode, bMateNode;
    // ��ȡ�û��������Ϣ�Ĺ��̰������¼������裺

    // 1. ����ȡ�û�����
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

    // 2. �ж��Ƿ����Beta�߽�
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

    // 3. �ж��Ƿ����Alpha�߽�
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

//�����û�����
void StoreHash(int flag, int mv, int vl, int nDepth)
{
    HashStruct hsh;
    int i, nHashDepth, nMinDepth, nMinLayer;
    // �洢�û��������Ϣ�Ĺ��̰������¼������裺

    // 1. �Է�ֵ��ɱ�岽��������
    if (vl > WIN_VALUE) 
    {
        if (mv == 0 && vl <= BAN_VALUE) 
        {
            return; // ���³����ľ���(�������û��ü�)���������ŷ�Ҳû�У���ôû�б�Ҫд���û���
        }
        vl += board.nowDepth;
    }
    else if (vl < -WIN_VALUE) 
    {
        if (mv == 0 && vl >= -BAN_VALUE) 
        {
            return; // ͬ��
        }
        vl -= board.nowDepth;
    }
    else if (vl == board.DrawValue() && mv == 0) 
    {
        return;   // ͬ��
    }

    // 2. �����̽�û���
    nMinDepth = 512;
    nMinLayer = 0;
    for (i = 0; i < HASH_LAYERS; i++) 
    {
        hsh = HASH_ITEM(i);

        // 3. �����̽��һ���ľ��棬��ô�����û�����Ϣ���ɣ�
        if (HASH_POS_EQUAL(hsh)) 
        {
            // �����ȸ�����߽߱���С�����ɸ����û����ֵ
            if ((flag & HASH_ALPHA) != 0 && (hsh.ucAlphaDepth <= nDepth || hsh.svlAlpha >= vl)) 
            {
                hsh.ucAlphaDepth = nDepth;
                hsh.svlAlpha = vl;
            }
            // Beta���Ҫע�⣺��Ҫ��Null-Move�Ľ�㸲�������Ľ��
            if ((flag & HASH_BETA) != 0 && (hsh.ucBetaDepth <= nDepth || hsh.svlBeta <= vl) && (mv != 0 || hsh.wmv == 0)) 
            {
                hsh.ucBetaDepth = nDepth;
                hsh.svlBeta = vl;
            }
            // ����ŷ���ʼ�ո��ǵ�
            if (mv != 0) 
            {
                hsh.wmv = mv;
            }
            HASH_ITEM(i) = hsh;
            return;
        }

        // 4. �������һ���ľ��棬��ô��������С���û����
        nHashDepth = MAX((hsh.ucAlphaDepth == 0 ? 0 : hsh.ucAlphaDepth + 256),
            (hsh.wmv == 0 ? hsh.ucBetaDepth : hsh.ucBetaDepth + 256));
        if (nHashDepth < nMinDepth) 
        {
            nMinDepth = nHashDepth;
            nMinLayer = i;
        }
    }

    // 5. ��¼�û���
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