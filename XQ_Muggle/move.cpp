#include "move.h"
#include "ucci.h"
#include "hash.h"
#include "base.h"
#include "buffer.h"
#include "sortmove.h"

const long long MAX_TIME = 2000;        //�����ʱ��

extern boardStruct board;

extern int32 wmvKiller[LIMIT_DEPTH][2];//ɱ���ŷ���

void PRINT(int Move)
{
    char moves[5];
    int endPos = GETEND(Move);
    int beginPos = GETBEGIN(Move);
    int beginLine = beginPos >> 4;
    int beginCol = beginPos & 15;
    int endLine = endPos >> 4;
    int endCol = endPos & 15;
    moves[0] = beginCol - 3 + 'a';
    moves[1] = 12 - beginLine + '0';
    moves[2] = endCol - 3 + 'a';
    moves[3] = 12 - endLine + '0';
    moves[4] = '\0';
    printf("%s", moves);
}

extern void printboard();

BestMove path[100];

/***************************************/
// �ظ��ü�
static int RepPruning(int vlBeta)
{
    int vlRep = board.RepStatus();
    if (vlRep > 0) 
        return board.RepValue(vlRep);
    return -MATE_VALUE;
}
/******************************************/

/*
BestMove QuiescSearchh(boardStruct& board, int Alpha, int Beta)
��̬�������÷�ͬAlphaBeta������Ҫ����������Ҷ�ڵ�ʱʹ�þ�̬��������ֱ�ӷ��ؾ����ֵ����ֹˮƽ��ЧӦ��
*/
int QuiescSearch(int Alpha, int Beta)
{
    if (-MATE_VALUE + board.nowDepth >= Beta)
        return -MATE_VALUE + board.nowDepth;
    // �ظ��ü���
    int vlRep = board.RepStatus();
    if (vlRep)
        return board.RepValue(vlRep);/**/
    if (board.nowDepth == LIMIT_DEPTH)//���ﵽ���������ȷ��ع�ֵ
    {
        return board.Evaluate();
    }
    int Val = 0, bestVal = -MATE_VALUE;
    int Moves[MAX_MOVES];
    int numOfMoves=0;
    if(board.InCheck())//����ǰ�ڽ������򷵻������ŷ�
        numOfMoves = SortMove(Moves,HISTORY);
    else
    {
        Val = board.Evaluate();
        if (Val >= Beta)
            return Val;
        bestVal = Val;
        Alpha = MAX(Alpha, Val);
        numOfMoves = SortMove(Moves,MVVLVA,true);//���򷵻����г����ŷ����������ֱ仯�ŷ�
    }
    
    for (int i = 0; i < numOfMoves; i++)//�����ŷ�
    {
        board.MakeMove(Moves[i]);
        board.ChangeSide();
        if (board.InCheck())
        {
            board.UndoMakeMove();
            board.ChangeSide();
            continue;
        }
        board.ChangeSide();
        Val = -QuiescSearch(-Beta, -Alpha);
        board.UndoMakeMove();

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
        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            isNormalEnd = 0;
            return bestVal;
        }
    }

    return bestVal==-MATE_VALUE?board.nowDepth-MATE_VALUE:bestVal;
}


/*���Ųü�*/
const bool NO_NULL = true; // "SearchCut()"�Ĳ������Ƿ��ֹ���Ųü�
const int NULL_MARGIN = 400;   // �ղ��ü��������߽磬���ڴֲ�

#define R 2

bool NullOkay()// �ж��Ƿ�����ղ��ü�
{                 
    return (board.playerSide == 0 ? board.redVal : board.blackVal) > NULL_MARGIN;
}

bool NullSafe()// �ж��Ƿ�����ղ��ü�
{                 
    return (board.playerSide == 0 ? board.redVal : board.blackVal) > 230;
}

