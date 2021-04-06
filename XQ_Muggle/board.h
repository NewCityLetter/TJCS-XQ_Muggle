/*修改和增加的内容：
1.修改了GETTYPE函数，使其对棋子类型的判断的返回值从简单的0/1变成了可以返回棋子具体类型，实现方法：增加pieceTypes数组；
2.由1衍生出的“判断棋子是红方还是黑方”的方式从看GETTYPE返回值是0/1变成了“chessPiece是否<32”；
3.增加了boardStruct.Evaluate作为局面评价函数，依据是红黑双方的总子力值之差，定义常变量ADVANCED_VALUE=3；
4.增加了boardStruct.Drawvalue作为和棋分值，定义常变量DRAW_VALUE=20；
5.在move.cpp中增加“调整型局面评价函数”，并在AlphaBeta函数中补齐了return的空缺部分。
*/
#ifndef BOARD_H
#define BOARD_H
#pragma once

#include "base.h"
#include "zobrist.h"

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

const int ADVANCED_VALUE = 3;      //先行权值
const int DRAW_VALUE = 20;         //和棋时返回的分数(取负值)

// 判断棋子是否在棋盘中的数组
static const bool ccInBoard[256] = {
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
static const bool ccInFort[256] = {
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

// 判断步长是否符合特定走法的数组，1=帅(将)，2=仕(士)，3=相(象)
static const int8 ccLegalSpan[512] = {
                       0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};

// 根据步长判断马是否蹩腿的数组
static const int8 ccKnightPin[512] = {
                              0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,-16,  0,-16,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0, 16,  0, 16,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0
};

/* 帅(将)的步长*/
static const int8 ccKingDelta[4] = { -16, -1, 1, 16 };

/*仕(士)的步长(移动两次当做相移动一次，减少一个数组)*/
static const int8 ccAdvisorDelta[4] = { -17, -15, 15, 17 };

/*马的步长，以帅(将)的步长作为马腿*/
static const int8 ccKnightDelta[4][2] = { {-33, -31}, {-18, 14}, {-14, 18}, {31, 33} };

/*马被将军的步长，以仕(士)的步长作为马腿*/
static const int8 ccKnightCheckDelta[4][2] = { {-33, -18}, {-31, -14}, {14, 31}, {18, 33} };

// 子力位置价值表
static const uint8 cucvlPiecePos[7][256] = {
  { // 帅(将)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 11, 15, 11,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 仕(士)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0, 23,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0, 20,  0, 20,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 相(象)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 18,  0,  0,  0, 23,  0,  0,  0, 18,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0, 20,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 马
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 90, 90, 90, 96, 90, 96, 90, 90, 90,  0,  0,  0,  0,
    0,  0,  0, 90, 96,103, 97, 94, 97,103, 96, 90,  0,  0,  0,  0,
    0,  0,  0, 92, 98, 99,103, 99,103, 99, 98, 92,  0,  0,  0,  0,
    0,  0,  0, 93,108,100,107,100,107,100,108, 93,  0,  0,  0,  0,
    0,  0,  0, 90,100, 99,103,104,103, 99,100, 90,  0,  0,  0,  0,
    0,  0,  0, 90, 98,101,102,103,102,101, 98, 90,  0,  0,  0,  0,
    0,  0,  0, 92, 94, 98, 95, 98, 95, 98, 94, 92,  0,  0,  0,  0,
    0,  0,  0, 93, 92, 94, 95, 92, 95, 94, 92, 93,  0,  0,  0,  0,
    0,  0,  0, 85, 90, 92, 93, 78, 93, 92, 90, 85,  0,  0,  0,  0,
    0,  0,  0, 88, 85, 90, 88, 90, 88, 90, 85, 88,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 车
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,206,208,207,213,214,213,207,208,206,  0,  0,  0,  0,
    0,  0,  0,206,212,209,216,233,216,209,212,206,  0,  0,  0,  0,
    0,  0,  0,206,208,207,214,216,214,207,208,206,  0,  0,  0,  0,
    0,  0,  0,206,213,213,216,216,216,213,213,206,  0,  0,  0,  0,
    0,  0,  0,208,211,211,214,215,214,211,211,208,  0,  0,  0,  0,
    0,  0,  0,208,212,212,214,215,214,212,212,208,  0,  0,  0,  0,
    0,  0,  0,204,209,204,212,214,212,204,209,204,  0,  0,  0,  0,
    0,  0,  0,198,208,204,212,212,212,204,208,198,  0,  0,  0,  0,
    0,  0,  0,200,208,206,212,200,212,206,208,200,  0,  0,  0,  0,
    0,  0,  0,194,206,204,212,200,212,204,206,194,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 炮
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,100,100, 96, 91, 90, 91, 96,100,100,  0,  0,  0,  0,
    0,  0,  0, 98, 98, 96, 92, 89, 92, 96, 98, 98,  0,  0,  0,  0,
    0,  0,  0, 97, 97, 96, 91, 92, 91, 96, 97, 97,  0,  0,  0,  0,
    0,  0,  0, 96, 99, 99, 98,100, 98, 99, 99, 96,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 96, 96,100, 96, 96, 96, 96,  0,  0,  0,  0,
    0,  0,  0, 95, 96, 99, 96,100, 96, 99, 96, 95,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 96, 96, 96, 96, 96, 96, 96,  0,  0,  0,  0,
    0,  0,  0, 97, 96,100, 99,101, 99,100, 96, 97,  0,  0,  0,  0,
    0,  0,  0, 96, 97, 98, 98, 98, 98, 98, 97, 96,  0,  0,  0,  0,
    0,  0,  0, 96, 96, 97, 99, 99, 99, 97, 96, 96,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }, { // 兵(卒)
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  9,  9,  9, 11, 13, 11,  9,  9,  9,  0,  0,  0,  0,
    0,  0,  0, 19, 24, 34, 42, 44, 42, 34, 24, 19,  0,  0,  0,  0,
    0,  0,  0, 19, 24, 32, 37, 37, 37, 32, 24, 19,  0,  0,  0,  0,
    0,  0,  0, 19, 23, 27, 29, 30, 29, 27, 23, 19,  0,  0,  0,  0,
    0,  0,  0, 14, 18, 20, 27, 29, 27, 20, 18, 14,  0,  0,  0,  0,
    0,  0,  0,  7,  0, 13,  0, 16,  0, 13,  0,  7,  0,  0,  0,  0,
    0,  0,  0,  7,  0,  7,  0, 15,  0,  7,  0,  7,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
  }
};

/* 棋子序号对应的棋子类型
   每方的棋子顺序依次是：帅仕仕相相马马车车炮炮兵兵兵兵兵(将士士象象马马车车炮炮卒卒卒卒卒)
 */
 static const int pieceTypes[48] = {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
   0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
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
inline bool IN_FORT(int pos)
{
    return ccInFort[pos];
}
// 判断棋子是否在棋盘中
inline bool IN_BOARD(int pos)
{
    return ccInBoard[pos];
}
//向前移动一步
inline int32 SQUARE_FORWARD(int position, int sidePlayer)
{
    return position - 16 + (sidePlayer << 5);
}
// 是否未过河
inline bool HOME_HALF(int position, int sidePlayer)
{
    return (position & 0x80) != (sidePlayer << 7);
}
// 是否已过河
inline bool AWAY_HALF(int position, int sidePlayer)
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
//判断黑方还是红方，黑方返回1，红方返回0
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
inline uint8 GetOrder(int line, int col)
{
    return (line << 4) + col;
}
//翻转格子
inline int SQUARE_FLIP(int pos) {
    return 254 - pos;
}

// 历史走法信息
struct MoveStruct
{
    uint16 wmv;//前八位endpos，后八位beginpos
    uint16 ucpcCaptured, ucbCheck;//capture为被吃子编号
    uint32 dwKey;//zobrist

    void Set(int mv, int pcCaptured/*, bool bCheck*/, uint32 dwKey_)
    {
        wmv = mv;
        ucpcCaptured = pcCaptured;
        //ucbCheck = bCheck;
        dwKey = dwKey_;
    }
};

struct boardStruct
{
    int8 playerSide;//当前行走方R0/B1
    uint8 currentBoard[256];
    /*
    当前棋盘16*16 [3-12][3-11]为棋盘
    16-31  帅仕仕相相马马车车炮炮兵兵兵兵兵(R)
    32-47  将士士象象马马车车炮炮卒卒卒卒卒(B)
    0表示无棋子
    */
    uint8  currentPosition[48];//每个棋子当前位置，0表示被吃
    int32 redVal, blackVal;//红黑棋子的子力价值
    int32 nowDepth, nowMoveNum;
    MoveStruct mvsList[MAX_MOVES];//历史走法信息列表
    MoveStruct checkMove;
    ZobristStruct zobr;// Zobrist


    void ClearBoard()//棋盘初始化
    {
        playerSide = 0;
        nowDepth = 0;
        nowMoveNum = 0;
        redVal = 0;
        blackVal = 0;
        memset(currentBoard, 0, sizeof(currentBoard));
        memset(currentPosition, 0, sizeof(currentPosition));
        zobr.InitZero();
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
                    currentPosition[currentBoard[nowOrder]] = nowOrder;
                else currentPosition[currentBoard[nowOrder]] = 0;
            }
        }
    }
    /*
    void GetCurrentBoard(int8 fenSide,int8 fenBoard[])
    fenSide 当前玩家方
    fenBoard 当前棋盘
    由fen串获取当前棋盘信息
    */
    void GetCurrentBoard(int8 fenSide, int8 fenBoard[])
    {
        playerSide = fenSide;
        memcpy(currentBoard, fenBoard, 8 * 256);
    }
    //初始化红黑双方子力值
    void InitValue()
    {
        for (int chessPiece = 16; chessPiece < 48; chessPiece++)
        {
            if(currentPosition[chessPiece])
                AddPiece(currentPosition[chessPiece], chessPiece);
        }
    }
    /*
    int32 GenerateMove(int32* movesArray)
    生成移动方案
    将移动方案传入moveArray数组，其中终点放在左八位，起点放在右八位
    返回总的着法数量
    */
    int32 GenerateMove(int32* movesArray)
    {
        int32 numOfMoves = 0;
        int32 selfSide = SELF_SIDE(playerSide);//将棋子与之异或以判断归属
        int32 oppoSide = OPPO_SIDE(playerSide);
        for (int32 beginPosition = 0; beginPosition < 256; beginPosition++)
        {
            if (!ccInBoard[beginPosition])continue;
            int32 chessPieceFrom = currentBoard[beginPosition];
            if (!(chessPieceFrom & selfSide))  continue;
            switch (PIECE_INDEX(chessPieceFrom))
            {
            case KING_FROM://将移动
                for (int32 i = 0; i < 4; i++)
                {
                    int32 endPosition = beginPosition + ccKingDelta[i];
                    if (!IN_FORT(endPosition))
                        continue;
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                        
                }
                break;
            case ADVISOR_FROM://士移动
            case ADVISOR_TO:
                for (int32 i = 0; i < 4; i++)
                {
                    int32 endPosition = beginPosition + ccAdvisorDelta[i];
                    if (!IN_FORT(endPosition))
                        continue;
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                }
                break;
            case BISHOP_FROM://象移动
            case BISHOP_TO:
                for (int32 i = 0; i < 4; i++)
                {
                    int32 endPosition = beginPosition + ccAdvisorDelta[i];
                    if (!(IN_BOARD(endPosition) && HOME_HALF(endPosition, playerSide) && currentBoard[endPosition] == 0))
                        continue;
                    endPosition += ccAdvisorDelta[i];
                    int32 chessPieceTo = currentBoard[endPosition];
                    if (!(chessPieceTo & selfSide))
                        movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                }
                break;
            case KNIGHT_FROM://马移动
            case KNIGHT_TO:
                for (int32 i = 0; i < 4; i++)
                {
                    int32 endPosition = beginPosition + ccKingDelta[i];
                    if (currentBoard[endPosition] != 0)
                        continue;//蹩脚
                    for (int32 j = 0; j < 2; j++)
                    {
                        endPosition = beginPosition + ccKnightDelta[i][j];
                        if (!IN_BOARD(endPosition))
                            continue;
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (!(chessPieceTo & selfSide))
                            movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                    }
                }
                break;
            case ROOK_FROM://车移动
            case ROOK_TO:
                for (int32 i = 0; i < 4; i++)
                {
                    int32 endPosition = beginPosition + ccKingDelta[i];
                    while (IN_BOARD(endPosition))
                    {
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (chessPieceTo & selfSide)break;
                        if(chessPieceTo==0)
                            movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                        else if(chessPieceTo & oppoSide)
                        {
                            movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                            break;
                        }
                            
                        endPosition += ccKingDelta[i];
                    }
                }
                break;
            case CANNON_FROM://炮移动
            case CANNON_TO:
                for (int32 i = 0; i < 4; i++)
                {
                    int32 nDelta = ccKingDelta[i];
                    int32 endPosition = beginPosition + nDelta;
                    while (IN_BOARD(endPosition))
                    {
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (chessPieceTo == 0)
                            movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                        else
                            break;
                        endPosition += nDelta;
                    }//炮可以直接到达的地方
                    endPosition += nDelta;
                    while (IN_BOARD(endPosition))
                    {
                        int32 chessPieceTo = currentBoard[endPosition];
                        if (chessPieceTo != 0)
                        {
                            if ((chessPieceTo & oppoSide) != 0)
                                movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                            break;
                        }
                        endPosition += nDelta;
                    }//炮可以打的地方
                }
                break;
            default://兵移动
                int32 endPosition = SQUARE_FORWARD(beginPosition, playerSide);
                if (IN_BOARD(endPosition))
                {
                    int32 chessPieceTo = currentBoard[endPosition];
                    if ((chessPieceTo & selfSide) == 0)
                        movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                }
                if (AWAY_HALF(beginPosition, playerSide))
                {
                    for (int32 nDelta = -1; nDelta <= 1; nDelta += 2)
                    {
                        endPosition = beginPosition + nDelta;
                        if (IN_BOARD(endPosition))
                        {
                            int32 chessPieceTo = currentBoard[endPosition];
                            if ((chessPieceTo & selfSide) == 0)
                                movesArray[numOfMoves++] = RecordMove(beginPosition, endPosition);
                        }
                    }
                }
                break;
            }
        }
        return numOfMoves;//返回着法数量
    }
    //交换走子
    void ChangeSide()
    {
        playerSide = 1 - playerSide;
        zobr.Xor(Zobrist.Player);
    }
    //局面评价函数
    int Evaluate(void) const
    {
        return (playerSide == 0 ? redVal - blackVal : blackVal - redVal) + ADVANCED_VALUE;
    }
    /*//和棋分值
    int DrawValue(void) const
    {
        return (nowDepth & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
    }*/
    /*
    void DelPiece(int32 pos,int32 chessPiece)
    移除一个棋子 pos为移除棋子位置，chhessPiece为移除棋子编号
    */
    void DelPiece(int pos, int chessPiece)
    {
        currentBoard[pos] = 0;
        currentPosition[chessPiece] = 0;
        int32 pieceType = GETTYPE(chessPiece);//棋子具体类型，数值范围0-6
        // 红方减分，黑方加分
        if (chessPiece < 32)
        {
            redVal -= cucvlPiecePos[pieceType][pos];
            //zobr.Xor(Zobrist.Table[pieceType][pos]);
        }
        else
        {
            blackVal -= cucvlPiecePos[pieceType][SQUARE_FLIP(pos)];//取值颠倒
            //zobr.Xor(Zobrist.Table[pieceType + 7][pos]);
        }
    }

    /*
    void AddPiece(int32 pos,int32 chessPiece)
    增加一个棋子 pos为增加棋子位置，chhessPiece为增加棋子编号
    */
    void AddPiece(int pos, int chessPiece)
    {
        currentBoard[pos] = chessPiece;
        currentPosition[chessPiece] = pos;
        int32 pieceType = GETTYPE(chessPiece);//棋子具体类型，数值范围0-6
        // 红方加分，黑方减分
        if (chessPiece < 32)
        {
            redVal += cucvlPiecePos[pieceType][pos];
            //zobr.Xor(Zobrist.Table[pieceType][pos]);
        }
        else
        {
            blackVal += cucvlPiecePos[pieceType][SQUARE_FLIP(pos)];//取值颠倒
            //zobr.Xor(Zobrist.Table[pieceType + 7][pos]);
        }
    }
    /*
    进行一次将军局面下的假设移动，不换边，仅记录下棋着法和吃子，判断后即恢复
    */
    void MakeInCheckMove(int Move)
    {
        int Capture=0;
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

        checkMove.Set(Move, Capture, 0);//在移动列表中加入此次移动信息
    }

    /*
    撤销一次将军局面下的假设移动，不换边，仅撤销下棋着法和吃子
    */
    void UndoMakeInCheckMove()
    {
        int32 beginPos, endPos, chessPiece;
        beginPos = GETBEGIN(checkMove.wmv);
        endPos = GETEND(checkMove.wmv);
        chessPiece = currentBoard[endPos];
        DelPiece(endPos, chessPiece);
        AddPiece(beginPos, chessPiece);
        if (checkMove.ucpcCaptured)
        {
            int Capture = checkMove.ucpcCaptured;
            AddPiece(endPos, Capture);
        }
    }


    /*
    void MakeMove(int32 Move)
    进行一次移动，Move前八位为终点，后八位为起点
    */
    void MakeMove(int32 Move)
    {
        uint32 dwKey = zobr.dwKey;//记录局面
        ChangeSide();
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

        mvsList[nowMoveNum++].Set(Move, Capture, dwKey);//在移动列表中加入此次移动信息
        nowDepth++;//当前搜索深度++
        return;
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

    /*
    bool InCheck()
    判断被将军与否，返回true为将军
    */
    bool InCheck()
    {
        int kingNum = SELF_SIDE(playerSide);//根据palyerSide获取将编号
        int KingPos = currentPosition[kingNum];
        int32 selfSide = SELF_SIDE(playerSide);//将棋子与之异或以判断归属
        int32 oppoSide = OPPO_SIDE(playerSide);
        //兵将军
        //获取正前方 将军的兵编号
        int chessPiece = currentBoard[SQUARE_FORWARD(KingPos, playerSide)];
        if ((chessPiece & oppoSide) != 0 && PIECE_INDEX(chessPiece) >= PAWN_FROM)//判断是否为对面兵且过河
            return true;
        //判断左右兵将军
        for (int nDelta = -1; nDelta <= 1; nDelta += 2)
        {
            chessPiece = currentBoard[KingPos + nDelta];
            if ((chessPiece & oppoSide) != 0 && PIECE_INDEX(chessPiece) >= PAWN_FROM)
                return true;
        }

        //马将军
        for (int i = 0; i < 4; i++)
        {
            if (currentBoard[KingPos + ccAdvisorDelta[i]] != 0)//蹩脚
                continue;
            for (int j = 0; j < 2; j++)
            {
                chessPiece = currentBoard[KingPos + ccKnightCheckDelta[i][j]];
                if ((chessPiece & oppoSide) != 0 && (PIECE_INDEX(chessPiece) == KNIGHT_FROM
                    || PIECE_INDEX(chessPiece) == KNIGHT_TO))
                    return true;
            }
        }

        //车、炮、将
        for (int i = 0; i < 4; i++)//对应四个方向
        {
            int nDelta = ccKingDelta[i];
            int chessPos = KingPos + nDelta;
            while (IN_BOARD(chessPos))
            {
                chessPiece = currentBoard[chessPos];
                if (chessPiece != 0) //扫描线上第一个棋子
                {
                    if ((chessPiece & oppoSide) != 0 && (PIECE_INDEX(chessPiece) == ROOK_FROM
                        || PIECE_INDEX(chessPiece) == ROOK_TO|| PIECE_INDEX(chessPiece) == KING_FROM))
                        return true;//为对面车
                    break;
                }
                chessPos += nDelta;
            }
            chessPos += nDelta;
            while (IN_BOARD(chessPos)) //若中间有阻拦，则判断炮将军
            {
                int chessPiece = currentBoard[chessPos];
                if (chessPiece != 0)
                {	//扫描线上第二个棋子
                    if ((chessPiece & oppoSide) != 0 && (PIECE_INDEX(chessPiece) == CANNON_FROM
                        || PIECE_INDEX(chessPiece) == CANNON_TO))
                        return true;
                    break;
                }
                chessPos += nDelta;
            }
        }
        return false;
    }
};


#endif