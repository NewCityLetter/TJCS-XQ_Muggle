#include "board.h"
#ifndef HASH_H
#define HASH_H

const int MATE_VALUE = 10000;           // �����ķ�ֵ
const int WIN_VALUE = MATE_VALUE - 200; // ������ʤ���ķ�ֵ���ޣ�������ֵ��˵���Ѿ�������ɱ����
const int BAN_VALUE = MATE_VALUE - 100;  // �����и��ķ�ֵ�������жϽ���Ƿ�Ҫд���û���

const int HASH_SIZE = 1 << 20; // �û����С
const int HASH_ALPHA = 1;      // ALPHA�ڵ���û�����
const int HASH_BETA = 2;       // BETA�ڵ���û�����
const int HASH_PV = 3;         // PV�ڵ���û�����

struct HashItem
{
    uint8_t depth,flag;	                // �������,��־λ
    int16_t svl;                        // ��ֵ
    uint16_t wmv;	                    // ����ŷ�
    uint32_t dwLock0, dwLock1;	        // ����У���룬��ֹ��ȡ��ͻ
};
HashItem hashTable[HASH_SIZE];          // �û���

//��ȡ�û�����
int LookUpHash(boardStruct& board, int Alpha, int Beta, int nDepth, int& mv)
{
    bool isMate = false;    //ɱ���־������ɱ������Ҫ�����������
    HashItem hsh = hashTable[board.zobr.dwKey & (HASH_SIZE - 1)]; //���û����в�ѯ�Ƿ�����
    if (hsh.dwLock0 != board.zobr.dwLock0 || hsh.dwLock1 != board.zobr.dwLock1)
    {
        mv = 0;
        return -MATE_VALUE;//������
    }
    mv = hsh.wmv;

    if (hsh.svl > WIN_VALUE)
    {
        if (nDepth > 0 && hsh.svl < BAN_VALUE)      //���ܵ��������Ĳ��ȶ��ԣ�������ŷ������õ�
            return -MATE_VALUE;                     //������ڵ�д���û�����Ϊ��Ҫ����������ŷ�
        hsh.svl -= board.nowDepth;                  //ʤ����������⴦��
        isMate = true;
    }
    else if (hsh.svl < -WIN_VALUE) {
        if (nDepth > 0 && hsh.svl > -BAN_VALUE)     //ͬ��
            return -MATE_VALUE;
        hsh.svl += board.nowDepth;
        isMate = true;
    }
    if (hsh.depth >= nDepth || isMate) {                   //�ܷ������û�����������أ�����Ƿ�ﵽ����/�߽��ж�
        if (hsh.flag == HASH_BETA)
            return (hsh.svl >= Beta ? hsh.svl : -MATE_VALUE);
        else if (hsh.flag == HASH_ALPHA)
            return (hsh.svl <= Alpha ? hsh.svl : -MATE_VALUE);
        return hsh.svl;
    }
    return -MATE_VALUE;
}

//�����û�����
void StoreHash(boardStruct& board, int flag, int mv, int vl, int nDepth)
{
    HashItem hsh = hashTable[board.zobr.dwKey & (HASH_SIZE - 1)];
    if (hsh.depth > nDepth) {     //������ȵ��滻����
        return;
    }
    hsh.flag = flag;
    hsh.depth = nDepth;
    if (vl > WIN_VALUE)
    {
        if (mv == 0 && vl <= BAN_VALUE)
            return; // ���ܵ��������Ĳ��ȶ�����û������ŷ����˳�
        hsh.svl = vl + board.nowDepth;
    }
    else if (vl < -WIN_VALUE)
    {
        if (mv == 0 && vl >= -BAN_VALUE)
            return; // ͬ��
        hsh.svl = vl - board.nowDepth;
    }
    else {
        hsh.svl = vl;
    }
    hsh.wmv = mv;
    hsh.dwLock0 = board.zobr.dwLock0;
    hsh.dwLock1 = board.zobr.dwLock1;
    hashTable[board.zobr.dwKey & (HASH_SIZE - 1)] = hsh;
}
#endif