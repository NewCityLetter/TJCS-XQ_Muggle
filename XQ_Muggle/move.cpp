#include "move.h"
#include "ucci.h"
#include "hash.h"
/*// �����;������ۺ���
inline int Evaluate(const boardStruct& Board) {
    int vl;
    vl = Board.Evaluate();
    return vl == Board.DrawValue() ? vl - 1 : vl;
}*/
extern int additionalDepth;
const long long MAX_TIME = 3000;               //�����ʱ��

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
ʱ��������
uint32 AlphaBeta(boardStruct & Board)
���뵱ǰ����
AlphaBeta��֦
��һ�� Min �ڵ�� ��ֵ���κ�һ�����ڵ�Ħ�ֵʱ �������ýڵ�������ӽڵ�
��һ�� Max �ڵ�� ��ֵ���κ�һ�����ڵ�Ħ�ֵʱ �������ýڵ�������ӽڵ�
��һ���ڵ��У�������ֻ��һ����������
BestMove AlphaBeta(boardStruct& Board, int Alpha, int Beta)
{
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

int MAX(int a, int b)
{
    return a > b ? a : b;
}

/*
BestMove QuiescSearchh(boardStruct& Board, int Alpha, int Beta)
��̬�������÷�ͬAlphaBeta������Ҫ����������Ҷ�ڵ�ʱʹ�þ�̬��������ֱ�ӷ��ؾ����ֵ����ֹˮƽ��ЧӦ��
*/
int QuiescSearch(boardStruct& Board, int Alpha, int Beta)
{
    if (Board.nowDepth == LIMIT_DEPTH+DEPTH)//���ﵽ���������ȷ��ع�ֵ
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
    if(Board.InCheck())//����ǰ�ڽ������򷵻������ŷ�
        numOfMoves = SortMove(board, Moves);
    else
    {
        Val = Board.Evaluate();
        if (Val >= Beta)
            return Val;
        bestVal = Val;
        Alpha = MAX(Alpha, Val);
        numOfMoves = SortMove(board, Moves,true);//���򷵻����г����ŷ����������ֱ仯�ŷ�
    }
    
    /*if (numOfMoves == 0)
    {
        if (Board.InCheck())
            return MIN_VAL;
    }*/
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
��Ҫ�������������÷���ͬAlphaBeta
*/
/*BestMove PVS(boardStruct& Board,int Alpha, int Beta)
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
        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            isNormalEnd = 0;
            return { bestVal,bestMove };
        }
    }
    return { bestVal,bestMove };
}*/

/*���Ųü�*/
const bool NO_NULL = true; // "SearchCut()"�Ĳ������Ƿ��ֹ���Ųü�
const int NULL_MARGIN = 500;   // �ղ��ü��������߽� 

#define R 2

bool NullOkay(boardStruct& Board)
{                 // �ж��Ƿ�����ղ��ü�
    return (Board.playerSide == 0 ? Board.redVal : Board.blackVal) > NULL_MARGIN;
}
/*
BestMove PVS(boardStruct& Board,int Alpha, int Beta)
��Ҫ�������������÷���ͬAlphaBeta
*/

BestMove PVS(boardStruct& Board, int Alpha, int Beta, bool bNoNull)
{

    if (Board.nowDepth >= DEPTH)
    {
        /*if (!bNoNull)
            additionalDepth = DEPTH;
        else
            additionalDepth = 0;*/
        additionalDepth = Board.nowDepth;
        return { QuiescSearch(Board, Alpha, Beta), 0 };
    }

    int hashf = HASH_ALPHA;
    //����ǰ�����û����в���
    int mv;
    int hashDepth;
    int vl = LookUpHash(Board, Alpha, Beta, Board.nowDepth, mv, hashDepth);
    if (vl > -MATE_VALUE)
    {
        //std::cout << "��ֵ��" << vl << " �ŷ���" << mv << '\n';
        //if (mv == 0 && Board.nowDepth || mv != 0)
        additionalDepth = hashDepth + Board.nowDepth;
        return { vl,mv };
    }/**/

    if (Board.nowDepth == MAX_DEPTH)
    {
        additionalDepth = MAX_DEPTH;
        return { Board.Evaluate(), 0 };
    }
    int val = 0, bestVal = MIN_VAL, bestMove = 0;

    //���Կ��Ųü���
    if (!bNoNull && !Board.InCheck() && NullOkay(Board)) {
        Board.NullMove();
        Board.nowDepth += R;
        val = -PVS(Board, -Beta, -Beta + 1, NO_NULL).Val;
        Board.nowDepth -= R;
        Board.UndoNullMove();

        if (val >= Beta) {
            return { val,0 };
        }
    }/**/

    int Moves[MAX_MOVES];
    int numOfMoves = SortMove(Board, Moves);

    struct tempHashNode
    {
        int Val;
        int Move;
        int Depth;
    };
    tempHashNode tmpHash;
    tmpHash.Val = MIN_VAL;
    tmpHash.Move = 0;
    tmpHash.Depth = 0;
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
            val = -PVS(Board, -Beta, -Alpha).Val;
        }
        else
        {
            val = -PVS(Board, -Alpha - 1, -Alpha).Val;
            if (val > Alpha&& val < Beta)
            {
                val = -PVS(Board, -Beta, -Alpha).Val;
            }
        }
        Board.UndoMakeMove();


        if (val > bestVal)
        {
            bestVal = val;
            bestMove = Moves[i];
            if (val >= Beta)
            {
                if (additionalDepth - Board.nowDepth >= tmpHash.Depth)
                {
                    tmpHash.Val = val;
                    tmpHash.Move = Moves[i];
                    tmpHash.Depth = additionalDepth - Board.nowDepth;
                    hashf = HASH_BETA;
                }
                break;
            }

            if (val > Alpha)
            {
                hashf = HASH_PV;
                Alpha = val;
            }
        }

        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            isNormalEnd = 0;
            return { bestVal,bestMove };
        }
    }
    if (isNormalEnd) {
        StoreHash(Board, hashf, bestMove, bestVal, additionalDepth - Board.nowDepth);//����û���

        if (Board.nowDepth == 0)
        {
            printf("now_MAX_DEPTH=%d Alpha=%d Beta=%d\n", DEPTH,Alpha,Beta);

            printf("flag=%d now_HASH_ALPHA_val=%d depth=%d Move=",hashf, bestVal, additionalDepth - Board.nowDepth);
            PRINT(bestMove);

            printf("\n");
        }/**/
        
    }
    return { bestVal,bestMove };
}




/*
int MainSearch(boardStruct& board)
��������ɢ����������MainSearch�У�����MainSearch���������̣������ŷ�
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
        //��������
    }
    return Move.BestMove;
}


