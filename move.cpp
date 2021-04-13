#include "move.h"
#include "ucci.h"
#include "hash.h"
/*// 调整型局面评价函数
inline int Evaluate(const boardStruct& Board) {
    int vl;
    vl = Board.Evaluate();
    return vl == Board.DrawValue() ? vl - 1 : vl;
}*/

const long long MAX_TIME = 5000;               //最长搜索时间

void PRINT(int Move)
{
    char moves[5];
    int endPos = GETEND(Move);
    int beginPos = GETBEGIN(Move);
    int beginLine = beginPos >> 4;
    int beginCol = beginPos & 15;
    int endLine = endPos >> 4;
    int endCol = endPos & 15;
    //char moves[5];
    moves[0] = beginCol - 3 + 'a';
    moves[1] = 12 - beginLine + '0';
    moves[2] = endCol - 3 + 'a';
    moves[3] = 12 - endLine + '0';
    moves[4] = '\0';
    printf("%s\n", moves);
}

/*
uint32 AlphaBeta(boardStruct & Board)
传入当前棋盘
AlphaBeta剪枝
当一个 Min 节点的 β值≤任何一个父节点的α值时 ，剪掉该节点的所有子节点
当一个 Max 节点的 α值≥任何一个父节点的β值时 ，剪掉该节点的所有子节点
在一个节点中，α、β只有一个产生作用
*/
BestMove AlphaBeta(boardStruct& Board, int Alpha/*initial negative inf*/, int Beta/*initial postive inf*/)
{
    if (GetTime() - beginSearchTime >= MAX_TIME)
    {
        isNormalEnd = 0;
        return {0,0};
    }
    if (Board.nowDepth == DEPTH)
    {
        return { Board.Evaluate(), 0 };
    }
    int Val = 0, bestVal = MIN_VAL, bestMove = 0;
    int Moves[MAX_MOVES];
    int numOfMoves = Board.GenerateMove(Moves);

    for (int i = 0; i < numOfMoves; i++)//遍历着法
    {
        Board.MakeInCheckMove(Moves[i]);
        if (Board.InCheck())
        {
            Board.UndoMakeInCheckMove();
            continue;
        }
        Board.UndoMakeInCheckMove();
        Board.MakeMove(Moves[i]);
        Val = -AlphaBeta(Board, -Beta, -Alpha).Val;
        Board.UndoMakeMove();

        if (Val > bestVal)
        {
            if (Val >= Beta)
            {
                return { Val, Moves[i] };
            }
            bestVal = Val;
            bestMove = Moves[i];
            if (Val > Alpha)
            {
                //hashf = HASH_PV;
                Alpha = Val;
            }
        }
    }
    return { bestVal,bestMove };
}

int GetRandomMove(boardStruct& Board)
{
    int Moves[MAX_MOVES];
    Board.GenerateMove(Moves);
    return Moves[1];
}
/*
BestMove PVS(boardStruct& Board,int Alpha, int Beta)
主要变例搜索，调用方法同AlphaBeta
*/
BestMove PVS(boardStruct& Board,int Alpha, int Beta)
{
    if (GetTime() - beginSearchTime >= MAX_TIME)
    {
        isNormalEnd = 0;
        return { 0,0 };
    }
    if (Board.nowDepth == DEPTH)
    {
        return { Board.Evaluate(), 0 };
    }
    int val = 0, bestVal = MIN_VAL, bestMove = 0;
    int Moves[MAX_MOVES];
    int numOfMoves = SortMove(board, Moves);

    for (int i = 0; i < numOfMoves; i++)//遍历着法
    {
        Board.MakeInCheckMove(Moves[i]);
        if (Board.InCheck())
        {
            Board.UndoMakeInCheckMove();
            continue;
        }
        Board.UndoMakeInCheckMove();


        Board.MakeMove(Moves[i]);
        if (bestVal == MIN_VAL)
        {
            val = -PVS(Board,-Beta, -Alpha).Val;
        }
        else
        {
            val = -PVS(Board,-Alpha - 1, -Alpha).Val;
            if (val > Alpha && val < Beta)
            {
                val= -PVS(Board, -Beta, -Alpha).Val;
            }
        }
        Board.UndoMakeMove();

        if (val > bestVal)
        {
            if (val >= Beta)
            {
                return { val, Moves[i] };
            }
            bestVal = val;
            bestMove = Moves[i];
            if (val > Alpha)
            {
                Alpha = val;
            }
        }
    }
    return { bestVal,bestMove };
}

/*
int MainSearch(boardStruct& board)
将所有零散变量整合在MainSearch中，调用MainSearch，传入棋盘，返回着法
*/
int MainSearch(boardStruct& board)
{
    int Move=0;
    beginSearchTime= GetTime();
    isNormalEnd = 1;
    DEPTH = 4;
    while (isNormalEnd&&DEPTH<=MAX_DEPTH)
    {
        int tmpMove = PVS(board, MIN_VAL, MAX_VAL).BestMove;
        if (isNormalEnd)
            Move = tmpMove;
        DEPTH++;
        //迭代加深
    }
    return Move;
}