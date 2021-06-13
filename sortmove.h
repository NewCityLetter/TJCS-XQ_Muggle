#pragma once
#ifndef SORTMOVE_H
#define SORTMOVE_H

#include "base.h"
#include "board.h"

extern boardStruct board;
extern int historyTable[65536];
extern uint16 wmvKiller[LIMIT_DEPTH][2];

const int KILLER1 = 1;
const int KILLER2 = 2;
const int GENERATE = 3;
const int RETURN = 4;

const int MVVLVA = 1;
const int HISTORY = 2;

static int cucMvvLva[16] = 
{
  5,1,1,1,1,3,3,4,4,3,3,2,2,2,2,2
};
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

inline int SortMove(int* Moves,int kind,bool capture=0)
{
	int numOfMoves = board.GenerateMove(Moves,capture);
	std::sort(Moves, Moves + numOfMoves, kind==MVVLVA?CompareMvvLva:CompareHistory);
	return numOfMoves;
}	

// ���ɱ���ŷ���
inline void ClearKiller()
{
	memset(wmvKiller, 0,sizeof(wmvKiller));
}

struct sortMoveStruct 
{
	int mvKiller1, mvKiller2; // ����ɱ���߷�
	int nPhase, nIndex, numOfMoves;    // ��ǰ�׶Σ���ǰ���õڼ����߷����ܹ��м����߷�
	int Moves[MAX_MOVES];           // ���е��߷�
	void Init()
	{ // ��ʼ�����趨�û����߷�������ɱ���߷�
		mvKiller1 = wmvKiller[board.nowDepth][0];
		mvKiller2 = wmvKiller[board.nowDepth][1];
		nPhase = KILLER1;
	}
	int Next(); // �õ���һ���߷�
};

// �õ���һ���߷�
int sortMoveStruct::Next() 
{
	int move;
	switch (nPhase)
	{
		case KILLER1:
		{
			nPhase = KILLER2;
			//if (mvKiller1 == 13395)printf("case 1\n");
			if (mvKiller1 != 0 && board.LegalMove(mvKiller1))
				return mvKiller1;
		}
		case KILLER2:
		{
			nPhase = GENERATE;
			//if (mvKiller2 == 13395)printf("case 2\n");
			if (mvKiller2 != 0 && board.LegalMove(mvKiller2))
				return mvKiller2;
		}
		case GENERATE:
		{
			nPhase = RETURN;
			//printf("case 3\n");
			numOfMoves = board.GenerateMove(Moves);
			std::sort(Moves, Moves + numOfMoves, CompareHistory);
			nIndex = 0;
		}
		default:
		{
			
			while (nIndex < numOfMoves) 
			{
				move = Moves[nIndex++];
				//if (move == 13395)printf("case 4\n");
				if (move != mvKiller1 && move != mvKiller2) 
					return move;
			}
		}
	}
	return 0;

}


// ������߷��Ĵ���
inline void SetBestMove(int mv, int nDepth, uint16* lpwmvKiller) 
{
	historyTable[mv] += nDepth * nDepth;
	if (lpwmvKiller[0] != mv) 
	{
		lpwmvKiller[1] = lpwmvKiller[0];
		lpwmvKiller[0] = mv;
	}
}

#endif
