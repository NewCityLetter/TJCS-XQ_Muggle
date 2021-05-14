#include "base.h"
#include "board.h"

#pragma once
#ifndef SORTMOVE_H
#define SORTMOVE_H


const int MVVLVA = 1;
const int HISTORY = 2;

extern int historyTable[65536];

static int cucMvvLva[16] = 
{
  5,1,1,1,1,3,3,4,4,3,3,2,2,2,2,2
};/*
    ��ǰ����16*16 [3-12][3-11]Ϊ����
    16-31  ˧�����������������ڱ���������(R)
    32-47  ��ʿʿ������������������������(B)
    0��ʾ������
    */

extern boardStruct board;

// ��MVV/LVAֵ
inline int MvvLva(int Move) 
{
	return (cucMvvLva[board.currentBoard[GETEND(Move)]] &15) - cucMvvLva[board.currentBoard[GETBEGIN(Move)]&15];
}

// "sort"��MVV/LVAֵ����ıȽϺ���
static bool CompareMvvLva(const int a, const int b) 
{
	return MvvLva(a) > MvvLva(b);
}

static bool CompareHistory(const int a, const int b)
{
	return historyTable[a] > historyTable[b];
}

inline int SortMove(boardStruct board, int* Moves,int kind,bool capture=0)
{
	int numOfMoves = board.GenerateMove(Moves,capture);
	std::sort(Moves, Moves + numOfMoves, kind==MVVLVA?CompareMvvLva:CompareHistory);
	return numOfMoves;
}	

#endif
