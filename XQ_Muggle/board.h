/*�޸ĺ����ӵ����ݣ�
1.�޸���GETTYPE������ʹ����������͵��жϵķ���ֵ�Ӽ򵥵�0/1����˿��Է������Ӿ������ͣ�ʵ�ַ���������pieceTypes���飻
2.��1�������ġ��ж������Ǻ췽���Ǻڷ����ķ�ʽ�ӿ�GETTYPE����ֵ��0/1����ˡ�chessPiece�Ƿ�<32����
3.������boardStruct.Evaluate��Ϊ�������ۺ����������Ǻ��˫����������ֵ֮����峣����ADVANCED_VALUE=3��
4.������boardStruct.Drawvalue��Ϊ�����ֵ�����峣����DRAW_VALUE=20��
5.��move.cpp�����ӡ������;������ۺ�����������AlphaBeta�����в�����return�Ŀ�ȱ���֡�
*/
#ifndef BOARD_H
#define BOARD_H
#pragma once

#include "base.h"
#include "zobrist.h"

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

const int ADVANCED_VALUE = 3;      //����Ȩֵ
const int DRAW_VALUE = 20;         //����ʱ���صķ���(ȡ��ֵ)

// �ж������Ƿ��������е�����
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

// �ж������Ƿ��ھŹ�������
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

// �жϲ����Ƿ�����ض��߷������飬1=˧(��)��2=��(ʿ)��3=��(��)
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

// ���ݲ����ж����Ƿ����ȵ�����
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

/* ˧(��)�Ĳ���*/
static const int8 ccKingDelta[4] = { -16, -1, 1, 16 };

/*��(ʿ)�Ĳ���(�ƶ����ε������ƶ�һ�Σ�����һ������)*/
static const int8 ccAdvisorDelta[4] = { -17, -15, 15, 17 };

/*��Ĳ�������˧(��)�Ĳ�����Ϊ����*/
static const int8 ccKnightDelta[4][2] = { {-33, -31}, {-18, 14}, {-14, 18}, {31, 33} };

/*�������Ĳ���������(ʿ)�Ĳ�����Ϊ����*/
static const int8 ccKnightCheckDelta[4][2] = { {-33, -18}, {-31, -14}, {14, 31}, {18, 33} };

