#ifndef BOARD_H
#define BOARD_H
#pragma once

#include "base.h"
#include "zobrist.h"
#include "premove.h"

extern int AdvancedVal;                      //����Ȩ���صķ�ֵ
extern int redValueTable[7][256];        //�������������ֵ��
extern int blackValueTable[7][256];        //�������������ֵ��
extern int HollowThreatVal[16];//��ͷ����в��ֵ
extern int RedBottomThreatVal[16];//��������в��ֵ
extern int BlackBottomThreatVal[16];
extern int BlackAdvisorLeakageVal;//ȱʿ��˫���ķ���
extern int RedAdvisorLeakageVal;
//FROMΪ��ʼ��� TOΪ�������
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

// �ж������Ƿ��������е�����
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

// �ж������Ƿ��ھŹ�������
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


/* ˧(��)�Ĳ���*/
static const int32 KingDelta[4] = { -16, -1, 1, 16 };

/*��(ʿ)�Ĳ���(�ƶ����ε������ƶ�һ�Σ�����һ������)*/
static const int32 AdvisorDelta[4] = { -17, -15, 15, 17 };

/*��Ĳ�������˧(��)�Ĳ�����Ϊ����*/
static const int32 KnightDelta[4][2] = { {-33, -31}, {-18, 14}, {-14, 18}, {31, 33} };

/*�������Ĳ���������(ʿ)�Ĳ�����Ϊ����*/
static const int32 KnightCheckDelta[4][2] = { {-33, -18}, {-31, -14}, {14, 31}, {18, 33} };

// �������������в��ֵ���к� 0-16 ��һ��������в
const int32 CENTRAL_THREAT[16] = 
{
    0,  0,  0,  0,  0,  0, 50, 45, 40, 35, 30, 30, 30,  0,  0,  0
};

// ���������λ��
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

/* ������Ŷ�Ӧ����������
   ÿ��������˳�������ǣ�˧�����������������ڱ���������(��ʿʿ������������������������)
 */
static const int32 pieceTypes[48] = 
{
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
  0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
};

// ������"ValuableStringPieces"���ж�ǣ���Ƿ��м�ֵ
// ����0�����Ƕ��ڳ���˵�ģ�ǣ�������(��ǣ��)���м�ֵ������1�����Ƕ�������˵ֻ��ǣ������м�ֵ
static const int ValuableStringPieces[48] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0
};

// "StringValueTabVal"������"KNIGHT_PIN_TAB"�ĳ�����(����"pregen.h")������ǣ�Ƽ�ֵ
// �м��Ӻͱ�ǣ���ӵľ���Խ����ǣ�Ƶļ�ֵ��Խ��
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

//��ȡ�������������Ϣ
inline int32 SELF_SIDE(int32 sidePlayer)
{
    return 16 + (sidePlayer << 4);
}
//��ȡ�Է����������Ϣ
inline int32 OPPO_SIDE(int32 sidePlayer)
{
    return 32 - (sidePlayer << 4);
}
//��ȡ��ǰ��������
inline int32 PIECE_INDEX(int32 chessPiece)
{
    return chessPiece & 15;
}
// �ж������Ƿ��ھŹ���
inline bool IN_FORT(int32 pos)
{
    return InFort[pos];
}
// �ж������Ƿ���������
inline bool IN_BOARD(int32 pos)
{
    return InBoard[pos];
}
//��ǰ�ƶ�һ��
inline int32 SQUARE_FORWARD(int32 position, int32 sidePlayer)
{
    return position - 16 + (sidePlayer << 5);
}
// �Ƿ�δ����
inline bool HOME_HALF(int32 position, int32 sidePlayer)
{
    return (position & 0x80) != (sidePlayer << 7);
}
// �Ƿ��ѹ���
inline bool AWAY_HALF(int32 position, int32 sidePlayer)
{
    return (position & 0x80) == (sidePlayer << 7);
}
//��RecordMove����������
inline int32 GETBEGIN(int32 Move)
{
    return Move & 255;
}
//��RecordMove����յ�����
inline int32 GETEND(int32 Move)
{
    return Move >> 8;
}
//�ж���������
inline int32 GETTYPE(int32 chessPiece)
{
    return pieceTypes[chessPiece];
}
//�յ�������λ���������Ұ�λ
inline int32 RecordMove(int32 beginPos, int32 endPos)
{
    return beginPos + (endPos << 8);
}
//��ȡ��ǰ������
inline int32 GetOrder(int32 line, int32 col)
{
    return (line << 4) + col;
}
//��ת����
inline int32 SQUARE_FLIP(int32 pos) 
{
    return 254 - pos;
}

