#include "move.h"
#include "ucci.h"
#include "hash.h"
/*// 调整型局面评价函数
inline int Evaluate(const boardStruct& Board) {
    int vl;
    vl = Board.Evaluate();
    return vl == Board.DrawValue() ? vl - 1 : vl;
}*/

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
    int hashf = HASH_ALPHA;
    
    //搜索前先在置换表中查找
    int mv;
    int vl = LookUpHash(Board, Alpha, Beta, Board.nowDepth, mv);
    if (vl > -MATE_VALUE)
    {
        //std::cout << "分值：" << vl << " 着法：" << mv << '\n';
        if (mv == 0 && Board.nowDepth || mv != 0)
            return { vl,mv };
    }
    //搜索
    if (Board.nowDepth == DEPTH)
    {
        //printf("%d ", Board.Evaluate());
        StoreHash(Board, HASH_PV, 0, Board.Evaluate(), DEPTH);
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
            /*if (Board.nowDepth == 0)
            {
                printf("INCHECK! ");
                PRINT(Moves[i]);
                printf("\n\n");
            }*/

            Board.UndoMakeInCheckMove();
            continue;
        }
        Board.UndoMakeInCheckMove();
        Board.MakeMove(Moves[i]);
        Val = -AlphaBeta(Board, -Beta, -Alpha).Val;
        Board.UndoMakeMove();
        /*if (Board.nowDepth == 0)
        {
            printf("depth=0 val=%d alpha=%d beta=%d Moves=", Val, Alpha, Beta, Moves[i]);
            PRINT(Moves[i]);
            printf("\n\n");
        }

        if (Board.nowDepth == 1)
        {
            printf("depth=1 val=%d bsetVal=%d alpha=%d beta=%d\n", Val, bestVal, Alpha, Beta);
            PRINT(Moves[i]);
        }*/

        if (Val > bestVal)
        {
            if (Val >= Beta)
            {
                StoreHash(Board, HASH_BETA, Moves[i], Val, Board.nowDepth);//添加置换项
                return { Val, Moves[i] };
            }
            bestVal = Val;
            bestMove = Moves[i];
            if (Val > Alpha)
            {
                hashf = HASH_PV;
                Alpha = Val;
            }
        }
    }
    StoreHash(Board, hashf, bestMove, bestVal, Board.nowDepth);//添加置换项
    return { bestVal,bestMove };
}

int GetRandomMove(boardStruct& Board)
{
    int Moves[MAX_MOVES];
    Board.GenerateMove(Moves);
    return Moves[1];
}