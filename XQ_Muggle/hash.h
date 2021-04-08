#include "board.h"
#ifndef HASH_H
#define HASH_H

const int MATE_VALUE = 10000;           // 将死的分值
const int WIN_VALUE = MATE_VALUE - 200; // 搜索出胜负的分值界限，超出此值就说明已经搜索出杀棋了
const int BAN_VALUE = MATE_VALUE - 100;  // 长将判负的分值，用于判断结点是否要写入置换表

const int HASH_SIZE = 1 << 20; // 置换表大小
const int HASH_ALPHA = 1;      // ALPHA节点的置换表项
const int HASH_BETA = 2;       // BETA节点的置换表项
const int HASH_PV = 3;         // PV节点的置换表项

struct HashItem
{
    uint8_t depth,flag;	                // 搜索深度,标志位
    int16_t svl;                        // 分值
    uint16_t wmv;	                    // 最佳着法
    uint32_t dwLock0, dwLock1;	        // 两个校验码，防止存取冲突
};
HashItem hashTable[HASH_SIZE];          // 置换表

//提取置换表项
int LookUpHash(boardStruct& board, int Alpha, int Beta, int nDepth, int& mv)
{
    bool isMate = false;    //杀棋标志，若是杀棋则不需要满足深度条件
    HashItem hsh = hashTable[board.zobr.dwKey & (HASH_SIZE - 1)]; //在置换表中查询是否命中
    if (hsh.dwLock0 != board.zobr.dwLock0 || hsh.dwLock1 != board.zobr.dwLock1)
    {
        mv = 0;
        return -MATE_VALUE;//不命中
    }
    mv = hsh.wmv;

    if (hsh.svl > WIN_VALUE)
    {
        if (nDepth > 0 && hsh.svl < BAN_VALUE)      //可能导致搜索的不稳定性，但最佳着法可能拿到
            return -MATE_VALUE;                     //允许根节点写入置换表，因为需要靠其获得最佳着法
        hsh.svl -= board.nowDepth;                  //胜利局面的特殊处理
        isMate = true;
    }
    else if (hsh.svl < -WIN_VALUE) {
        if (nDepth > 0 && hsh.svl > -BAN_VALUE)     //同上
            return -MATE_VALUE;
        hsh.svl += board.nowDepth;
        isMate = true;
    }
    if (hsh.depth >= nDepth || isMate) {                   //能否利用置换表的两个因素：深度是否达到条件/边界判定
        if (hsh.flag == HASH_BETA)
            return (hsh.svl >= Beta ? hsh.svl : -MATE_VALUE);
        else if (hsh.flag == HASH_ALPHA)
            return (hsh.svl <= Alpha ? hsh.svl : -MATE_VALUE);
        return hsh.svl;
    }
    return -MATE_VALUE;
}

//保存置换表项
void StoreHash(boardStruct& board, int flag, int mv, int vl, int nDepth)
{
    HashItem hsh = hashTable[board.zobr.dwKey & (HASH_SIZE - 1)];
    if (hsh.depth > nDepth) {     //深度优先的替换策略
        return;
    }
    hsh.flag = flag;
    hsh.depth = nDepth;
    if (vl > WIN_VALUE)
    {
        if (mv == 0 && vl <= BAN_VALUE)
            return; // 可能导致搜索的不稳定性且没有最佳着法，退出
        hsh.svl = vl + board.nowDepth;
    }
    else if (vl < -WIN_VALUE)
    {
        if (mv == 0 && vl >= -BAN_VALUE)
            return; // 同上
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