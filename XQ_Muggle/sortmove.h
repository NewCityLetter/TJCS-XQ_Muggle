#pragma once
#ifndef SORTMOVE_H
#define SORTMOVE_H

#include "base.h"
#include "board.h"

extern boardStruct board;
extern int32 KillerMove[LIMIT_DEPTH][2];

const int HASHMOVE = 0;
const int GENCAP = 1;
const int CAPTURE =2;
const int KILLER1 = 3;
const int KILLER2 = 4;
const int GENERATE = 5;
const int RETURN = 6;

const int MVVLVA = 1;
const int HISTORY = 2;

const int SORT_MAX = 10000000;

static bool CompareHistory(const SearchMove a, const SearchMove b)
{
	return board.historyTable[a.move] > board.historyTable[b.move];
}

static bool CompareMvvLva(const SearchMove a, const SearchMove b)
{
	return a.val > b.val;
}

inline int SortMove(SearchMove* Moves,int kind,bool capture=0)
{
	int numOfMoves = capture?board.GenerateMove(Moves,1):board.GenerateAllMove(Moves);
	std::sort(Moves, Moves + numOfMoves, kind==MVVLVA?CompareMvvLva:CompareHistory);
	return numOfMoves;
}

//
inline void ClearKiller()
{
	memset(KillerMove, 0,sizeof(KillerMove));
}

struct sortMoveStruct 
{
	int moveHash; 
	int nPhase, nIndex, numOfMoves;    
	SearchMove Moves[MAX_MOVES];           

	int Next(); 
	int InCheckMove(int mvHash);
	void FullInit(int mvHash);
	void RootInit();
};

static bool RULE(const SearchMove a, const SearchMove b)
{
	return a.val < b.val;
}

void sortMoveStruct::RootInit()
{
	nIndex=0;
	nPhase=RETURN;
	numOfMoves=board.GenerateAllMove(Moves);
	int tmpNum=0;
	for(int i=0;i<numOfMoves;i++)
	{
		Moves[i].val=0;
		if(board.MakeMove(Moves[i].move))
		{
			Moves[i].val=1,tmpNum++;
			board.UndoMakeMove();
		}
			
		
	}
	//printf("numofmoves=%d tmp=%d\n",numOfMoves,tmpNum);
	std::sort(Moves,Moves+numOfMoves,CompareMvvLva);
	numOfMoves=tmpNum;
}

int sortMoveStruct::InCheckMove(int mvHash)
{
	nIndex=0;
	moveHash=mvHash;
	nPhase=RETURN;
	numOfMoves=board.GenerateAllMove(Moves);
	int legalNum=0;
	for(int i=0;i<numOfMoves;i++)
	{
		if(Moves[i].move==moveHash)
			Moves[i].val=SORT_MAX,legalNum++;
		else if(board.MakeMove(Moves[i].move))
		{
			board.UndoMakeMove();
			if(Moves[i].move==KillerMove[board.nowDepth][0])
				Moves[i].val=SORT_MAX-1;
			else if(Moves[i].move==KillerMove[board.nowDepth][1])
				Moves[i].val=SORT_MAX-2;
			else
				Moves[i].val=board.historyTable[Moves[i].move]+1;
			legalNum++;
		}
		else 
		Moves[i].val=0;
	}
	std::sort(Moves,Moves+numOfMoves,CompareMvvLva);
	numOfMoves=legalNum;
	return (legalNum==1?Moves[0].move:0);
}

void sortMoveStruct::FullInit(int mvHash)
{
	nPhase=HASHMOVE;
	moveHash=mvHash;
	nIndex=0;
}

int sortMoveStruct::Next() 
{
	switch (nPhase)
	{
		case HASHMOVE:
		{
			nPhase = GENCAP;
			if (moveHash != 0 && board.LegalMove(moveHash))
				return moveHash;
		}
		case GENCAP:
		{
			nPhase=CAPTURE;
			numOfMoves=board.GenerateMove(Moves,1);
			std::sort(Moves,Moves+numOfMoves,RULE);
			nIndex=numOfMoves-1;
		}
		case CAPTURE:
		{
			if(nIndex>=0&&Moves[nIndex].val>1)
				return Moves[nIndex--].move;
			nIndex++;
			nPhase=KILLER1;
		}
		case KILLER1:
		{
			nPhase = KILLER2;
			if (KillerMove[board.nowDepth][0] != 0 && board.LegalMove(KillerMove[board.nowDepth][0]))
				return KillerMove[board.nowDepth][0];
		}
		case KILLER2:
		{
			nPhase = GENERATE;
			if (KillerMove[board.nowDepth][1] != 0 && board.LegalMove(KillerMove[board.nowDepth][1]))
				return KillerMove[board.nowDepth][1];
		}
		case GENERATE:
		{
			nPhase = RETURN;
			numOfMoves=nIndex;
			numOfMoves += board.GenerateMove(Moves+nIndex);
			std::sort(Moves, Moves +numOfMoves, CompareHistory);
			nIndex = 0;
		}
		default:
		{
			while (nIndex <numOfMoves) 
			{
				return Moves[nIndex++].move;
			}
		}
	}
	return 0;

}


//
inline void SetBestMove(int mv, int nDepth, int32* Killer) 
{
	board.historyTable[mv] += nDepth * nDepth;
	if (Killer[0] != mv) 
	{
		Killer[1] = Killer[0];
		Killer[0] = mv;
	}
}

#endif
