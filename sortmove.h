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
    当前棋盘16*16 [3-12][3-11]为棋盘
    16-31  帅仕仕相相马马车车炮炮兵兵兵兵兵(R)
    32-47  将士士象象马马车车炮炮卒卒卒卒卒(B)
    0表示无棋子
    */

extern boardStruct board;

// 求MVV/LVA值
inline int MvvLva(int Move) 
{
	return (cucMvvLva[board.currentBoard[GETEND(Move)]] &15) - cucMvvLva[board.currentBoard[GETBEGIN(Move)]&15];
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
