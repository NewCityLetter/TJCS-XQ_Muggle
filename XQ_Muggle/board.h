#ifndef BOARD_H
#define BOARD_H
#pragma once

#include "base.h"
#include "zobrist.h"
#include "premove.h"

extern int AdvancedVal;                      //先行权因素的分值
extern int redValueTable[7][256];        //计算出的子力价值表
extern int blackValueTable[7][256];        //计算出的子力价值表
extern int HollowThreatVal[16];//空头炮威胁分值
extern int RedBottomThreatVal[16];//沉底炮威胁分值
extern int BlackBottomThreatVal[16];
extern int BlackAdvisorLeakageVal;//缺士怕双车的罚分
extern int RedAdvisorLeakageVal;
//FROM为起始编号 TO为结束编号
const int32 KING_FROM = 0;
const int32 ADVISOR_FROM = 1;
const int32 ADVISOR_TO = 2;
const int32 BISHOP_FROM = 3;
const int32 BISHOP_TO = 4;
const int32 KNIGHT_FROM = 5;
const int32 KNIGHT_TO = 6;
const int32 ROOK_FROM = 7;
const int32 ROOK_TO = 8;
const int32 CANNON_FROM = 9;
const int32 CANNON_TO = 10;
const int32 PAWN_FROM = 11;
const int32 PAWN_TO = 15;

const int32 KING = 0;
const int32 ADVISOR= 1;
const int32 BISHOP = 2;
const int32 KNIGHT = 3;
const int32 ROOK = 4;
const int32 CANNON = 5;
const int32 PAWN = 6;

const int EVALUATE_LEVEL_1 = 160;
const int EVALUATE_LEVEL_2 = 80;
const int EVALUATE_LEVEL_3 = 40;
const int EVALUATE_LEVEL_4 = 20;

