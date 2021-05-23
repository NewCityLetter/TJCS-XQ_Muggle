#ifndef MOVE_H
#define MOVE_H
#pragma once

#include "base.h"
#include "board.h"


struct BestMove
{
	int Val;
	int BestMove;
};

int MainSearch();
int PVS(int depth,int Alpha, int Beta, bool bNoNull = false);
int searchRoot(int depth);
int QuiescSearch( int Alpha, int Beta);
void PRINT(int Move);
#endif