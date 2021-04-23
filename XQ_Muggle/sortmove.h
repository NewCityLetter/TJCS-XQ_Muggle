#include "base.h"
#include "board.h"

#pragma once
#ifndef SORTMOVE_H
#define SORTMOVE_H


const int MVVLVA = 1;
const int HISTORY = 2;

extern int historyTable[65536];

static int cucMvvLva[24] = 
{
  0, 0, 0, 0, 0, 0, 0, 0,
  5, 1, 1, 3, 4, 3, 2, 0,
  5, 1, 1, 3, 4, 3, 2, 0
};

extern boardStruct board;

// 求MVV/LVA值
inline int MvvLva(int Move) 
{
	return (cucMvvLva[board.currentBoard[GETEND(Move)]] << 3) - cucMvvLva[board.currentBoard[GETBEGIN(Move)]];
}

// "sort"按MVV/LVA值排序的比较函数
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