inline bool SAME_HALF(int32 beginPos, int32 endPos) 
{
  return ((beginPos ^ endPos) & 0x80) == 0;
}

//����sq��������0 ~ 16
inline int32 GETLINE(int32 sq) {
    return sq >> 4;
}

//����sq��������0 ~ 16
inline int32 GETCOL(int32 sq) {
    return sq & 15;
}


/********************************************************/

//ͬ��
inline bool SAME_RANK(int32 beginPos, int32 endPos) 
{
    return ((beginPos ^ endPos) & 0xf0) == 0;
}

//ͬ��
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

// ��ʷ�߷���Ϣ
struct MoveStruct
{
    int32 wmv;//ǰ��λendpos�����λbeginpos
    int32 ucpcCaptured;//captureΪ�����ӱ��
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
    int32 playerSide;//��ǰ���߷�R0/B1
    int32 currentBoard[256];
    /*
    ��ǰ����16*16 [3-12][3-11]Ϊ����
    16-31  ˧�����������������ڱ���������(R)
    32-47  ��ʿʿ������������������������(B)
    0��ʾ������
    */
    int32  currentPosition[48];//ÿ�����ӵ�ǰλ�ã�0��ʾ����
    int32 redVal, blackVal;//������ӵ�������ֵ
    int32 bitLine[13],bitCol[12];
    int32 nowDepth, nowMoveNum;
    int32 historyTable[65536];
    MoveStruct mvsList[MAX_MOVES];//��ʷ�߷���Ϣ�б�
    ZobristStruct zobr;// Zobrist


    void ClearBoard()//���̳�ʼ��
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
    //��ȡ��ǰcurrentPosition����
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
    

