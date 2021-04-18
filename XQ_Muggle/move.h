#ifndef MOVE_H
#define MOVE_H
#pragma once

#include "base.h"
#include "board.h"
#include "sortmove.h"

struct BestMove
{
	int Val;
	int BestMove;
};

BestMove AlphaBeta(boardStruct& Board, int Alpha/*initial negative inf*/, int Beta/*initial postive inf*/);
int MainSearch(boardStruct& board);
//BestMove PVS(boardStruct& Board, int Alpha, int Beta);
BestMove PVS(boardStruct& Board, int Alpha, int Beta, bool bNoNull = false);
int QuiescSearch(boardStruct& Board, int Alpha, int Beta);

int GetRandomMove(boardStruct& Board);
void PRINT(int Move);
#endif