// ����λ�ü�ֵ��
static const uint8 cucvlPiecePos[7][256] = {
  { // ˧(��)
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
  }, { // ��(ʿ)
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
  }, { // ��(��)
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
  }, { // ��
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
  }, { // ��
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
  }, { // ��
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
  }, { // ��(��)
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

/* ������Ŷ�Ӧ����������
   ÿ��������˳�������ǣ�˧�����������������ڱ���������(��ʿʿ������������������������)
 */
 static const int pieceTypes[48] = {
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
   0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6
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
inline bool IN_FORT(int pos)
{
    return ccInFort[pos];
}
// �ж������Ƿ���������
inline bool IN_BOARD(int pos)
{
    return ccInBoard[pos];
}
//��ǰ�ƶ�һ��
inline int32 SQUARE_FORWARD(int position, int sidePlayer)
{
    return position - 16 + (sidePlayer << 5);
}
// �Ƿ�δ����
inline bool HOME_HALF(int position, int sidePlayer)
{
    return (position & 0x80) != (sidePlayer << 7);
}
// �Ƿ��ѹ���
inline bool AWAY_HALF(int position, int sidePlayer)
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
//�жϺڷ����Ǻ췽���ڷ�����1���췽����0
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
inline uint8 GetOrder(int line, int col)
{
    return (line << 4) + col;
}
//��ת����
inline int SQUARE_FLIP(int pos) {
    return 254 - pos;
}

// ��ʷ�߷���Ϣ
struct MoveStruct
{
    uint16 wmv;//ǰ��λendpos�����λbeginpos
    uint16 ucpcCaptured, ucbCheck;//captureΪ�����ӱ��
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
    int8 playerSide;//��ǰ���߷�R0/B1
    uint8 currentBoard[256];
    /*
    ��ǰ����16*16 [3-12][3-11]Ϊ����
    16-31  ˧�����������������ڱ���������(R)
    32-47  ��ʿʿ������������������������(B)
    0��ʾ������
    */
    uint8  currentPosition[48];//ÿ�����ӵ�ǰλ�ã�0��ʾ����
    int32 redVal, blackVal;//������ӵ�������ֵ
    int32 nowDepth, nowMoveNum;
    MoveStruct mvsList[MAX_MOVES];//��ʷ�߷���Ϣ�б�
    MoveStruct checkMove;
    ZobristStruct zobr;// Zobrist


    void ClearBoard()//���̳�ʼ��
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
    //��ȡ��ǰcurrentPosition����
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
    fenSide ��ǰ��ҷ�
    fenBoard ��ǰ����
    ��fen����ȡ��ǰ������Ϣ
    */
    void GetCurrentBoard(int8 fenSide, int8 fenBoard[])
    {
        playerSide = fenSide;
        memcpy(currentBoard, fenBoard, 8 * 256);
    }
    //��ʼ�����˫������ֵ
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
    �����ƶ�����
    ���ƶ���������moveArray���飬�����յ�������λ���������Ұ�λ
    �����ܵ��ŷ�����
    */
    int32 GenerateMove(int32* movesArray)
    {
        int32 numOfMoves = 0;
        int32 selfSide = SELF_SIDE(playerSide);//��������֮������жϹ���
        int32 oppoSide = OPPO_SIDE(playerSide);
        for (int32 beginPosition = 0; beginPosition < 256; beginPosition++)
        {
            if (!ccInBoard[beginPosition])continue;
            int32 chessPieceFrom = currentBoard[beginPosition];
            if (!(chessPieceFrom & selfSide))  continue;
            switch (PIECE_INDEX(chessPieceFrom))
            {
            case KING_FROM://���ƶ�
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
            case ADVISOR_FROM://ʿ�ƶ�
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
            case BISHOP_FROM://���ƶ�
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
            case KNIGHT_FROM://���ƶ�
            case KNIGHT_TO:
                for (int32 i = 0; i < 4; i++)
                {
                    int32 endPosition = beginPosition + ccKingDelta[i];
                    if (currentBoard[endPosition] != 0)
                        continue;//����
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
            case ROOK_FROM://���ƶ�
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
            case CANNON_FROM://���ƶ�
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
                    }//�ڿ���ֱ�ӵ���ĵط�
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
                    }//�ڿ��Դ�ĵط�
                }
                break;
            default://���ƶ�
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
        return numOfMoves;//�����ŷ�����
    }
    //��������
    void ChangeSide()
    {
        playerSide = 1 - playerSide;
        zobr.Xor(Zobrist.Player);
    }
    //�������ۺ���
    int Evaluate(void) const
    {
        return (playerSide == 0 ? redVal - blackVal : blackVal - redVal) + ADVANCED_VALUE;
    }
    /*//�����ֵ
    int DrawValue(void) const
    {
        return (nowDepth & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
    }*/
    /*
    void DelPiece(int32 pos,int32 chessPiece)
    �Ƴ�һ������ posΪ�Ƴ�����λ�ã�chhessPieceΪ�Ƴ����ӱ��
    */
    void DelPiece(int pos, int chessPiece)
    {
        currentBoard[pos] = 0;
        currentPosition[chessPiece] = 0;
        int32 pieceType = GETTYPE(chessPiece);//���Ӿ������ͣ���ֵ��Χ0-6
        // �췽���֣��ڷ��ӷ�
        if (chessPiece < 32)
        {
            redVal -= cucvlPiecePos[pieceType][pos];
            //zobr.Xor(Zobrist.Table[pieceType][pos]);
        }
        else
        {
            blackVal -= cucvlPiecePos[pieceType][SQUARE_FLIP(pos)];//ȡֵ�ߵ�
            //zobr.Xor(Zobrist.Table[pieceType + 7][pos]);
        }
    }

    /*
    void AddPiece(int32 pos,int32 chessPiece)
    ����һ������ posΪ��������λ�ã�chhessPieceΪ�������ӱ��
    */
    void AddPiece(int pos, int chessPiece)
    {
        currentBoard[pos] = chessPiece;
        currentPosition[chessPiece] = pos;
        int32 pieceType = GETTYPE(chessPiece);//���Ӿ������ͣ���ֵ��Χ0-6
        // �췽�ӷ֣��ڷ�����
        if (chessPiece < 32)
        {
            redVal += cucvlPiecePos[pieceType][pos];
            //zobr.Xor(Zobrist.Table[pieceType][pos]);
        }
        else
        {
            blackVal += cucvlPiecePos[pieceType][SQUARE_FLIP(pos)];//ȡֵ�ߵ�
            //zobr.Xor(Zobrist.Table[pieceType + 7][pos]);
        }
    }
    /*
    ����һ�ν��������µļ����ƶ��������ߣ�����¼�����ŷ��ͳ��ӣ��жϺ󼴻ָ�
    */
    void MakeInCheckMove(int Move)
    {
        int Capture=0;
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

        checkMove.Set(Move, Capture, 0);//���ƶ��б��м���˴��ƶ���Ϣ
    }

    /*
    ����һ�ν��������µļ����ƶ��������ߣ������������ŷ��ͳ���
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
    ����һ���ƶ���Moveǰ��λΪ�յ㣬���λΪ���
    */
    void MakeMove(int32 Move)
    {
        uint32 dwKey = zobr.dwKey;//��¼����
        ChangeSide();
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

        mvsList[nowMoveNum++].Set(Move, Capture, dwKey);//���ƶ��б��м���˴��ƶ���Ϣ
        nowDepth++;//��ǰ�������++
        return;
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

    /*
    bool InCheck()
    �жϱ�������񣬷���trueΪ����
    */
    bool InCheck()
    {
        int kingNum = SELF_SIDE(playerSide);//����palyerSide��ȡ�����
        int KingPos = currentPosition[kingNum];
        int32 selfSide = SELF_SIDE(playerSide);//��������֮������жϹ���
        int32 oppoSide = OPPO_SIDE(playerSide);
        //������
        //��ȡ��ǰ�� �����ı����
        int chessPiece = currentBoard[SQUARE_FORWARD(KingPos, playerSide)];
        if ((chessPiece & oppoSide) != 0 && PIECE_INDEX(chessPiece) >= PAWN_FROM)//�ж��Ƿ�Ϊ������ҹ���
            return true;
        //�ж����ұ�����
        for (int nDelta = -1; nDelta <= 1; nDelta += 2)
        {
            chessPiece = currentBoard[KingPos + nDelta];
            if ((chessPiece & oppoSide) != 0 && PIECE_INDEX(chessPiece) >= PAWN_FROM)
                return true;
        }

        //����
        for (int i = 0; i < 4; i++)
        {
            if (currentBoard[KingPos + ccAdvisorDelta[i]] != 0)//����
                continue;
            for (int j = 0; j < 2; j++)
            {
                chessPiece = currentBoard[KingPos + ccKnightCheckDelta[i][j]];
                if ((chessPiece & oppoSide) != 0 && (PIECE_INDEX(chessPiece) == KNIGHT_FROM
                    || PIECE_INDEX(chessPiece) == KNIGHT_TO))
                    return true;
            }
        }

        //�����ڡ���
        for (int i = 0; i < 4; i++)//��Ӧ�ĸ�����
        {
            int nDelta = ccKingDelta[i];
            int chessPos = KingPos + nDelta;
            while (IN_BOARD(chessPos))
            {
                chessPiece = currentBoard[chessPos];
                if (chessPiece != 0) //ɨ�����ϵ�һ������
                {
                    if ((chessPiece & oppoSide) != 0 && (PIECE_INDEX(chessPiece) == ROOK_FROM
                        || PIECE_INDEX(chessPiece) == ROOK_TO|| PIECE_INDEX(chessPiece) == KING_FROM))
                        return true;//Ϊ���泵
                    break;
                }
                chessPos += nDelta;
            }
            chessPos += nDelta;
            while (IN_BOARD(chessPos)) //���м������������ж��ڽ���
            {
                int chessPiece = currentBoard[chessPos];
                if (chessPiece != 0)
                {	//ɨ�����ϵڶ�������
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