// 判断棋子是否在棋盘中的数组
static const bool InBoard[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// 判断棋子是否在九宫的数组
static const bool InFort[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


/* 帅(将)的步长*/
static const int32 KingDelta[4] = { -16, -1, 1, 16 };

/*仕(士)的步长(移动两次当做相移动一次，减少一个数组)*/
static const int32 AdvisorDelta[4] = { -17, -15, 15, 17 };

/*马的步长，以帅(将)的步长作为马腿*/
static const int32 KnightDelta[4][2] = { {-33, -31}, {-18, 14}, {-14, 18}, {31, 33} };

/*马被将军的步长，以仕(士)的步长作为马腿*/
static const int32 KnightCheckDelta[4][2] = { {-33, -18}, {-31, -14}, {14, 31}, {18, 33} };

// 炮镇窝心马的威胁分值，行号 0-16 或一般中炮威胁
const int32 CENTRAL_THREAT[16] = 
{
    0,  0,  0,  0,  0,  0, 50, 45, 40, 35, 30, 30, 30,  0,  0,  0
};

// 不利于马的位置
const int32 KnigntBadPos[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* 棋子序号对应的棋子类型
   每方的棋子顺序依次是：帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 */
static const int32 pieceTypes[48] = 
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

// 常数表"ValuableStringPieces"用判断牵制是否有价值
// 大于0的项是对于车来说的，牵制马和炮(被牵制)都有价值，大于1的项是对于炮来说只有牵制马才有价值
static const int ValuableStringPieces[48] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0
};

// "StringValueTabVal"是类似"KNIGHT_PIN_TAB"的常数表(参阅"pregen.h")，决定牵制价值
// 中间子和被牵制子的距离越近，牵制的价值就越大
static const char StringValueTabVal[512] = {
                               0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
  12, 16, 20, 24, 28, 32, 36,  0, 36, 32, 28, 24, 20, 16, 12,  0,
   0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0
};

//获取己方棋子异或信息
inline int32 SELF_SIDE(int32 sidePlayer)
{
    return 16 + (sidePlayer << 4);
}
//获取对方棋子异或信息
inline int32 OPPO_SIDE(int32 sidePlayer)
{
    return 32 - (sidePlayer << 4);
}
//获取当前棋子类型
inline int32 PIECE_INDEX(int32 chessPiece)
{
    return chessPiece & 15;
}
// 判断棋子是否在九宫中
inline bool IN_FORT(int32 pos)
{
    return InFort[pos];
}
// 判断棋子是否在棋盘中
inline bool IN_BOARD(int32 pos)
{
    return InBoard[pos];
}
//向前移动一步
inline int32 SQUARE_FORWARD(int32 position, int32 sidePlayer)
{
    return position - 16 + (sidePlayer << 5);
}
// 是否未过河
inline bool HOME_HALF(int32 position, int32 sidePlayer)
{
    return (position & 0x80) != (sidePlayer << 7);
}
// 是否已过河
inline bool AWAY_HALF(int32 position, int32 sidePlayer)
{
    return (position & 0x80) == (sidePlayer << 7);
}
//由RecordMove获得起点坐标
inline int32 GETBEGIN(int32 Move)
{
    return Move & 255;
}
//由RecordMove获得终点坐标
inline int32 GETEND(int32 Move)
{
    return Move >> 8;
}
//判断棋子类型
inline int32 GETTYPE(int32 chessPiece)
{
    return pieceTypes[chessPiece];
}
//终点放在左八位，起点放在右八位
inline int32 RecordMove(int32 beginPos, int32 endPos)
{
    return beginPos + (endPos << 8);
}
//获取当前坐标编号
inline int32 GetOrder(int32 line, int32 col)
{
    return (line << 4) + col;
}
//翻转格子
inline int32 SQUARE_FLIP(int32 pos) 
{
    return 254 - pos;
}

inline bool SAME_HALF(int32 beginPos, int32 endPos) 
{
  return ((beginPos ^ endPos) & 0x80) == 0;
}

//根据sq返回行数0 ~ 16
inline int32 GETLINE(int32 sq) {
    return sq >> 4;
}

//根据sq返回列数0 ~ 16
inline int32 GETCOL(int32 sq) {
    return sq & 15;
}


/********************************************************/

//同行
inline bool SAME_RANK(int32 beginPos, int32 endPos) 
{
    return ((beginPos ^ endPos) & 0xf0) == 0;
}

//同列
inline bool SAME_FILE(int32 beginPos, int32 endPos)
{
    return ((beginPos ^ endPos) & 0x0f) == 0;
}
/*****************************************************/
static int MvvLvaVal[16] = 
{
  5,1,1,1,1,3,3,4,4,3,3,2,2,2,2,2
};

extern void ClearKiller();

// 历史走法信息
struct MoveStruct
{
    int32 wmv;//前八位endpos，后八位beginpos
    int32 ucpcCaptured;//capture为被吃子编号
    bool ucbCheck;
    uint64 dwKey;//zobrist

    void Set(int32 mv, int32 pcCaptured, bool bCheck, uint64 dwKey_)
    {
        wmv = mv;
        ucpcCaptured = pcCaptured;
        ucbCheck = bCheck;
        dwKey = dwKey_;
    }
};

struct SearchMove
{
    int move;
    int val;
};

struct boardStruct
{
    int32 playerSide;//当前行走方R0/B1
    int32 currentBoard[256];
    /*
    当前棋盘16*16 [3-12][3-11]为棋盘
    16-31  帅仕仕相相马马车车炮炮兵兵兵兵兵(R)
    32-47  将士士象象马马车车炮炮卒卒卒卒卒(B)
    0表示无棋子
    */
    int32  currentPosition[48];//每个棋子当前位置，0表示被吃
    int32 redVal, blackVal;//红黑棋子的子力价值
    int32 bitLine[13],bitCol[12];
    int32 nowDepth, nowMoveNum;
    int32 historyTable[65536];
    MoveStruct mvsList[MAX_MOVES];//历史走法信息列表
    ZobristStruct zobr;// Zobrist


    void ClearBoard()//棋盘初始化
    {
        openBookKey = 0;
        playerSide = 0;
        nowDepth = 0;
        nowMoveNum = 0;
        redVal = 0;
        blackVal = 0;
        ClearKiller();
        memset(bitCol,0,sizeof(bitCol));
        memset(bitLine,0,sizeof(bitLine));
        memset(historyTable, 0, sizeof(historyTable));
        memset(currentBoard, 0, sizeof(currentBoard));
        memset(currentPosition, 0, sizeof(currentPosition));
        zobr.InitZero();
    }
    void UpdateHistory(int move, int depth)
    {
        historyTable[move] += depth * depth;
    }
    //获取当前currentPosition数组
    void GetCurrentPosition()
    {
        for (int line = 0x3; line <= 0xc; line++)
        {
            for (int col = 0x3; col <= 0xb; col++)
            {
                int nowOrder = GetOrder(line, col);
                if (currentBoard[nowOrder] != 0)
                {
                    currentPosition[currentBoard[nowOrder]] = nowOrder;
                }
                else currentPosition[currentBoard[nowOrder]] = 0;
            }
        }
    }
    

    //初始化红黑双方子力值
    void InitValue()
    {
        for (int chessPiece = 16; chessPiece < 48; chessPiece++)
        {
            if (currentPosition[chessPiece])
                AddPiece(currentPosition[chessPiece], chessPiece);
        }
        if (playerSide == 0)
        {
            zobr.Xor(Zobrist.Player, Player);
            openBookKey ^= Player;
        }
    }

    MoveStruct LastMove()
    {
        return mvsList[nowMoveNum-1];
    }
    
    /*
    int32 GenerateMove(int32* movesArray,bool capture=0)
    生成移动方案
    将移动方案传入moveArray数组，其中终点放在左八位，起点放在右八位
    返回总的着法数量
    capture默认为0，表示生成不吃子，1为生成吃子
    */
    int32 GenerateMove(SearchMove* movesArray,int capture=0)//1 capture 0 noncap
    {
        int32 numOfMoves = 0;
        int32 selfSide = SELF_SIDE(playerSide);//将棋子与之异或以判断归属
        int32 oppoSide = OPPO_SIDE(playerSide);
        for (int32 chessPieceFrom = selfSide; chessPieceFrom < selfSide+16; chessPieceFrom++)
        {
            if(!currentPosition[chessPieceFrom])continue;
            int32 beginPosition = currentPosition[chessPieceFrom];
            switch (PIECE_INDEX(chessPieceFrom))
            {
            case KING_FROM://将移动
                for (int32 i = 1; i <= preMove.kingPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.kingPreMove[i][beginPosition];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                    {
                        if (!capture)
                        {
                            if(!chessPieceTo)
                                movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                        }
                        else
                        {
                            if(chessPieceTo&oppoSide)
                            {
                                movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                                movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                            }
                        }
                    }

                }
                break;
            case ADVISOR_FROM://士移动
            case ADVISOR_TO:
                for (int32 i = 1; i <= preMove.advisorPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.advisorPreMove[i][beginPosition];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                    {
                        if (!capture)
                        {
                            if(!chessPieceTo)
                                movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                        }
                        else
                        {
                            if(chessPieceTo&oppoSide)
                            {
                                movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                                movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                            }
                        }
                    }
                }
                break;
            case BISHOP_FROM://象移动
            case BISHOP_TO:
                for (int32 i = 1; i <= preMove.bishopPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.bishopPreMove[i][beginPosition];
                    if(currentBoard[preMove.bishopEye[i][beginPosition]])
                        continue;
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                    {
                        if (!capture)
                        {
                            if(!chessPieceTo)
                                movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                        }
                        else
                        {
                            if(chessPieceTo&oppoSide)
                            {
                                movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                                movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                            }
                        }
                    }
                }
                break;
            case KNIGHT_FROM://马移动
            case KNIGHT_TO:
                for (int32 i = 1; i <= preMove.knightPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.knightPreMove[i][beginPosition];
                    if (currentBoard[preMove.knightLeg[i][beginPosition]])
                        continue;//蹩脚
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                    {
                        if (!capture)
                        {
                            if(!chessPieceTo)
                                movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                        }
                        else
                        {
                            if(chessPieceTo&oppoSide)
                            {
                                movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                                movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                            }
                        }
                    }
                }
                break;
            case ROOK_FROM://车移动
            case ROOK_TO:
            {
                if(capture)
                {
                    int line=beginPosition>>4;
                    int col=beginPosition&15;
                    int bitState=bitLine[line];
                    for (int32 i = 0; i <=1; i++)//行
                    {
                        int32 endPosition =beginPosition+preMove.rookLinePreMove[col][bitState][i]-col;
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (chessPieceTo & oppoSide)
                        {
                            movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                            movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                        }
                    }

                    bitState=bitCol[col];
                    for (int32 i = 0; i <=1; i++)//列
                    {
                        int32 endPosition =beginPosition+((preMove.rookColPreMove[line][bitState][i]-line)<<4);
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (chessPieceTo & oppoSide)
                        {
                            movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                            movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                        }
                    }
                }
                else
                {
                    int line=beginPosition>>4;
                    int col=beginPosition&15;
                    int bitState=bitLine[line];
                    for (int32 i = preMove.rookLinePreMove[col][bitState][0]; i <=preMove.rookLinePreMove[col][bitState][1]; i++)//
                    {
                        int32 endPosition =beginPosition+i-col;
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (!chessPieceTo)
                            movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                    }

                    bitState=bitCol[col];
                    for (int32 i = preMove.rookColPreMove[line][bitState][0]; i <=preMove.rookColPreMove[line][bitState][1]; i++)//
                    {
                        int32 endPosition =beginPosition+((i-line)<<4);
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (!chessPieceTo)
                            movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                    }
                }
                break;
            }
                
            case CANNON_FROM://炮移动
            case CANNON_TO:
            {
                int line=beginPosition>>4;
                int col=beginPosition&15;
                int bitState;
                if(capture)
                {
                    bitState=bitLine[line];
                    for (int32 i = 0; i <=1; i++)//行
                    {
                        if (!preMove.cannonLinePreCap[col][bitState][i])
                            continue;
                        int32 endPosition =beginPosition+preMove.cannonLinePreCap[col][bitState][i]-col;
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (chessPieceTo & oppoSide)
                        {
                            movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                            movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                        }
                    }

                    bitState=bitCol[col];
                    for (int32 i = 0; i <=1; i++)//列
                    {
                        if (!preMove.cannonColPreCap[line][bitState][i])
                            continue;
                        int32 endPosition =beginPosition+((preMove.cannonColPreCap[line][bitState][i]-line)<<4);
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (chessPieceTo & oppoSide)
                        {
                            movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                            movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                        }
                    }
                }
                
                if(!capture)
                {
                    bitState=bitLine[line];
                    for (int32 i = preMove.rookLinePreMove[col][bitState][0]; i <=preMove.rookLinePreMove[col][bitState][1]; i++)//
                    {
                        int32 endPosition =beginPosition+i-col;
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (!chessPieceTo)
                            movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                    }

                    bitState=bitCol[col];
                    for (int32 i = preMove.rookColPreMove[line][bitState][0]; i <=preMove.rookColPreMove[line][bitState][1]; i++)//
                    {
                        int32 endPosition =beginPosition+((i-line)<<4);
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (!chessPieceTo)
                            movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                    }
                }
                break;
            }
                
            default://兵移动
                for (int32 i = 1; i <= preMove.pawnPreMove[playerSide][0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.pawnPreMove[playerSide][i][beginPosition];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                    {
                        if (!capture)
                        {
                            if(!chessPieceTo)
                                movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                        }
                        else
                        {
                            if (chessPieceTo & oppoSide)
                            {
                                movesArray[numOfMoves].move = RecordMove(beginPosition, endPosition);
                                movesArray[numOfMoves++].val=MvvLva(beginPosition,endPosition);
                            }
                        }
                    }
                }
                break;
            }
        }
        return numOfMoves;//返回着法数量
    }

    int32 GenerateAllMove(SearchMove* movesArray)
    {
        int32 numOfMoves = 0;
        int32 selfSide = SELF_SIDE(playerSide);//将棋子与之异或以判断归属
        int32 oppoSide = OPPO_SIDE(playerSide);
        for (int32 chessPieceFrom = selfSide; chessPieceFrom < selfSide+16; chessPieceFrom++)
        {
            if(!currentPosition[chessPieceFrom])continue;
            int32 beginPosition = currentPosition[chessPieceFrom];
            switch (PIECE_INDEX(chessPieceFrom))
            {
            case KING_FROM://将移动
                for (int32 i = 1; i <= preMove.kingPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.kingPreMove[i][beginPosition];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);

                }
                break;
            case ADVISOR_FROM://士移动
            case ADVISOR_TO:
                for (int32 i = 1; i <= preMove.advisorPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.advisorPreMove[i][beginPosition];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                break;
            case BISHOP_FROM://象移动
            case BISHOP_TO:
                for (int32 i = 1; i <= preMove.bishopPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.bishopPreMove[i][beginPosition];
                    if(currentBoard[preMove.bishopEye[i][beginPosition]])
                        continue;
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                break;
            case KNIGHT_FROM://马移动
            case KNIGHT_TO:
                for (int32 i = 1; i <= preMove.knightPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.knightPreMove[i][beginPosition];
                    if (currentBoard[preMove.knightLeg[i][beginPosition]])
                        continue;//蹩脚
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                break;
            case ROOK_FROM://车移动
            case ROOK_TO:
            {
                int line=beginPosition>>4;
                int col=beginPosition&15;
                int bitState=bitLine[line];
                for (int32 i = preMove.rookLinePreMove[col][bitState][0]; i <=preMove.rookLinePreMove[col][bitState][1]; i++)//
                {
                    int32 endPosition =beginPosition+i-col;
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (chessPieceTo & selfSide)
                        continue;
                    movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }

                bitState=bitCol[col];
                for (int32 i = preMove.rookColPreMove[line][bitState][0]; i <=preMove.rookColPreMove[line][bitState][1]; i++)//
                {
                    int32 endPosition =beginPosition+((i-line)<<4);
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (chessPieceTo & selfSide)
                        continue;
                    movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                break;
            }
                
            case CANNON_FROM://炮移动
            case CANNON_TO:
            {
                int line=beginPosition>>4;
                int col=beginPosition&15;
                int bitState=bitLine[line];
                for (int32 i = 0; i <=1; i++)//行
                {
                    if (!preMove.cannonLinePreCap[col][bitState][i])
                        continue;
                    int32 endPosition =beginPosition+preMove.cannonLinePreCap[col][bitState][i]-col;
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (chessPieceTo & oppoSide)
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }

                bitState=bitCol[col];
                for (int32 i = 0; i <=1; i++)//列
                {
                    if (!preMove.cannonColPreCap[line][bitState][i])
                        continue;
                    int32 endPosition =beginPosition+((preMove.cannonColPreCap[line][bitState][i]-line)<<4);
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (chessPieceTo & oppoSide)
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                
                bitState=bitLine[line];
                for (int32 i = preMove.rookLinePreMove[col][bitState][0]; i <=preMove.rookLinePreMove[col][bitState][1]; i++)//
                {
                    int32 endPosition =beginPosition+i-col;
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (chessPieceTo)
                        continue;
                    movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }

                bitState=bitCol[col];
                for (int32 i = preMove.rookColPreMove[line][bitState][0]; i <=preMove.rookColPreMove[line][bitState][1]; i++)//
                {
                    int32 endPosition =beginPosition+((i-line)<<4);
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (chessPieceTo)
                        continue;
                    movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                
                break;
            }
                
            default://兵移动
                for (int32 i = 1; i <= preMove.pawnPreMove[playerSide][0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.pawnPreMove[playerSide][i][beginPosition];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                break;
            }
        }
        return numOfMoves;//返回着法数量
    }

    //交换走子
    void ChangeSide()
    {
        playerSide = 1 ^ playerSide;
        zobr.Xor(Zobrist.Player, Player);
        openBookKey ^= Player;
    }
    //局面评价函数
    //特殊棋形
    int SpeacialShape()
    {
        int redVal = 0, blackVal = 0,side=1;
        int kingNum=SELF_SIDE(side)+KING_FROM;
        int kingPos=currentPosition[kingNum];
        if (currentPosition[SELF_SIDE(side) + ADVISOR_FROM] && currentPosition[SELF_SIDE(side) + ADVISOR_TO])//双士全
        {
            if (kingPos == 0x37)//将在原位
            {
                if (pieceTypes[currentBoard[0x36]] == ADVISOR && pieceTypes[currentBoard[0x38]] == ADVISOR)//左右双士
                {
                    for (int i = OPPO_SIDE(side) + CANNON_FROM; i <= OPPO_SIDE(side) + CANNON_TO; i++)
                    {
                        int beginPos = currentPosition[i];
                        if (!beginPos)
                            continue;
                        if (SAME_FILE(beginPos, kingPos))
                        {

                            if (preMove.rookColPreMove[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][0] == GETLINE(kingPos))//空头炮
                            {
                                redVal += HollowThreatVal[GETLINE(beginPos)];
                                //std::cout<<"red hollow"<<std::endl;
                            }
                            else if (preMove.cannonSupCol[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][0] == GETLINE(kingPos) && (currentBoard[0x47] == SELF_SIDE(side) + KNIGHT_FROM || currentBoard[0x47] == SELF_SIDE(side) + KNIGHT_TO))//炮镇窝心马
                            {
                                redVal += CENTRAL_THREAT[GETLINE(beginPos)];
                                //std::cout<<"red central"<<std::endl;
                            }
                        }

                    }
                }
                else if (pieceTypes[currentBoard[0x47]] == ADVISOR && (pieceTypes[currentBoard[0x36]] == ADVISOR || pieceTypes[currentBoard[0x38]] == ADVISOR))//花心+底线 双士
                {
                    for (int i = OPPO_SIDE(side) + CANNON_FROM; i <= OPPO_SIDE(side) + CANNON_TO; i++)
                    {
                        int beginPos = currentPosition[i];
                        if (!beginPos)
                            continue;
                        if (SAME_FILE(beginPos, kingPos))
                        {
                            if (preMove.cannonSupCol[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][0] == GETLINE(kingPos))//普通中炮
                            {
                                redVal += (CENTRAL_THREAT[GETLINE(beginPos)] >> 2);

                                //将门被控
                                if (!currentBoard[0x36] && IsProtected(1 ^ side, 0x36))
                                    redVal += 20;
                                else if (!currentBoard[0x38] && IsProtected(1 ^ side, 0x38))
                                    redVal += 20;

                                //车在底线保护将扣分
                                for (int j = SELF_SIDE(side) + ROOK_FROM; j <= SELF_SIDE(side) + ROOK_TO; j++)
                                {
                                    int rookPos = currentPosition[j];
                                    if (!rookPos)
                                        continue;
                                    if (SAME_RANK(rookPos, kingPos))
                                    {
                                        if (preMove.rookLinePreMove[GETCOL(rookPos)][bitLine[GETLINE(rookPos)]][0] == GETLINE(kingPos))
                                            redVal += 80;
                                        if (preMove.rookLinePreMove[GETCOL(rookPos)][bitLine[GETLINE(rookPos)]][1] == GETLINE(kingPos))
                                            redVal += 80;
                                    }
                                }
                            }

                        }
                        else if (SAME_RANK(beginPos, kingPos))//沉底炮
                        {
                            if (preMove.rookLinePreMove[GETCOL(beginPos)][bitLine[GETLINE(beginPos)]][0] == GETCOL(kingPos)|| preMove.rookLinePreMove[GETCOL(beginPos)][bitLine[GETLINE(beginPos)]][1] == GETCOL(kingPos))
                                redVal += BlackBottomThreatVal[GETCOL(beginPos)];
                        }

                    }
                }
            }
            else if (kingPos == 0x47)//将在花心影响士
                redVal += 20;

        }
        else if (currentPosition[OPPO_SIDE(side) + ROOK_FROM] && currentPosition[OPPO_SIDE(side) + ROOK_TO])//缺士怕双车
            redVal += BlackAdvisorLeakageVal;


        side = 0;
        kingNum = SELF_SIDE(side) + KING_FROM;
        kingPos=currentPosition[kingNum];
        if (currentPosition[SELF_SIDE(side) + ADVISOR_FROM] && currentPosition[SELF_SIDE(side) + ADVISOR_TO])//双士全
        {
            if (kingPos == 0xc7)//帅在原位
            {
                if (pieceTypes[currentBoard[0xc6]] == ADVISOR && pieceTypes[currentBoard[0xc8]] == ADVISOR)//左右双士
                {
                    for (int i = OPPO_SIDE(side) + CANNON_FROM; i <= OPPO_SIDE(side) + CANNON_TO; i++)
                    {
                        int beginPos = currentPosition[i];
                        if (!beginPos)
                            continue;
                        if (SAME_FILE(beginPos, kingPos))
                        {
                            if (preMove.rookColPreMove[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][1] >= GETLINE(kingPos))//空头炮
                            {
                                blackVal += HollowThreatVal[15-GETLINE(beginPos)];
                            }
                            else if (preMove.cannonSupCol[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][1] == GETLINE(kingPos) && (currentBoard[0xb7] == SELF_SIDE(side) + KNIGHT_FROM || currentBoard[0xb7] == SELF_SIDE(side) + KNIGHT_TO))//炮镇窝心马
                            {
                                blackVal += CENTRAL_THREAT[15-GETLINE(beginPos)];
                            }
                        }

                    }
                }
                else if (pieceTypes[currentBoard[0xb7]] == ADVISOR && (pieceTypes[currentBoard[0xc6]] == ADVISOR || pieceTypes[currentBoard[0xc8]] == ADVISOR))//花心+底线 双士
                {
                    for (int i = OPPO_SIDE(side) + CANNON_FROM; i <= OPPO_SIDE(side) + CANNON_TO; i++)
                    {
                        int beginPos = currentPosition[i];
                        if (!beginPos)
                            continue;
                        if (SAME_FILE(beginPos, kingPos))
                        {
                            if (preMove.cannonSupCol[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][1] == GETLINE(kingPos))//普通中炮
                            {
                                blackVal += (CENTRAL_THREAT[15-GETLINE(beginPos)] >> 2);

                                //将门被控
                                if (!currentBoard[0xc6] && IsProtected(1 ^ side, 0xc6))
                                    blackVal += 20;
                                else if (!currentBoard[0xc8] && IsProtected(1 ^ side, 0xc8))
                                    blackVal += 20;

                                //车在底线保护将扣分
                                for (int j = SELF_SIDE(side) + ROOK_FROM; j <= SELF_SIDE(side) + ROOK_TO; j++)
                                {
                                    int rookPos = currentPosition[j];
                                    if (!rookPos)
                                        continue;
                                    if (SAME_RANK(rookPos, kingPos))
                                    {
                                        if (preMove.rookLinePreMove[GETCOL(rookPos)][bitLine[GETLINE(rookPos)]][0] == GETLINE(kingPos))
                                            blackVal += 80;
                                        if (preMove.rookLinePreMove[GETCOL(rookPos)][bitLine[GETLINE(rookPos)]][1] == GETLINE(kingPos))
                                            blackVal += 80;
                                    }
                                }
                            }

                        }
                        else if (SAME_RANK(beginPos, kingPos))//沉底炮
                        {
                            if (preMove.rookLinePreMove[GETCOL(beginPos)][bitLine[GETLINE(beginPos)]][0] == GETCOL(kingPos)|| preMove.rookLinePreMove[GETCOL(beginPos)][bitLine[GETLINE(beginPos)]][1] == GETCOL(kingPos))
                                blackVal += RedBottomThreatVal[GETCOL(beginPos)];
                        }

                    }
                }
            }
            else if (kingPos == 0xb7)//将在花心影响士
                blackVal += 20;

        }
        else if (currentPosition[OPPO_SIDE(side) + ROOK_FROM] && currentPosition[OPPO_SIDE(side) + ROOK_TO])//缺士怕双车
            blackVal += RedAdvisorLeakageVal;

        return playerSide == 0 ? (redVal - blackVal) : (blackVal - redVal);
    }

    int StringHold()
    {
        int holdVal[2];
        int beginPos,endPos;
        for(int side=0;side<=1;side++)
        {
            holdVal[side]=0;
            int selfSide=SELF_SIDE(side);
            int oppoSide=OPPO_SIDE(side);

            //车牵制
            for(int i=selfSide+ROOK_FROM;i<=selfSide+ROOK_TO;i++)
            {
                beginPos=currentPosition[i];
                if(!beginPos)
                    continue;

                //牵制对方将
                endPos=currentPosition[oppoSide+KING_FROM];
                int line=GETLINE(beginPos);
                int col=GETCOL(beginPos);
                if(line==GETLINE(endPos))//同一行
                {
                    int dirction=(beginPos<endPos)?1:0;
                    if(preMove.cannonLinePreCap[col][bitLine[line]][dirction]==GETCOL(endPos))
                    {
                        int midPos=beginPos-col+preMove.rookLinePreMove[col][bitLine[line]][dirction];
                        if((currentBoard[midPos]&selfSide)==0)
                        {
                            //牵制子有价值且不被其他子保护
                            if(ValuableStringPieces[currentBoard[midPos]]>0&&!IsProtected(1^side,midPos,oppoSide+KING_FROM))
                            {
                                holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                            }
                        }
                    }
                }
                if(col==GETCOL(endPos))//同一列
                {
                    int dirction=(beginPos<endPos)?1:0;
                    if(preMove.cannonColPreCap[line][bitCol[col]][dirction]==GETLINE(endPos))
                    {
                        int midPos=beginPos+((preMove.rookColPreMove[line][bitCol[col]][dirction]-line)<<4);
                        if((currentBoard[midPos]&selfSide)==0)
                        {
                            //牵制子有价值且不被其他子保护
                            if(ValuableStringPieces[currentBoard[midPos]]>0&&!IsProtected(1^side,midPos,oppoSide+KING_FROM))
                            {
                                holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                            }
                        }
                    }
                }

                //牵制对方车
                for(int j=oppoSide+ROOK_FROM;j<=oppoSide+ROOK_TO;j++)
                {
                    endPos=currentPosition[j];
                    if(!endPos)
                        continue;
                    int line=GETLINE(beginPos);
                    int col=GETCOL(beginPos);
                    if(line==GETLINE(endPos))//同一行
                    {
                        int dirction=(beginPos<endPos)?1:0;
                        if(preMove.cannonLinePreCap[col][bitLine[line]][dirction]==GETCOL(endPos))
                        {
                            int midPos=beginPos-col+preMove.rookLinePreMove[col][bitLine[line]][dirction];
                            if((currentBoard[midPos]&selfSide)==0)
                            {
                                //牵制子有价值且不被其他子保护且车无保护
                                if(ValuableStringPieces[currentBoard[midPos]]>0&&!IsProtected(1^side,midPos,j)&&!IsProtected(1^side,endPos))
                                {
                                    holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                                }
                            }
                        }
                    }
                    if(col==GETCOL(endPos))//同一列
                    {
                        int dirction=(beginPos<endPos)?1:0;
                        if(preMove.cannonColPreCap[line][bitCol[col]][dirction]==GETLINE(endPos))
                        {
                            int midPos=beginPos+((preMove.rookColPreMove[line][bitCol[col]][dirction]-line)<<4);
                            if((currentBoard[midPos]&selfSide)==0)
                            {
                                if(ValuableStringPieces[currentBoard[midPos]]>0&&!IsProtected(1^side,midPos,j)&&!IsProtected(1^side,endPos))
                                {
                                    holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                                }
                            }
                        }
                    }
                }
            }

            //炮牵制
            for(int i=selfSide+CANNON_FROM;i<=selfSide+CANNON_TO;i++)
            {
                beginPos=currentPosition[i];
                if(!beginPos)
                    continue;

                //牵制对方将
                endPos=currentPosition[oppoSide+KING_FROM];
                int line=GETLINE(beginPos);
                int col=GETCOL(beginPos);
                if(line==GETLINE(endPos))//同一行
                {
                    int dirction=(beginPos<endPos)?1:0;
                    if(preMove.cannonSupLine[col][bitLine[line]][dirction]==GETCOL(endPos))
                    {
                        int midPos=beginPos-col+preMove.cannonLinePreCap[col][bitLine[line]][dirction];
                        if((currentBoard[midPos]&selfSide)==0)
                        {
                            //牵制子有价值且不被其他子保护
                            if(ValuableStringPieces[currentBoard[midPos]]>1&&!IsProtected(1^side,midPos,oppoSide+KING_FROM))
                            {
                                holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                            }
                        }
                    }
                }
                if(col==GETCOL(endPos))//同一列
                {
                    int dirction=(beginPos<endPos)?1:0;
                    if(preMove.cannonSupCol[line][bitCol[col]][dirction]==GETLINE(endPos))
                    {
                        int midPos=beginPos+((preMove.cannonColPreCap[line][bitCol[col]][dirction]-line)<<4);
                        if((currentBoard[midPos]&selfSide)==0)
                        {
                            //牵制子有价值且不被其他子保护
                            if(ValuableStringPieces[currentBoard[midPos]]>1&&!IsProtected(1^side,midPos,oppoSide+KING_FROM))
                            {
                                holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                            }
                        }
                    }
                }

                //牵制对方车
                for(int j=oppoSide+ROOK_FROM;j<=oppoSide+ROOK_TO;j++)
                {
                    endPos=currentPosition[j];
                    if(!endPos)
                        continue;
                    int line=GETLINE(beginPos);
                    int col=GETCOL(beginPos);
                    if(line==GETLINE(endPos))//同一行
                    {
                        int dirction=(beginPos<endPos)?1:0;
                        if(preMove.cannonSupLine[col][bitLine[line]][dirction]==GETCOL(endPos))
                        {
                            int midPos=beginPos-col+preMove.cannonLinePreCap[col][bitLine[line]][dirction];
                            if((currentBoard[midPos]&selfSide)==0)
                            {
                                //牵制子有价值且不被其他子保护且车无保护
                                if(ValuableStringPieces[currentBoard[midPos]]>1&&!IsProtected(1^side,midPos,j)&&!IsProtected(1^side,endPos))
                                {
                                    holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                                }
                            }
                        }
                    }
                    if(col==GETCOL(endPos))//同一列
                    {
                        int dirction=(beginPos<endPos)?1:0;
                        if(preMove.cannonSupCol[line][bitCol[col]][dirction]==GETLINE(endPos))
                        {
                            int midPos=beginPos+((preMove.cannonColPreCap[line][bitCol[col]][dirction]-line)<<4);
                            if((currentBoard[midPos]&selfSide)==0)
                            {
                                //牵制子有价值且不被其他子保护且车无保护
                                if(ValuableStringPieces[currentBoard[midPos]]>1&&!IsProtected(1^side,midPos,j)&&!IsProtected(1^side,endPos))
                                {
                                    holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                                }
                            }
                        }
                    }
                }
            }
        }
        return playerSide==0?(holdVal[0]-holdVal[1]):(holdVal[1]-holdVal[0]);
    }


    //车的灵活性
    int RookMobility()
    {
        int rookMob[2] = { 0,0 };
        for (int side = 0; side <=1; side++)
        {
            for (int i = SELF_SIDE(side) + ROOK_FROM; i <= SELF_SIDE(side) + ROOK_TO; i++)
            {
                int beginPos = currentPosition[i];  //车当前的位置
                if (!beginPos)//车已经被吃
                    continue;

                int col=GETCOL(beginPos),line=GETLINE(beginPos);
                //右
                int endCol = preMove.rookLinePreMove[col][bitLine[line]][1];
                int beginCol = preMove.rookLinePreMove[col][bitLine[line]][0];
                if (currentBoard[beginPos-col+endCol] & SELF_SIDE(side))
                    rookMob[side]--;
                if (currentBoard[beginPos-col+beginCol] & SELF_SIDE(side))
                    rookMob[side]--;
                rookMob[side] += endCol-beginCol+(beginCol==col)+(endCol==col);

                //上下
                int endLine = preMove.rookColPreMove[line][bitCol[col]][1];
                int beginLine=preMove.rookColPreMove[line][bitCol[col]][0];
                if (currentBoard[beginPos+((endLine-line)<<4)] & SELF_SIDE(side))
                    rookMob[side]--;
                if (currentBoard[beginPos+((beginLine-line)<<4)] & SELF_SIDE(side))
                    rookMob[side]--;
                rookMob[side] += endLine - beginLine+(beginLine==line)+(endLine==line);


                //printf("\n\nline=%d col=%d beginline=%d endline=%d begincol=%d endcol=%d\n\n",line,col,beginLine,endLine,beginCol,endCol);
            }
        }
        //std::cout << "rookMobility_red：" << rookMob[0] << " ";
        //std::cout << "black:" << rookMob[1] << '\n';
        return (playerSide == 0 ? (rookMob[0] - rookMob[1]) : (rookMob[1] - rookMob[0])) >> 1;
    }
    int KnightBlock()
    {
        int knightPenalty[2] = { 0,0 };
        for (int side = 0; side < 2; side++)
        {
            for (int i = SELF_SIDE(side) + KNIGHT_FROM; i <= SELF_SIDE(side) + KNIGHT_TO; i++)
            {
                int count = 0;    //好走法的个数
                int beginPos = currentPosition[i];//马当前的位置
                if (!beginPos)//马已经被吃
                    continue;
                for (int j = 1; j <= preMove.knightPreMove[0][beginPos]; j++)
                {
                    int dst = preMove.knightPreMove[j][beginPos];
                    if (!currentBoard[preMove.knightLeg[j][beginPos]] &&   //不会蹩马腿
                        !KnigntBadPos[dst] && !(currentBoard[dst] & SELF_SIDE(side)) &&  //目标位置不是己方棋子且目标位置不是不利位置
                        !IsProtected(1 ^ side, dst))   //目标位置没有被对方的棋子守护
                        if ((++count) > 1)//已经搜到了大于一个的好走法，可以退出
                            break;
                }
                knightPenalty[side] += (count == 0 ? 10 : (count == 1 ? 5 : 0));
            }
        }
        return playerSide == 0 ?( -knightPenalty[0] + knightPenalty[1] ):( -knightPenalty[1] + knightPenalty[0]);
    }
    int Evaluate(int alpha,int beta)
    {

        // 1. 四级偷懒评价(彻底偷懒评价)，只包括子力平衡；
        int vl = (playerSide?(blackVal-redVal):(redVal-blackVal))+AdvancedVal;
        if (vl + EVALUATE_LEVEL_1 <= alpha) 
            return vl + EVALUATE_LEVEL_1;
        else if (vl - EVALUATE_LEVEL_1 >= beta) 
            return vl - EVALUATE_LEVEL_1;

        // 2. 三级偷懒评价，包括特殊棋型；
        vl += SpeacialShape();
        if (vl + EVALUATE_LEVEL_2 <= alpha)
            return vl + EVALUATE_LEVEL_2;
        else if (vl - EVALUATE_LEVEL_2 >= beta) 
            return vl - EVALUATE_LEVEL_2;

        // 3. 二级偷懒评价，包括牵制；
        vl += StringHold();
        if (vl + EVALUATE_LEVEL_3 <= alpha) 
            return vl + EVALUATE_LEVEL_3;
        else if (vl - EVALUATE_LEVEL_3 >= beta) 
            return vl - EVALUATE_LEVEL_3;

        // 4. 一级偷懒评价，包括车的灵活性；
        vl += RookMobility();
        if (vl + EVALUATE_LEVEL_4 <= alpha) 
            return vl + EVALUATE_LEVEL_4;
        else if (vl - EVALUATE_LEVEL_4 >= beta) 
            return vl - EVALUATE_LEVEL_4;

        // 5. 零级偷懒评价(完全评价)，包括马的阻碍。
        return vl + KnightBlock();
    }

    /*
    void DelPiece(int32 pos,int32 chessPiece)
    移除一个棋子 pos为移除棋子位置，chhessPiece为移除棋子编号
    */
    void DelPiece(int32 pos, int32 chessPiece)
    {
        currentBoard[pos] = 0;
        currentPosition[chessPiece] = 0;
        int32 pieceType = GETTYPE(chessPiece);//棋子具体类型，数值范围0-6

        //bitBoard修改
        bitLine[GETLINE(pos)]^=bitMaskLine[pos];
        bitCol[GETCOL(pos)]^=bitMaskCol[pos];

        // 红方减分，黑方加分
        if (chessPiece < 32)
        {
            //redVal -= cucvlPiecePos[pieceType][pos];
            redVal -= redValueTable[pieceType][pos];
            zobr.Xor(Zobrist.Table[pieceType][pos], Table[pieceType][pos]);
            if (openBookFlag)
                openBookKey ^= Table[pieceType][pos - ((pos & 15) << 1) + 14];
        }
        else
        {
            //blackVal -= cucvlPiecePos[pieceType][SQUARE_FLIP(pos)];//取值颠倒
            blackVal -= blackValueTable[pieceType][pos];//取值颠倒
            zobr.Xor(Zobrist.Table[pieceType + 7][pos], Table[pieceType + 7][pos]);
            if (openBookFlag)
                openBookKey ^= Table[pieceType + 7][pos - ((pos & 15) << 1) + 14];
        }
    }

    /*
    void AddPiece(int32 pos,int32 chessPiece)
    增加一个棋子 pos为增加棋子位置，chhessPiece为增加棋子编号
    */
    void AddPiece(int32 pos, int32 chessPiece)
    {
        currentBoard[pos] = chessPiece;
        currentPosition[chessPiece] = pos;
        int32 pieceType = GETTYPE(chessPiece);//棋子具体类型，数值范围0-6

        //bitBoard修改
        bitLine[GETLINE(pos)]^=bitMaskLine[pos];
        bitCol[GETCOL(pos)]^=bitMaskCol[pos];

        // 红方加分，黑方减分
        if (chessPiece < 32)
        {
            //redVal += cucvlPiecePos[pieceType][pos];
            redVal += redValueTable[pieceType][pos];
            zobr.Xor(Zobrist.Table[pieceType][pos], Table[pieceType][pos]);
            if (openBookFlag)
                openBookKey ^= Table[pieceType][pos-((pos&15)<<1)+14];
        }
        else
        {
            //blackVal += cucvlPiecePos[pieceType][SQUARE_FLIP(pos)];//取值颠倒
            blackVal += blackValueTable[pieceType][pos];//取值颠倒
            zobr.Xor(Zobrist.Table[pieceType + 7][pos], Table[pieceType + 7][pos]);
            if (openBookFlag)
                openBookKey ^= Table[pieceType+7][pos  - ((pos & 15)<<1)+ 14];
        }
    }
    
    /*
    int MakeMove(int32 Move)
    进行一次移动，Move前八位为终点，后八位为起点，返回一为合法
    */
    bool MakeMove(int32 Move,bool check=1)
    {
        uint64_t dwKey = zobr.dwKey;//记录局面
        int Capture = 0;
        int32 beginPos, endPos, chessPiece;
        beginPos = GETBEGIN(Move);
        endPos = GETEND(Move);
        chessPiece = currentBoard[beginPos];
        if (currentBoard[endPos])//如果发生吃子，则记录吃子编号，对局面估值进行修改
        {
            Capture = currentBoard[endPos];
            DelPiece(endPos, Capture);
        }
        DelPiece(beginPos, chessPiece);
        AddPiece(endPos, chessPiece);

        if(check)
        {
            if(InCheck())
            {
                DelPiece(endPos,chessPiece);
                AddPiece(beginPos,chessPiece);
                if(Capture)
                    AddPiece(endPos,Capture);
                return 0;
            }
        }
        
        ChangeSide();
        mvsList[nowMoveNum++].Set(Move, Capture,InCheck(), dwKey);//在移动列表中加入此次移动信息
        nowDepth++;//当前搜索深度++
        return 1;
    }

    /*
    void UndoMakeMove()
    撤销一次移动，Move前八位为终点，后八位为起点
    */
    void UndoMakeMove()
    {
        nowMoveNum--;
        nowDepth--;
        ChangeSide();
        //由于只能从局面得到Zobrist值，并不能从Zobrist值得到局面
        //所以要记录当前撤销着法是否吃子
        int32 beginPos, endPos, chessPiece;
        beginPos = GETBEGIN(mvsList[nowMoveNum].wmv);
        endPos = GETEND(mvsList[nowMoveNum].wmv);
        chessPiece = currentBoard[endPos];
        DelPiece(endPos, chessPiece);
        AddPiece(beginPos, chessPiece);
        if (mvsList[nowMoveNum].ucpcCaptured)
        {
            int Capture = mvsList[nowMoveNum].ucpcCaptured;
            AddPiece(endPos, Capture);
        }
    }

    bool InCheck()//由预置着法判断将军
    {
        int kingNum = SELF_SIDE(playerSide);//根据palyerSide获取将编号
        int kingPos = currentPosition[kingNum];
        int32 selfSide = SELF_SIDE(playerSide);//将棋子与之异或以判断归属
        int32 oppoSide = OPPO_SIDE(playerSide);
        int beginPos;

        //将见面
        for(int i=0;i<=1;i++)
        {
            if ((kingPos & 15) != (currentPosition[oppoSide + KING_FROM] & 15))
                continue;
            if(preMove.rookColPreMove[kingPos>>4][bitCol[kingPos&15]][i]== (currentPosition[oppoSide + KING_FROM]>>4))
                return 1;
        }

        //兵将军
        for(int i=oppoSide+PAWN_FROM;i<=oppoSide+PAWN_TO;i++)
        {
            beginPos=currentPosition[i];
            if(beginPos==0)
                continue;
            for(int j=1;j<=preMove.pawnPreMove[1-playerSide][0][beginPos];j++)
                if(preMove.pawnPreMove[1-playerSide][j][beginPos]==kingPos)
                    return 1;
        }

        //马将军
        for (int i=oppoSide+KNIGHT_FROM;i<=oppoSide+KNIGHT_TO;i++)
        {
            beginPos=currentPosition[i];
            if(beginPos==0)
                continue;
            for(int j=1;j<=preMove.knightPreMove[0][beginPos];j++)
                if(currentBoard[preMove.knightLeg[j][beginPos]]==0&&preMove.knightPreMove[j][beginPos]==kingPos)
                    return 1;
        }

        //车将军
        for (int i=oppoSide+ROOK_FROM;i<=oppoSide+ROOK_TO;i++)
        {
            beginPos=currentPosition[i];
            if(beginPos==0)
                continue;
            for(int j=0;j<=1;j++)//行
            {
                if((beginPos>>4)!=(kingPos>>4))
                    continue;
                if(preMove.rookLinePreMove[beginPos&15][bitLine[beginPos>>4]][j]==(kingPos&15))
                    return 1;
            }
            for(int j=0;j<=1;j++)//列
            {
                if((beginPos&15)!=(kingPos&15))
                    continue;
                if(preMove.rookColPreMove[beginPos>>4][bitCol[beginPos&15]][j]==(kingPos>>4))
                    return 1;
            }
        }

        //炮将军
        for (int i=oppoSide+CANNON_FROM;i<=oppoSide+CANNON_TO;i++)
        {
            beginPos=currentPosition[i];
            if(beginPos==0)
                continue;
            for(int j=0;j<=1;j++)//行
            {
                if((beginPos>>4)!=(kingPos>>4))
                    continue;
                if(preMove.cannonLinePreCap[beginPos&15][bitLine[beginPos>>4]][j]==(kingPos&15))
                    return 1;
            }
            for(int j=0;j<=1;j++)//列
            {
                if((beginPos&15)!=(kingPos&15))
                    continue;
                if(preMove.cannonColPreCap[beginPos>>4][bitCol[beginPos&15]][j]==(kingPos>>4))
                    return 1;
            }
        }
        
        return false;
    }

    void NullMove() // 走一步空步
    {         
        mvsList[nowMoveNum++].Set(0,0, 0, zobr.dwKey);  
        ChangeSide();
        nowDepth++;
    }

    void UndoNullMove() // 撤消走一步空步
    {
        nowMoveNum--;
        nowDepth--;
        ChangeSide();
    }

    /*
    A. 返回0，表示没有重复局面；
    B. 返回1，表示存在重复局面，但双方都无长将(判和)；
　  C. 返回3(=1+2)，表示存在重复局面，本方单方面长将(判本方负)；
　  D. 返回5(=1+4)，表示存在重复局面，对方单方面长将(判对方负)；
　　E. 返回7(=1+2+4)，表示存在重复局面，双方长将(判和)。
    */
    int RepStatus()
    {
        bool bSelfSide, bPerpCheck, bOppPerpCheck;
        const MoveStruct* lpmvs;

        bSelfSide = false;
        bPerpCheck = bOppPerpCheck = true;
        lpmvs = mvsList + nowMoveNum - 1;
        while (lpmvs != NULL && lpmvs->wmv != 0 && lpmvs->ucpcCaptured == 0)
        {
            if (bSelfSide)
            {
                bPerpCheck = bPerpCheck & lpmvs->ucbCheck;
                if (lpmvs->dwKey == zobr.dwKey)
                {
                    return 1 + (bPerpCheck ? 2 : 0) + (bOppPerpCheck ? 4 : 0);
                }
            }
            else
            {
                bOppPerpCheck = bOppPerpCheck & lpmvs->ucbCheck;
                if (lpmvs->dwKey == zobr.dwKey)
                {
                    return 1 + (bPerpCheck ? 2 : 0) + (bOppPerpCheck ? 4 : 0);
                }
            }
            bSelfSide = !bSelfSide;
            lpmvs--;
        }
        return 0;
    }

    int RepValue(int nRepStatus)
    {
        int vlReturn = ((nRepStatus & 2) ? (nowDepth - BAN_VALUE) : 0) +
            ((nRepStatus & 4) ? (BAN_VALUE - nowDepth) : 0);
        return vlReturn ? vlReturn : DrawValue();
    }

    /***************************************************************************/
    int DrawValue(void) const // 平局
    {                 
        return (nowDepth & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
    }
    //合理着法判断
    bool LegalMove(int32 mv) const 
    {
        int beginPos, endPos;
        int selfSide, chessPieceFrom,chessPieceTo;

        
        beginPos = GETBEGIN(mv);
        chessPieceFrom = currentBoard[beginPos];
        selfSide = SELF_SIDE(playerSide);
        if ((chessPieceFrom & selfSide) == 0) 
            return 0;

        endPos = GETEND(mv);
        chessPieceTo = currentBoard[endPos];
        if (chessPieceTo & selfSide)
            return 0;

        switch (PIECE_INDEX(chessPieceFrom)) 
        {
            case KING_FROM:
            {
                for (int i = 1; i <= preMove.kingPreMove[0][beginPos]; i++)
                {
                    if (preMove.kingPreMove[i][beginPos] == endPos)
                        return 1;
                }
                return 0;
            }
            case ADVISOR_FROM:
            case ADVISOR_TO:
            {
                for (int i = 1; i <= preMove.advisorPreMove[0][beginPos]; i++)
                {
                    if (preMove.advisorPreMove[i][beginPos] == endPos)
                        return 1;
                }
                return 0;
            }
            case BISHOP_FROM:
            case BISHOP_TO:
            {
                for (int i = 1; i <= preMove.bishopPreMove[0][beginPos]; i++)
                {
                    if (currentBoard[preMove.bishopEye[i][beginPos]]== 0&&preMove.bishopPreMove[i][beginPos] == endPos)
                        return 1;
                }
                return 0;
            }
            case KNIGHT_FROM:
            case KNIGHT_TO:
            {
                for (int i = 1; i <= preMove.knightPreMove[0][beginPos]; i++)
                {
                    if (currentBoard[preMove.knightLeg[i][beginPos]] == 0 && preMove.knightPreMove[i][beginPos] == endPos)
                        return 1;
                }
                return 0;
            }
                
            case ROOK_FROM:
            case ROOK_TO:
            {
                int line = beginPos >> 4;
                int col = beginPos & 15;
                int bitState = bitLine[line];
                for (int32 i = preMove.rookLinePreMove[col][bitState][0]; i <= preMove.rookLinePreMove[col][bitState][1]; i++)
                {
                    if (endPos == beginPos + i - col)
                        return 1;
                }

                bitState = bitCol[col];
                for (int32 i = preMove.rookColPreMove[line][bitState][0]; i <= preMove.rookColPreMove[line][bitState][1]; i++)
                {
                    if (endPos == beginPos + ((i - line) << 4))
                        return 1;
                }
                return 0;
            }
            case CANNON_FROM:
            case CANNON_TO:
            {
                int line = GETLINE(beginPos);
                int col = GETCOL(beginPos);
                int bitState = bitLine[line];
                for (int32 i = 0; i <= 1; i++)//行
                {
                    if (!preMove.cannonLinePreCap[col][bitState][i])
                        continue;
                    if (endPos == beginPos + preMove.cannonLinePreCap[col][bitState][i] - col)
                        return 1;
                }

                bitState = bitCol[col];
                for (int32 i = 0; i <= 1; i++)//列
                {
                    if (!preMove.cannonColPreCap[line][bitState][i])
                        continue;
                    if (endPos == beginPos + ((preMove.cannonColPreCap[line][bitState][i] - line) << 4))
                        return 1;
                }

                bitState = bitLine[line];
                for (int32 i = preMove.rookLinePreMove[col][bitState][0]; i <= preMove.rookLinePreMove[col][bitState][1]; i++)//??
                {
                    if (endPos == beginPos + i - col&&!currentBoard[endPos])
                        return 1;
                }

                bitState = bitCol[col];
                for (int32 i = preMove.rookColPreMove[line][bitState][0]; i <= preMove.rookColPreMove[line][bitState][1]; i++)//??
                {
                    if (endPos == beginPos + ((i - line) << 4)&& !currentBoard[endPos])
                        return 1;
                }
                return 0;
            }
            default:
                for (int i = 1; i <= preMove.pawnPreMove[playerSide][0][beginPos]; i++)
                {
                    if (preMove.pawnPreMove[playerSide][i][beginPos] == endPos)
                        return 1;
                }
                return 0;
        }
        return 0;
    }
    /***********************************************************************/
    bool IsProtected(int playerSide, int endPos,int except=0)
    {
        int selfSide = SELF_SIDE(playerSide);
        for (int chessPieceFrom = selfSide + KING_FROM; chessPieceFrom <= selfSide + PAWN_TO; chessPieceFrom++)
        {
            if(chessPieceFrom==except)
                continue;
            int beginPos = currentPosition[chessPieceFrom];
            if(!beginPos||beginPos==endPos)
                continue;
            switch (PIECE_INDEX(chessPieceFrom))
            {
                case KING_FROM:
                {
                    for (int i = 1; i <= preMove.kingPreMove[0][beginPos]; i++)
                    {
                        if (preMove.kingPreMove[i][beginPos] == endPos)
                            return 1;
                    }
                    break;
                }
                case ADVISOR_FROM:
                case ADVISOR_TO:
                {
                    for (int i = 1; i <= preMove.advisorPreMove[0][beginPos]; i++)
                    {
                        if (preMove.advisorPreMove[i][beginPos] == endPos)
                            return 1;
                    }
                    break;
                }
                case BISHOP_FROM:
                case BISHOP_TO:
                {
                    for (int i = 1; i <= preMove.bishopPreMove[0][beginPos]; i++)
                    {
                        if (currentBoard[preMove.bishopEye[i][beginPos]] == 0 && preMove.bishopPreMove[i][beginPos] == endPos)
                            return 1;
                    }
                    break;
                }
                case KNIGHT_FROM:
                case KNIGHT_TO:
                {
                    for (int i = 1; i <= preMove.knightPreMove[0][beginPos]; i++)
                    {
                        if (currentBoard[preMove.knightLeg[i][beginPos]] == 0 && preMove.knightPreMove[i][beginPos] == endPos)
                            return 1;
                    }
                    break;
                }

                case ROOK_FROM:
                case ROOK_TO:
                {
                    int line = beginPos >> 4;
                    int col = beginPos & 15;
                    int bitState = bitLine[line];
                    for (int32 i = preMove.rookLinePreMove[col][bitState][0]; i <= preMove.rookLinePreMove[col][bitState][1]; i++)
                    {
                        if (endPos == beginPos + i - col)
                            return 1;
                    }

                    bitState = bitCol[col];
                    for (int32 i = preMove.rookColPreMove[line][bitState][0]; i <= preMove.rookColPreMove[line][bitState][1]; i++)
                    {
                        if (endPos == beginPos + ((i - line) << 4))
                            return 1;
                    }
                    break;
                }
                case CANNON_FROM:
                case CANNON_TO:
                {
                    int line = beginPos >> 4;
                    int col = beginPos & 15;
                    int bitState = bitLine[line];
                    for (int32 i = 0; i <= 1; i++)//行
                    {
                        if (endPos == beginPos + preMove.cannonLinePreCap[col][bitState][i] - col)
                            return 1;
                    }

                    bitState = bitCol[col];
                    for (int32 i = 0; i <= 1; i++)//列
                    {
                        if (endPos == beginPos + ((preMove.cannonColPreCap[line][bitState][i] - line) << 4))
                            return 1;
                    }

                    bitState = bitLine[line];
                    for (int32 i = preMove.rookLinePreMove[col][bitState][0]-1; i >=3; i--)
                    {
                        if (currentBoard[beginPos + i - col])
                            break;
                        if (endPos == beginPos + i - col)
                            return 1;
                    }
                    for (int32 i = preMove.rookLinePreMove[col][bitState][1] + 1; i <12; i++)
                    {
                        if (currentBoard[beginPos + i - col])
                            break;
                        if (endPos == beginPos + i - col)
                            return 1;
                    }



                    bitState = bitCol[col];
                    for (int32 i = preMove.rookColPreMove[line][bitState][0]-1; i >=3; i--)
                    {
                        if (currentBoard[beginPos + ((i - line) << 4)])
                            break;
                        if (endPos == beginPos + ((i - line) << 4))
                            return 1;
                    }
                    for (int32 i = preMove.rookColPreMove[line][bitState][1] + 1; i <13; i++)
                    {
                        if (currentBoard[beginPos + ((i - line) << 4)])
                            break;
                        if (endPos == beginPos + ((i - line) << 4))
                            return 1;
                    }

                    break;
                }
                default:
                {
                    for (int i = 1; i <= preMove.pawnPreMove[playerSide][0][beginPos]; i++)
                    {
                        if (preMove.pawnPreMove[playerSide][i][beginPos] == endPos)
                            return 1;
                    }
                    break;
                }
                    
            }
        }
        return 0;
    }
    // 计算MVV/LVA值
    // 大于等于1才有价值，0无价值
    inline int MvvLva(int beginPos,int endPos) 
    {
        int32 beginPiece=currentBoard[beginPos]&15;
        int32 endPiece=currentBoard[endPos]&15;

        int mvv=MvvLvaVal[endPiece],lva=0;

        if(IsProtected(1^playerSide,endPos))
            lva=MvvLvaVal[beginPiece];

        if(mvv>=lva)
            return mvv-lva+1;
        return (mvv>=3||HOME_HALF(endPos,playerSide));
    }
};


#endif