    //��ʼ�����˫������ֵ
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
    �����ƶ�����
    ���ƶ���������moveArray���飬�����յ�������λ���������Ұ�λ
    �����ܵ��ŷ�����
    captureĬ��Ϊ0����ʾ���ɲ����ӣ�1Ϊ���ɳ���
    */
    int32 GenerateMove(SearchMove* movesArray,int capture=0)//1 capture 0 noncap
    {
        int32 numOfMoves = 0;
        int32 selfSide = SELF_SIDE(playerSide);//��������֮������жϹ���
        int32 oppoSide = OPPO_SIDE(playerSide);
        for (int32 chessPieceFrom = selfSide; chessPieceFrom < selfSide+16; chessPieceFrom++)
        {
            if(!currentPosition[chessPieceFrom])continue;
            int32 beginPosition = currentPosition[chessPieceFrom];
            switch (PIECE_INDEX(chessPieceFrom))
            {
            case KING_FROM://���ƶ�
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
            case ADVISOR_FROM://ʿ�ƶ�
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
            case BISHOP_FROM://���ƶ�
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
            case KNIGHT_FROM://���ƶ�
            case KNIGHT_TO:
                for (int32 i = 1; i <= preMove.knightPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.knightPreMove[i][beginPosition];
                    if (currentBoard[preMove.knightLeg[i][beginPosition]])
                        continue;//����
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
            case ROOK_FROM://���ƶ�
            case ROOK_TO:
            {
                if(capture)
                {
                    int line=beginPosition>>4;
                    int col=beginPosition&15;
                    int bitState=bitLine[line];
                    for (int32 i = 0; i <=1; i++)//��
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
                    for (int32 i = 0; i <=1; i++)//��
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
                
            case CANNON_FROM://���ƶ�
            case CANNON_TO:
            {
                int line=beginPosition>>4;
                int col=beginPosition&15;
                int bitState;
                if(capture)
                {
                    bitState=bitLine[line];
                    for (int32 i = 0; i <=1; i++)//��
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
                    for (int32 i = 0; i <=1; i++)//��
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
                
            default://���ƶ�
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
        return numOfMoves;//�����ŷ�����
    }

    int32 GenerateAllMove(SearchMove* movesArray)
    {
        int32 numOfMoves = 0;
        int32 selfSide = SELF_SIDE(playerSide);//��������֮������жϹ���
        int32 oppoSide = OPPO_SIDE(playerSide);
        for (int32 chessPieceFrom = selfSide; chessPieceFrom < selfSide+16; chessPieceFrom++)
        {
            if(!currentPosition[chessPieceFrom])continue;
            int32 beginPosition = currentPosition[chessPieceFrom];
            switch (PIECE_INDEX(chessPieceFrom))
            {
            case KING_FROM://���ƶ�
                for (int32 i = 1; i <= preMove.kingPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.kingPreMove[i][beginPosition];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);

                }
                break;
            case ADVISOR_FROM://ʿ�ƶ�
            case ADVISOR_TO:
                for (int32 i = 1; i <= preMove.advisorPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.advisorPreMove[i][beginPosition];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                break;
            case BISHOP_FROM://���ƶ�
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
            case KNIGHT_FROM://���ƶ�
            case KNIGHT_TO:
                for (int32 i = 1; i <= preMove.knightPreMove[0][beginPosition]; i++)
                {
                    int32 endPosition = preMove.knightPreMove[i][beginPosition];
                    if (currentBoard[preMove.knightLeg[i][beginPosition]])
                        continue;//����
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }
                break;
            case ROOK_FROM://���ƶ�
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
                
            case CANNON_FROM://���ƶ�
            case CANNON_TO:
            {
                int line=beginPosition>>4;
                int col=beginPosition&15;
                int bitState=bitLine[line];
                for (int32 i = 0; i <=1; i++)//��
                {
                    if (!preMove.cannonLinePreCap[col][bitState][i])
                        continue;
                    int32 endPosition =beginPosition+preMove.cannonLinePreCap[col][bitState][i]-col;
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (chessPieceTo & oppoSide)
                        movesArray[numOfMoves++].move = RecordMove(beginPosition, endPosition);
                }

                bitState=bitCol[col];
                for (int32 i = 0; i <=1; i++)//��
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
                
            default://���ƶ�
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
        return numOfMoves;//�����ŷ�����
    }

    //��������
    void ChangeSide()
    {
        playerSide = 1 ^ playerSide;
        zobr.Xor(Zobrist.Player, Player);
        openBookKey ^= Player;
    }
    //�������ۺ���
    //��������
    int SpeacialShape()
    {
        int redVal = 0, blackVal = 0,side=1;
        int kingNum=SELF_SIDE(side)+KING_FROM;
        int kingPos=currentPosition[kingNum];
        if (currentPosition[SELF_SIDE(side) + ADVISOR_FROM] && currentPosition[SELF_SIDE(side) + ADVISOR_TO])//˫ʿȫ
        {
            if (kingPos == 0x37)//����ԭλ
            {
                if (pieceTypes[currentBoard[0x36]] == ADVISOR && pieceTypes[currentBoard[0x38]] == ADVISOR)//����˫ʿ
                {
                    for (int i = OPPO_SIDE(side) + CANNON_FROM; i <= OPPO_SIDE(side) + CANNON_TO; i++)
                    {
                        int beginPos = currentPosition[i];
                        if (!beginPos)
                            continue;
                        if (SAME_FILE(beginPos, kingPos))
                        {

                            if (preMove.rookColPreMove[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][0] == GETLINE(kingPos))//��ͷ��
                            {
                                redVal += HollowThreatVal[GETLINE(beginPos)];
                                //std::cout<<"red hollow"<<std::endl;
                            }
                            else if (preMove.cannonSupCol[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][0] == GETLINE(kingPos) && (currentBoard[0x47] == SELF_SIDE(side) + KNIGHT_FROM || currentBoard[0x47] == SELF_SIDE(side) + KNIGHT_TO))//����������
                            {
                                redVal += CENTRAL_THREAT[GETLINE(beginPos)];
                                //std::cout<<"red central"<<std::endl;
                            }
                        }

                    }
                }
                else if (pieceTypes[currentBoard[0x47]] == ADVISOR && (pieceTypes[currentBoard[0x36]] == ADVISOR || pieceTypes[currentBoard[0x38]] == ADVISOR))//����+���� ˫ʿ
                {
                    for (int i = OPPO_SIDE(side) + CANNON_FROM; i <= OPPO_SIDE(side) + CANNON_TO; i++)
                    {
                        int beginPos = currentPosition[i];
                        if (!beginPos)
                            continue;
                        if (SAME_FILE(beginPos, kingPos))
                        {
                            if (preMove.cannonSupCol[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][0] == GETLINE(kingPos))//��ͨ����
                            {
                                redVal += (CENTRAL_THREAT[GETLINE(beginPos)] >> 2);

                                //���ű���
                                if (!currentBoard[0x36] && IsProtected(1 ^ side, 0x36))
                                    redVal += 20;
                                else if (!currentBoard[0x38] && IsProtected(1 ^ side, 0x38))
                                    redVal += 20;

                                //���ڵ��߱������۷�
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
                        else if (SAME_RANK(beginPos, kingPos))//������
                        {
                            if (preMove.rookLinePreMove[GETCOL(beginPos)][bitLine[GETLINE(beginPos)]][0] == GETCOL(kingPos)|| preMove.rookLinePreMove[GETCOL(beginPos)][bitLine[GETLINE(beginPos)]][1] == GETCOL(kingPos))
                                redVal += BlackBottomThreatVal[GETCOL(beginPos)];
                        }

                    }
                }
            }
            else if (kingPos == 0x47)//���ڻ���Ӱ��ʿ
                redVal += 20;

        }
        else if (currentPosition[OPPO_SIDE(side) + ROOK_FROM] && currentPosition[OPPO_SIDE(side) + ROOK_TO])//ȱʿ��˫��
            redVal += BlackAdvisorLeakageVal;


        side = 0;
        kingNum = SELF_SIDE(side) + KING_FROM;
        kingPos=currentPosition[kingNum];
        if (currentPosition[SELF_SIDE(side) + ADVISOR_FROM] && currentPosition[SELF_SIDE(side) + ADVISOR_TO])//˫ʿȫ
        {
            if (kingPos == 0xc7)//˧��ԭλ
            {
                if (pieceTypes[currentBoard[0xc6]] == ADVISOR && pieceTypes[currentBoard[0xc8]] == ADVISOR)//����˫ʿ
                {
                    for (int i = OPPO_SIDE(side) + CANNON_FROM; i <= OPPO_SIDE(side) + CANNON_TO; i++)
                    {
                        int beginPos = currentPosition[i];
                        if (!beginPos)
                            continue;
                        if (SAME_FILE(beginPos, kingPos))
                        {
                            if (preMove.rookColPreMove[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][1] >= GETLINE(kingPos))//��ͷ��
                            {
                                blackVal += HollowThreatVal[15-GETLINE(beginPos)];
                            }
                            else if (preMove.cannonSupCol[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][1] == GETLINE(kingPos) && (currentBoard[0xb7] == SELF_SIDE(side) + KNIGHT_FROM || currentBoard[0xb7] == SELF_SIDE(side) + KNIGHT_TO))//����������
                            {
                                blackVal += CENTRAL_THREAT[15-GETLINE(beginPos)];
                            }
                        }

                    }
                }
                else if (pieceTypes[currentBoard[0xb7]] == ADVISOR && (pieceTypes[currentBoard[0xc6]] == ADVISOR || pieceTypes[currentBoard[0xc8]] == ADVISOR))//����+���� ˫ʿ
                {
                    for (int i = OPPO_SIDE(side) + CANNON_FROM; i <= OPPO_SIDE(side) + CANNON_TO; i++)
                    {
                        int beginPos = currentPosition[i];
                        if (!beginPos)
                            continue;
                        if (SAME_FILE(beginPos, kingPos))
                        {
                            if (preMove.cannonSupCol[GETLINE(beginPos)][bitCol[GETCOL(beginPos)]][1] == GETLINE(kingPos))//��ͨ����
                            {
                                blackVal += (CENTRAL_THREAT[15-GETLINE(beginPos)] >> 2);

                                //���ű���
                                if (!currentBoard[0xc6] && IsProtected(1 ^ side, 0xc6))
                                    blackVal += 20;
                                else if (!currentBoard[0xc8] && IsProtected(1 ^ side, 0xc8))
                                    blackVal += 20;

                                //���ڵ��߱������۷�
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
                        else if (SAME_RANK(beginPos, kingPos))//������
                        {
                            if (preMove.rookLinePreMove[GETCOL(beginPos)][bitLine[GETLINE(beginPos)]][0] == GETCOL(kingPos)|| preMove.rookLinePreMove[GETCOL(beginPos)][bitLine[GETLINE(beginPos)]][1] == GETCOL(kingPos))
                                blackVal += RedBottomThreatVal[GETCOL(beginPos)];
                        }

                    }
                }
            }
            else if (kingPos == 0xb7)//���ڻ���Ӱ��ʿ
                blackVal += 20;

        }
        else if (currentPosition[OPPO_SIDE(side) + ROOK_FROM] && currentPosition[OPPO_SIDE(side) + ROOK_TO])//ȱʿ��˫��
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

            //��ǣ��
            for(int i=selfSide+ROOK_FROM;i<=selfSide+ROOK_TO;i++)
            {
                beginPos=currentPosition[i];
                if(!beginPos)
                    continue;

                //ǣ�ƶԷ���
                endPos=currentPosition[oppoSide+KING_FROM];
                int line=GETLINE(beginPos);
                int col=GETCOL(beginPos);
                if(line==GETLINE(endPos))//ͬһ��
                {
                    int dirction=(beginPos<endPos)?1:0;
                    if(preMove.cannonLinePreCap[col][bitLine[line]][dirction]==GETCOL(endPos))
                    {
                        int midPos=beginPos-col+preMove.rookLinePreMove[col][bitLine[line]][dirction];
                        if((currentBoard[midPos]&selfSide)==0)
                        {
                            //ǣ�����м�ֵ�Ҳ��������ӱ���
                            if(ValuableStringPieces[currentBoard[midPos]]>0&&!IsProtected(1^side,midPos,oppoSide+KING_FROM))
                            {
                                holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                            }
                        }
                    }
                }
                if(col==GETCOL(endPos))//ͬһ��
                {
                    int dirction=(beginPos<endPos)?1:0;
                    if(preMove.cannonColPreCap[line][bitCol[col]][dirction]==GETLINE(endPos))
                    {
                        int midPos=beginPos+((preMove.rookColPreMove[line][bitCol[col]][dirction]-line)<<4);
                        if((currentBoard[midPos]&selfSide)==0)
                        {
                            //ǣ�����м�ֵ�Ҳ��������ӱ���
                            if(ValuableStringPieces[currentBoard[midPos]]>0&&!IsProtected(1^side,midPos,oppoSide+KING_FROM))
                            {
                                holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                            }
                        }
                    }
                }

                //ǣ�ƶԷ���
                for(int j=oppoSide+ROOK_FROM;j<=oppoSide+ROOK_TO;j++)
                {
                    endPos=currentPosition[j];
                    if(!endPos)
                        continue;
                    int line=GETLINE(beginPos);
                    int col=GETCOL(beginPos);
                    if(line==GETLINE(endPos))//ͬһ��
                    {
                        int dirction=(beginPos<endPos)?1:0;
                        if(preMove.cannonLinePreCap[col][bitLine[line]][dirction]==GETCOL(endPos))
                        {
                            int midPos=beginPos-col+preMove.rookLinePreMove[col][bitLine[line]][dirction];
                            if((currentBoard[midPos]&selfSide)==0)
                            {
                                //ǣ�����м�ֵ�Ҳ��������ӱ����ҳ��ޱ���
                                if(ValuableStringPieces[currentBoard[midPos]]>0&&!IsProtected(1^side,midPos,j)&&!IsProtected(1^side,endPos))
                                {
                                    holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                                }
                            }
                        }
                    }
                    if(col==GETCOL(endPos))//ͬһ��
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

            //��ǣ��
            for(int i=selfSide+CANNON_FROM;i<=selfSide+CANNON_TO;i++)
            {
                beginPos=currentPosition[i];
                if(!beginPos)
                    continue;

                //ǣ�ƶԷ���
                endPos=currentPosition[oppoSide+KING_FROM];
                int line=GETLINE(beginPos);
                int col=GETCOL(beginPos);
                if(line==GETLINE(endPos))//ͬһ��
                {
                    int dirction=(beginPos<endPos)?1:0;
                    if(preMove.cannonSupLine[col][bitLine[line]][dirction]==GETCOL(endPos))
                    {
                        int midPos=beginPos-col+preMove.cannonLinePreCap[col][bitLine[line]][dirction];
                        if((currentBoard[midPos]&selfSide)==0)
                        {
                            //ǣ�����м�ֵ�Ҳ��������ӱ���
                            if(ValuableStringPieces[currentBoard[midPos]]>1&&!IsProtected(1^side,midPos,oppoSide+KING_FROM))
                            {
                                holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                            }
                        }
                    }
                }
                if(col==GETCOL(endPos))//ͬһ��
                {
                    int dirction=(beginPos<endPos)?1:0;
                    if(preMove.cannonSupCol[line][bitCol[col]][dirction]==GETLINE(endPos))
                    {
                        int midPos=beginPos+((preMove.cannonColPreCap[line][bitCol[col]][dirction]-line)<<4);
                        if((currentBoard[midPos]&selfSide)==0)
                        {
                            //ǣ�����м�ֵ�Ҳ��������ӱ���
                            if(ValuableStringPieces[currentBoard[midPos]]>1&&!IsProtected(1^side,midPos,oppoSide+KING_FROM))
                            {
                                holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                            }
                        }
                    }
                }

                //ǣ�ƶԷ���
                for(int j=oppoSide+ROOK_FROM;j<=oppoSide+ROOK_TO;j++)
                {
                    endPos=currentPosition[j];
                    if(!endPos)
                        continue;
                    int line=GETLINE(beginPos);
                    int col=GETCOL(beginPos);
                    if(line==GETLINE(endPos))//ͬһ��
                    {
                        int dirction=(beginPos<endPos)?1:0;
                        if(preMove.cannonSupLine[col][bitLine[line]][dirction]==GETCOL(endPos))
                        {
                            int midPos=beginPos-col+preMove.cannonLinePreCap[col][bitLine[line]][dirction];
                            if((currentBoard[midPos]&selfSide)==0)
                            {
                                //ǣ�����м�ֵ�Ҳ��������ӱ����ҳ��ޱ���
                                if(ValuableStringPieces[currentBoard[midPos]]>1&&!IsProtected(1^side,midPos,j)&&!IsProtected(1^side,endPos))
                                {
                                    holdVal[side]+=StringValueTabVal[endPos-midPos+256];
                                }
                            }
                        }
                    }
                    if(col==GETCOL(endPos))//ͬһ��
                    {
                        int dirction=(beginPos<endPos)?1:0;
                        if(preMove.cannonSupCol[line][bitCol[col]][dirction]==GETLINE(endPos))
                        {
                            int midPos=beginPos+((preMove.cannonColPreCap[line][bitCol[col]][dirction]-line)<<4);
                            if((currentBoard[midPos]&selfSide)==0)
                            {
                                //ǣ�����м�ֵ�Ҳ��������ӱ����ҳ��ޱ���
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


    //���������
    int RookMobility()
    {
        int rookMob[2] = { 0,0 };
        for (int side = 0; side <=1; side++)
        {
            for (int i = SELF_SIDE(side) + ROOK_FROM; i <= SELF_SIDE(side) + ROOK_TO; i++)
            {
                int beginPos = currentPosition[i];  //����ǰ��λ��
                if (!beginPos)//���Ѿ�����
                    continue;

                int col=GETCOL(beginPos),line=GETLINE(beginPos);
                //��
                int endCol = preMove.rookLinePreMove[col][bitLine[line]][1];
                int beginCol = preMove.rookLinePreMove[col][bitLine[line]][0];
                if (currentBoard[beginPos-col+endCol] & SELF_SIDE(side))
                    rookMob[side]--;
                if (currentBoard[beginPos-col+beginCol] & SELF_SIDE(side))
                    rookMob[side]--;
                rookMob[side] += endCol-beginCol+(beginCol==col)+(endCol==col);

                //����
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
        //std::cout << "rookMobility_red��" << rookMob[0] << " ";
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
                int count = 0;    //���߷��ĸ���
                int beginPos = currentPosition[i];//��ǰ��λ��
                if (!beginPos)//���Ѿ�����
                    continue;
                for (int j = 1; j <= preMove.knightPreMove[0][beginPos]; j++)
                {
                    int dst = preMove.knightPreMove[j][beginPos];
                    if (!currentBoard[preMove.knightLeg[j][beginPos]] &&   //����������
                        !KnigntBadPos[dst] && !(currentBoard[dst] & SELF_SIDE(side)) &&  //Ŀ��λ�ò��Ǽ���������Ŀ��λ�ò��ǲ���λ��
                        !IsProtected(1 ^ side, dst))   //Ŀ��λ��û�б��Է��������ػ�
                        if ((++count) > 1)//�Ѿ��ѵ��˴���һ���ĺ��߷��������˳�
                            break;
                }
                knightPenalty[side] += (count == 0 ? 10 : (count == 1 ? 5 : 0));
            }
        }
        return playerSide == 0 ?( -knightPenalty[0] + knightPenalty[1] ):( -knightPenalty[1] + knightPenalty[0]);
    }
    int Evaluate(int alpha,int beta)
    {

        // 1. �ļ�͵������(����͵������)��ֻ��������ƽ�⣻
        int vl = (playerSide?(blackVal-redVal):(redVal-blackVal))+AdvancedVal;
        if (vl + EVALUATE_LEVEL_1 <= alpha) 
            return vl + EVALUATE_LEVEL_1;
        else if (vl - EVALUATE_LEVEL_1 >= beta) 
            return vl - EVALUATE_LEVEL_1;

        // 2. ����͵�����ۣ������������ͣ�
        vl += SpeacialShape();
        if (vl + EVALUATE_LEVEL_2 <= alpha)
            return vl + EVALUATE_LEVEL_2;
        else if (vl - EVALUATE_LEVEL_2 >= beta) 
            return vl - EVALUATE_LEVEL_2;

        // 3. ����͵�����ۣ�����ǣ�ƣ�
        vl += StringHold();
        if (vl + EVALUATE_LEVEL_3 <= alpha) 
            return vl + EVALUATE_LEVEL_3;
        else if (vl - EVALUATE_LEVEL_3 >= beta) 
            return vl - EVALUATE_LEVEL_3;

        // 4. һ��͵�����ۣ�������������ԣ�
        vl += RookMobility();
        if (vl + EVALUATE_LEVEL_4 <= alpha) 
            return vl + EVALUATE_LEVEL_4;
        else if (vl - EVALUATE_LEVEL_4 >= beta) 
            return vl - EVALUATE_LEVEL_4;

        // 5. �㼶͵������(��ȫ����)����������谭��
        return vl + KnightBlock();
    }

    /*
    void DelPiece(int32 pos,int32 chessPiece)
    �Ƴ�һ������ posΪ�Ƴ�����λ�ã�chhessPieceΪ�Ƴ����ӱ��
    */
    void DelPiece(int32 pos, int32 chessPiece)
    {
        currentBoard[pos] = 0;
        currentPosition[chessPiece] = 0;
        int32 pieceType = GETTYPE(chessPiece);//���Ӿ������ͣ���ֵ��Χ0-6

        //bitBoard�޸�
        bitLine[GETLINE(pos)]^=bitMaskLine[pos];
        bitCol[GETCOL(pos)]^=bitMaskCol[pos];

        // �췽���֣��ڷ��ӷ�
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
            //blackVal -= cucvlPiecePos[pieceType][SQUARE_FLIP(pos)];//ȡֵ�ߵ�
            blackVal -= blackValueTable[pieceType][pos];//ȡֵ�ߵ�
            zobr.Xor(Zobrist.Table[pieceType + 7][pos], Table[pieceType + 7][pos]);
            if (openBookFlag)
                openBookKey ^= Table[pieceType + 7][pos - ((pos & 15) << 1) + 14];
        }
    }

    /*
    void AddPiece(int32 pos,int32 chessPiece)
    ����һ������ posΪ��������λ�ã�chhessPieceΪ�������ӱ��
    */
    void AddPiece(int32 pos, int32 chessPiece)
    {
        currentBoard[pos] = chessPiece;
        currentPosition[chessPiece] = pos;
        int32 pieceType = GETTYPE(chessPiece);//���Ӿ������ͣ���ֵ��Χ0-6

        //bitBoard�޸�
        bitLine[GETLINE(pos)]^=bitMaskLine[pos];
        bitCol[GETCOL(pos)]^=bitMaskCol[pos];

        // �췽�ӷ֣��ڷ�����
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
            //blackVal += cucvlPiecePos[pieceType][SQUARE_FLIP(pos)];//ȡֵ�ߵ�
            blackVal += blackValueTable[pieceType][pos];//ȡֵ�ߵ�
            zobr.Xor(Zobrist.Table[pieceType + 7][pos], Table[pieceType + 7][pos]);
            if (openBookFlag)
                openBookKey ^= Table[pieceType+7][pos  - ((pos & 15)<<1)+ 14];
        }
    }
    
    /*
    int MakeMove(int32 Move)
    ����һ���ƶ���Moveǰ��λΪ�յ㣬���λΪ��㣬����һΪ�Ϸ�
    */
    bool MakeMove(int32 Move,bool check=1)
    {
        uint64_t dwKey = zobr.dwKey;//��¼����
        int Capture = 0;
        int32 beginPos, endPos, chessPiece;
        beginPos = GETBEGIN(Move);
        endPos = GETEND(Move);
        chessPiece = currentBoard[beginPos];
        if (currentBoard[endPos])//����������ӣ����¼���ӱ�ţ��Ծ����ֵ�����޸�
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
        mvsList[nowMoveNum++].Set(Move, Capture,InCheck(), dwKey);//���ƶ��б��м���˴��ƶ���Ϣ
        nowDepth++;//��ǰ�������++
        return 1;
    }

    /*
    void UndoMakeMove()
    ����һ���ƶ���Moveǰ��λΪ�յ㣬���λΪ���
    */
    void UndoMakeMove()
    {
        nowMoveNum--;
        nowDepth--;
        ChangeSide();
        //����ֻ�ܴӾ���õ�Zobristֵ�������ܴ�Zobristֵ�õ�����
        //����Ҫ��¼��ǰ�����ŷ��Ƿ����
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

    bool InCheck()//��Ԥ���ŷ��жϽ���
    {
        int kingNum = SELF_SIDE(playerSide);//����palyerSide��ȡ�����
        int kingPos = currentPosition[kingNum];
        int32 selfSide = SELF_SIDE(playerSide);//��������֮������жϹ���
        int32 oppoSide = OPPO_SIDE(playerSide);
        int beginPos;

        //������
        for(int i=0;i<=1;i++)
        {
            if ((kingPos & 15) != (currentPosition[oppoSide + KING_FROM] & 15))
                continue;
            if(preMove.rookColPreMove[kingPos>>4][bitCol[kingPos&15]][i]== (currentPosition[oppoSide + KING_FROM]>>4))
                return 1;
        }

        //������
        for(int i=oppoSide+PAWN_FROM;i<=oppoSide+PAWN_TO;i++)
        {
            beginPos=currentPosition[i];
            if(beginPos==0)
                continue;
            for(int j=1;j<=preMove.pawnPreMove[1-playerSide][0][beginPos];j++)
                if(preMove.pawnPreMove[1-playerSide][j][beginPos]==kingPos)
                    return 1;
        }

        //����
        for (int i=oppoSide+KNIGHT_FROM;i<=oppoSide+KNIGHT_TO;i++)
        {
            beginPos=currentPosition[i];
            if(beginPos==0)
                continue;
            for(int j=1;j<=preMove.knightPreMove[0][beginPos];j++)
                if(currentBoard[preMove.knightLeg[j][beginPos]]==0&&preMove.knightPreMove[j][beginPos]==kingPos)
                    return 1;
        }

        //������
        for (int i=oppoSide+ROOK_FROM;i<=oppoSide+ROOK_TO;i++)
        {
            beginPos=currentPosition[i];
            if(beginPos==0)
                continue;
            for(int j=0;j<=1;j++)//��
            {
                if((beginPos>>4)!=(kingPos>>4))
                    continue;
                if(preMove.rookLinePreMove[beginPos&15][bitLine[beginPos>>4]][j]==(kingPos&15))
                    return 1;
            }
            for(int j=0;j<=1;j++)//��
            {
                if((beginPos&15)!=(kingPos&15))
                    continue;
                if(preMove.rookColPreMove[beginPos>>4][bitCol[beginPos&15]][j]==(kingPos>>4))
                    return 1;
            }
        }

        //�ڽ���
        for (int i=oppoSide+CANNON_FROM;i<=oppoSide+CANNON_TO;i++)
        {
            beginPos=currentPosition[i];
            if(beginPos==0)
                continue;
            for(int j=0;j<=1;j++)//��
            {
                if((beginPos>>4)!=(kingPos>>4))
                    continue;
                if(preMove.cannonLinePreCap[beginPos&15][bitLine[beginPos>>4]][j]==(kingPos&15))
                    return 1;
            }
            for(int j=0;j<=1;j++)//��
            {
                if((beginPos&15)!=(kingPos&15))
                    continue;
                if(preMove.cannonColPreCap[beginPos>>4][bitCol[beginPos&15]][j]==(kingPos>>4))
                    return 1;
            }
        }
        
        return false;
    }

    void NullMove() // ��һ���ղ�
    {         
        mvsList[nowMoveNum++].Set(0,0, 0, zobr.dwKey);  
        ChangeSide();
        nowDepth++;
    }

    void UndoNullMove() // ������һ���ղ�
    {
        nowMoveNum--;
        nowDepth--;
        ChangeSide();
    }

    /*
    A. ����0����ʾû���ظ����棻
    B. ����1����ʾ�����ظ����棬��˫�����޳���(�к�)��
��  C. ����3(=1+2)����ʾ�����ظ����棬���������泤��(�б�����)��
��  D. ����5(=1+4)����ʾ�����ظ����棬�Է������泤��(�жԷ���)��
����E. ����7(=1+2+4)����ʾ�����ظ����棬˫������(�к�)��
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
    int DrawValue(void) const // ƽ��
    {                 
        return (nowDepth & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
    }
    //�����ŷ��ж�
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
                for (int32 i = 0; i <= 1; i++)//��
                {
                    if (!preMove.cannonLinePreCap[col][bitState][i])
                        continue;
                    if (endPos == beginPos + preMove.cannonLinePreCap[col][bitState][i] - col)
                        return 1;
                }

                bitState = bitCol[col];
                for (int32 i = 0; i <= 1; i++)//��
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
                    for (int32 i = 0; i <= 1; i++)//��
                    {
                        if (endPos == beginPos + preMove.cannonLinePreCap[col][bitState][i] - col)
                            return 1;
                    }

                    bitState = bitCol[col];
                    for (int32 i = 0; i <= 1; i++)//��
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
    // ����MVV/LVAֵ
    // ���ڵ���1���м�ֵ��0�޼�ֵ
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