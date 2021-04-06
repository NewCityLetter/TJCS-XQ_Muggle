#ifndef MOVE_H
#define MOVE_H
#pragma once

#include "board.h"

struct BestMove
{
	int Val;
	int BestMove;
};

BestMove AlphaBeta(boardStruct& Board, int Alpha/*initial negative inf*/, int Beta/*initial postive inf*/);

int GetRandomMove(boardStruct& Board);

#endif