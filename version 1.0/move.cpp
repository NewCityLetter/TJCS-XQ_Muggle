#include "board.h"

/*
uint32 AlphaBeta(boardStruct & Board)
传入当前棋盘
AlphaBeta剪枝
*/
uint32 AlphaBeta(boardStruct & Board,int Alpha,int Beta)
{
    if(Board.nowDepth==0)
    {
        return //返回board估值函数
    }
    int Val;
    int Moves[MAX_MOVES];
    int numOfMoves=Board.GenerateMove(Moves);
    for(int i=0;i<numOfMoves;i++)//遍历着法
    {
        Board.MakeMove(Moves[i]);
        Val=-AlphaBeta(Board,-Beta,-Alpha);
        Board.UndoMakeMove();
        if(Val>=Beta)
            return Beta;
        if(Val>Alpha)
            Alpha=Val;
    }
    return Alpha;
}