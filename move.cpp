#include "move.h"
#include "ucci.h"
#include "hash.h"
/*// 调整型局面评价函数
inline int Evaluate(const boardStruct& Board) {
    int vl;
    vl = Board.Evaluate();
    return vl == Board.DrawValue() ? vl - 1 : vl;
}*/

const long long MAX_TIME = 9000;               //最长搜索时间

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
时代的眼泪
uint32 AlphaBeta(boardStruct & Board)
传入当前棋盘
AlphaBeta剪枝
当一个 Min 节点的 β值≤任何一个父节点的α值时 ，剪掉该节点的所有子节点
当一个 Max 节点的 α值≥任何一个父节点的β值时 ，剪掉该节点的所有子节点
在一个节点中，α、β只有一个产生作用
BestMove AlphaBeta(boardStruct& Board, int Alpha, int Beta)
{
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
        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            isNormalEnd = 0;
            return { bestVal,bestMove };
        }
    }
    return { bestVal,bestMove };
}*/


int GetRandomMove(boardStruct& Board)
{
    int Moves[MAX_MOVES];
    Board.GenerateMove(Moves);
    return Moves[1];
}

/*
BestMove QuiescSearchh(boardStruct& Board, int Alpha, int Beta)
静态搜索，用法同AlphaBeta。在主要变例搜索到叶节点时使用静态搜索代替直接返回局面估值，防止水平线效应。
*/
int QuiescSearch(boardStruct& Board, int Alpha, int Beta)
{
    if (Board.nowDepth == LIMIT_DEPTH+DEPTH)//若达到最大搜索深度返回估值
    {
        /*for (int i = 2; i <= 0xc; printf("\n"), i++)
            for (int j = 2; j <= 0xb; j++)
            {
                if (i == 2 && j == 2)
                {
                    printf("  ");
                    continue;
                }
                if (i == 2)
                {
                    printf(" %c ", 'A' + j - 3);
                    continue;
                }
                if (j == 2)
                {
                    printf("%d ", 9 - i + 3);
                    continue;
                }
                if (Board.currentBoard[(i << 4) + j] == 0)
                    printf("0");
                printf("%d ", Board.currentBoard[(i << 4) + j]);

            }
        printf("nowval=%d\n", Board.Evaluate());*/
        return Board.Evaluate();
    }
    int Val = 0, bestVal = MIN_VAL;
    int Moves[MAX_MOVES];
    int numOfMoves=0;
    if(Board.InCheck())//若当前在将军，则返回所有着法
        numOfMoves = SortMove(board, Moves);
    else numOfMoves = SortMove(board, Moves,true);//否则返回所有吃子着法，即局面大分变化着法
    if (numOfMoves == 0)
    {
        return Board.Evaluate();
    }
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
        Val = -QuiescSearch(Board, -Beta, -Alpha);
        Board.UndoMakeMove();
        /*if (Board.nowDepth == LIMIT_DEPTH -4)
        {
            printf("num=%d nowval=%d bestval=%d Alpha=%d Beta=%d nowdepth=%d\n",numOfMoves, Val,bestVal,Alpha,Beta,Board.nowDepth);
        }*/

        if (Val > bestVal)
        {
            if (Val >= Beta)
            {
                return Val;
            }
            bestVal = Val;
            if (Val > Alpha)
            {
                Alpha = Val;
            }
        }

        /*if (Board.nowDepth == LIMIT_DEPTH -4)
        {
            printf("num=%d nowval=%d bestval=%d Alpha=%d Beta=%d\n\n", numOfMoves, Val, bestVal, Alpha, Beta);
        }*/

        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            isNormalEnd = 0;
            return bestVal;
        }
    }

    return bestVal==MIN_VAL?Board.nowDepth+MIN_VAL:bestVal;
}


/*
BestMove PVS(boardStruct& Board,int Alpha, int Beta)
主要变例搜索，调用方法同AlphaBeta
*/
BestMove PVS(boardStruct& Board,int Alpha, int Beta)
{

    if (Board.nowDepth == DEPTH)
    {
        return {QuiescSearch(Board, Alpha, Beta), 0};
    }
    if (Board.nowDepth == LIMIT_DEPTH)
    {
        return {Board.Evaluate(), 0};
    }
    int val = 0, bestVal = MIN_VAL, bestMove = 0;
    int Moves[MAX_MOVES];
    int numOfMoves = SortMove(Board, Moves);

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

        
        /*if (Board.nowDepth ==DEPTH-4)
        {
            printf("num=%d nowval=%d bestval=%d Alpha=%d Beta=%d nowdepth=%d\n", numOfMoves, val, bestVal, Alpha, Beta, Board.nowDepth);
        }*/

        if (val > bestVal)
        {
            bestVal = val;
            bestMove = Moves[i];
            if (val >= Beta)
            {
                return { val, Moves[i] };
            }
            
            if (val > Alpha)
            {
                Alpha = val;
            }
        }
        /*if (Board.nowDepth == 0)
        {
            printf("depth=%d nowmove=", Board.nowDepth);
            PRINT(Moves[i]);
            printf("  val=%d bestMove=", val);
            PRINT(bestMove);
            printf("\n");
        }*/

        /*if (Board.nowDepth == 0)
        {
            printf("maxdepth=%d tot=%d num=%d nowval=%d bestval=%d Alpha=%d Beta=%d nowmove=",DEPTH,numOfMoves, i, val, bestVal, Alpha, Beta);
            PRINT(Moves[i]);
            printf("   bestMove=");
            PRINT(bestMove);
            printf("\n\n");
        }*/

        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            /*printf("nowdepth=%d BestVAl=%d move=", Board.nowDepth, bestVal);
            PRINT(bestMove);
            printf("\n");*/
            isNormalEnd = 0;
            return { bestVal,bestMove };
        }
    }
    return { bestVal,bestMove };
}

/*
int MainSearch(boardStruct& board)
将所有零散变量整合在MainSearch中，调用MainSearch，传入棋盘，返回着法
*/
int MainSearch(boardStruct& Board)
{
    BestMove Move = {MIN_VAL,0};
    beginSearchTime= GetTime();
    isNormalEnd = 1;
    DEPTH = 2;
    while (isNormalEnd&&DEPTH<=MAX_DEPTH)
    {
        BestMove tmpMove = PVS(Board, MIN_VAL, MAX_VAL);
        if (isNormalEnd&&tmpMove.BestMove)
            Move = tmpMove;
        DEPTH++;
        //迭代加深
    }
    return Move.BestMove;
}