/*
BestMove PVS(boardStruct& board,int Alpha, int Beta)
��Ҫ�������������÷���ͬAlphaBeta
���û���ü������Ųü�����ʷ����
*/
int PVS(int depth, int Alpha, int Beta, bool bNoNull)
{
    
    if (depth<=0)
        return QuiescSearch(Alpha, Beta);

    if (board.nowDepth == MAX_DEPTH)
        return board.Evaluate();

    if (-MATE_VALUE + board.nowDepth >= Beta)
        return -MATE_VALUE + board.nowDepth;
    // �ظ��ü���
    /**/int vlRep = board.RepStatus();
    if (vlRep)
        return board.RepValue(vlRep);


    //����ǰ�����û����в���
    int mv=0;
    int vl = LookUpHash(Alpha, Beta, depth ,mv);
    if (vl > -MATE_VALUE)
        return vl;/**/
    
    int val = 0, bestVal = -MATE_VALUE, bestMove = 0,hashf = HASH_ALPHA;
    //���Կ��Ųü���
    if (!bNoNull && !board.InCheck() && NullOkay()) 
    {
        board.NullMove();
        val = -PVS(depth-R-1, -Beta, -Beta + 1, NO_NULL);
        board.UndoNullMove();

        if (val >= Beta) 
        {
            if (NullSafe()) return val;
            if (PVS(depth - R, Alpha, Beta, NO_NULL)>= Beta)
                return val;
        }
    }

    sortMoveStruct MoveSort;
    MoveSort.Init();
    int move;
    while ((move = MoveSort.Next()))//�����ŷ�
    {
        
        board.MakeMove(move);
        board.ChangeSide();
        if (board.InCheck())
        {
            board.UndoMakeMove();
            board.ChangeSide();
            continue;
        }
        board.ChangeSide();

        int nextDepth=board.InCheck()?depth:depth-1;
        if (bestVal == -MATE_VALUE)
            val = -PVS(nextDepth, -Beta, -Alpha);
        else
        {
            val = -PVS(nextDepth,-Alpha - 1, -Alpha);
            if (val > Alpha&& val < Beta)
                val = -PVS(nextDepth,-Beta, -Alpha);
        }
        board.UndoMakeMove();

        if (val > bestVal)
        {
            bestVal = val;
            if (val >= Beta)
            {
                bestMove = move;
                hashf = HASH_BETA;
                break;
            }

            if (val > Alpha)
            {
                bestMove = move;
                path[board.nowDepth] = { val,move };
                hashf = HASH_PV;
                Alpha = val;
            }
        }
        
        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            isNormalEnd = 0;
            return bestVal;
        }
    }
    if (isNormalEnd)
    {
        StoreHash(hashf,bestMove, bestVal, depth);//����û���
        SetBestMove(bestMove, depth, wmvKiller[board.nowDepth]);
    }
    
    return bestVal;
}


int searchRoot(int depth)
{
    int val = 0, bestVal = -MATE_VALUE, bestMove = 0;
    sortMoveStruct MoveSort;
    MoveSort.Init();
    int move;
    while ((move = MoveSort.Next()))//�����ŷ�
    {
        board.MakeMove(move);
        board.ChangeSide();
        if (board.InCheck())
        {
            board.UndoMakeMove();
            board.ChangeSide();
            continue;
        }
        board.ChangeSide();

        int nextDepth = board.InCheck() ? depth : depth - 1;
        if (bestVal == -MATE_VALUE)
            val = -PVS(nextDepth, -MATE_VALUE, MATE_VALUE,1);
        else
        {
            val = -PVS(nextDepth, -bestVal - 1, -bestVal);
            if (val > bestVal)
                val = -PVS(nextDepth, -MATE_VALUE, -bestVal,1);
        }
        board.UndoMakeMove();

        if (val > bestVal)
        {
            bestVal = val;
            bestMove = move;
            path[board.nowDepth] = { val,move };
        }

        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            isNormalEnd = 0;
            return bestVal;
        }
    }
    if(isNormalEnd)
        SetBestMove(bestMove, depth, wmvKiller[board.nowDepth]);
    return bestMove;
}

/*
int MainSearch(boardStruct& board)
��������ɢ����������MainSearch�У�����MainSearch���������̣������ŷ�
*/
int MainSearch()
{
    beginSearchTime= GetTime();

    /*if (openBookFlag)
    {
        int chooseFrom = 0;
        int index1 = lower_bound(OpenBook, OpenBook + 596737, Book{ board.zobr.dwKey,0 }) - OpenBook;
        if( index1<=596737&&OpenBook[index1].HashKey == board.zobr.dwKey)
        {
            //PRINT(OpenBook[index1].Move);
            chooseFrom = 1;
            //return OpenBook[index1].Move;
        }
        //�Գƾ��棬����Ҫ��ת
        int index2 = lower_bound(OpenBook, OpenBook + 596737, Book{ openBookKey,0 }) - OpenBook;
        if( index2 <= 596737 && OpenBook[index2].HashKey == openBookKey)
        {
            if(chooseFrom==1&&OpenBook[index2].Score<=OpenBook[index1].Score)
                return OpenBook[index1].Move;
            //PRINT(OpenBook[index2].Move);
            uint16 Move = OpenBook[index2].Move;
            uint8 Begin = Move & 255;
            uint8 End = Move >> 8;
            printf("begin=%d end=%d\n", Begin, End);
            Begin = Begin  - ((Begin & 15)<<1)+ 14;
            End = End  - ((End & 15) << 1)+ 14;
            Move = (End << 8) + Begin;

            //PRINT(Move);
            return Move;
        }
        if(chooseFrom==1)
            return OpenBook[index1].Move;
        openBookFlag = 0;
    }*/
    

    int Move = 0;
    isNormalEnd = 1;
    int depth = 2;
    while (isNormalEnd&&depth<=MAX_DEPTH)
    {
        memset(path, 0, sizeof(path));
        int tmpMove = searchRoot(depth);
        if (!isNormalEnd)
            break;
        printf("depth=%d time=%lld move=%d ", depth,GetTime()-beginSearchTime,tmpMove);
        PRINT(tmpMove);
        printf("\n");
        int i;
        for (i = 0;path[i].BestMove; i++)
        {
            PRINT(path[i].BestMove);
            printf("(%d) -> ",path[i].Val);
        }
        printf(" i=%d\n\n",i);
        if (isNormalEnd&&tmpMove)
            Move = tmpMove;
        depth++;

        //��������
    }
    return Move;/**/
}
