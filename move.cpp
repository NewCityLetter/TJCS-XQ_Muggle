#include "move.h"
#include "ucci.h"
#include "hash.h"
/*// �����;������ۺ���
inline int Evaluate(const boardStruct& Board) {
    int vl;
    vl = Board.Evaluate();
    return vl == Board.DrawValue() ? vl - 1 : vl;
}*/

const long long MAX_TIME = 5000;               //�����ʱ��

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
���뵱ǰ����
AlphaBeta��֦
��һ�� Min �ڵ�� ��ֵ���κ�һ�����ڵ�Ħ�ֵʱ �������ýڵ�������ӽڵ�
��һ�� Max �ڵ�� ��ֵ���κ�һ�����ڵ�Ħ�ֵʱ �������ýڵ�������ӽڵ�
��һ���ڵ��У�������ֻ��һ����������
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

    for (int i = 0; i < numOfMoves; i++)//�����ŷ�
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
��Ҫ�������������÷���ͬAlphaBeta
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

    for (int i = 0; i < numOfMoves; i++)//�����ŷ�
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
��������ɢ����������MainSearch�У�����MainSearch���������̣������ŷ�
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
        //��������
    }
    return Move;
}