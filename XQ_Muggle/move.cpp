#include "move.h"
#include "ucci.h"
#include "hash.h"
/*// �����;������ۺ���
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
���뵱ǰ����
AlphaBeta��֦
��һ�� Min �ڵ�� ��ֵ���κ�һ�����ڵ�Ħ�ֵʱ �������ýڵ�������ӽڵ�
��һ�� Max �ڵ�� ��ֵ���κ�һ�����ڵ�Ħ�ֵʱ �������ýڵ�������ӽڵ�
��һ���ڵ��У�������ֻ��һ����������
*/
BestMove AlphaBeta(boardStruct& Board, int Alpha/*initial negative inf*/, int Beta/*initial postive inf*/)
{
    int hashf = HASH_ALPHA;
    
    //����ǰ�����û����в���
    int mv;
    int vl = LookUpHash(Board, Alpha, Beta, Board.nowDepth, mv);
    if (vl > -MATE_VALUE)
    {
        //std::cout << "��ֵ��" << vl << " �ŷ���" << mv << '\n';
        if (mv == 0 && Board.nowDepth || mv != 0)
            return { vl,mv };
    }
    //����
    if (Board.nowDepth == DEPTH)
    {
        //printf("%d ", Board.Evaluate());
        StoreHash(Board, HASH_PV, 0, Board.Evaluate(), DEPTH);
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
                StoreHash(Board, HASH_BETA, Moves[i], Val, Board.nowDepth);//����û���
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
    StoreHash(Board, hashf, bestMove, bestVal, Board.nowDepth);//����û���
    return { bestVal,bestMove };
}

int GetRandomMove(boardStruct& Board)
{
    int Moves[MAX_MOVES];
    Board.GenerateMove(Moves);
    return Moves[1];
}