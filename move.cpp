#include "move.h"
#include "ucci.h"
#include "hash.h"
#include "base.h"
#include "buffer.h"
#include "sortmove.h"

const long long MAX_TIME = 1000;               //�����ʱ��

extern boardStruct board;

extern uint16 wmvKiller[LIMIT_DEPTH][2];//ɱ���ŷ���

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
    printf("%s\n", moves);
}

extern void printboard();

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
    /*int vl = RepPruning(Beta);
    if (vl > -MATE_VALUE)
        return vl;
    */
    if (board.nowDepth == LIMIT_DEPTH)//���ﵽ���������ȷ��ع�ֵ
    {
        return board.Evaluate();
    }
    int Val = 0, bestVal = MIN_VAL;
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

    return bestVal==MIN_VAL?board.nowDepth+MIN_VAL:bestVal;
}


/*���Ųü�*/
const bool NO_NULL = true; // "SearchCut()"�Ĳ������Ƿ��ֹ���Ųü�
const int NULL_MARGIN = 400;   // �ղ��ü��������߽磬���ڴֲ�

#define R 2

extern int historyTable[65536];

inline void UpdateHistory(int move, int depth)
{
    historyTable[move] += depth * depth;
}

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
BestMove PVS(int depth, int Alpha, int Beta, bool bNoNull)
{

    if (depth<=0)
        return { QuiescSearch(Alpha, Beta), 0 };

    // �ظ��ü���
    int vl = RepPruning(Beta);
    if (vl > -MATE_VALUE) 
        return { vl,0 };
    
    //����ǰ�����û����в���
    int mv=0;
    /*vl = LookUpHash(Alpha, Beta, depth, bNoNull,mv);
    if (vl > -MATE_VALUE)
        return { vl,mv };*/
    
    int val = 0, bestVal = MIN_VAL, bestMove = 0,hashf = HASH_ALPHA;
    //���Կ��Ųü���
    if (!bNoNull && !board.InCheck() && NullOkay()) 
    {
        board.NullMove();
        val = -PVS(depth-R-1, -Beta, -Beta + 1, NO_NULL).Val;
        board.UndoNullMove();

        if (val >= Beta) 
        {
            if (NullSafe()) return {val,0};
            if (PVS(depth - R, Alpha, Beta, NO_NULL).Val>= Beta)
                return {val,0};
        }
    }

    /*********************************/
    sortMoveStruct MoveSort;
    MoveSort.Init();
    int move;
    /*int count = 0;
    if (DEPTH == 3 && depth == 3)
        {
            PRINT(wmvKiller[board.nowDepth][0]);
            printf("islegal=%d player=%d\n", board.LegalMove(wmvKiller[board.nowDepth][0]),board.playerSide);
        }
    ***********************************/
    while ((move = MoveSort.Next()))//�����ŷ�
    {
        /*if ((DEPTH==3&&depth==3)||(DEPTH==2&&depth==2))
        {
            count++;
            printf("player=%d DEPTH=%d nodedepth=%d cout=%d move=%d ",board.playerSide,DEPTH,depth,count, move);
            PRINT(move);
        }*/
        
        
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
        if (bestVal == MIN_VAL)
            val = -PVS(nextDepth, -Beta, -Alpha).Val;
        else
        {
            val = -PVS(nextDepth,-Alpha - 1, -Alpha).Val;
            if (val > Alpha&& val < Beta)
                val = -PVS(nextDepth,-Beta, -Alpha).Val;
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
    if (isNormalEnd&&bestMove) 
    {
        StoreHash(hashf,bestMove, bestVal, depth);//����û���
        SetBestMove(bestMove, depth, wmvKiller[board.nowDepth]);
    }
    return { bestVal,bestMove };
}


/*
int MainSearch(boardStruct& board)
��������ɢ����������MainSearch�У�����MainSearch���������̣������ŷ�
*/
int MainSearch()
{
    beginSearchTime= GetTime();

    if (openBookFlag)
    {
        int chooseFrom = 0;
        int index1 = lower_bound(OpenBook, OpenBook + 596737, Book{ board.zobr.dwKey,0 }) - OpenBook;
        if( index1<=596737&&OpenBook[index1].HashKey == board.zobr.dwKey)
        {
            PRINT(OpenBook[index1].Move);
            chooseFrom = 1;
            //return OpenBook[index1].Move;
        }
        //�Գƾ��棬����Ҫ��ת
        int index2 = lower_bound(OpenBook, OpenBook + 596737, Book{ openBookKey,0 }) - OpenBook;
        if( index2 <= 596737 && OpenBook[index2].HashKey == openBookKey)
        {
            if(chooseFrom==1&&OpenBook[index2].Score<=OpenBook[index1].Score)
                return OpenBook[index1].Move;
            PRINT(OpenBook[index2].Move);
            uint16 Move = OpenBook[index2].Move;
            uint8 Begin = Move & 255;
            uint8 End = Move >> 8;
            printf("begin=%d end=%d\n", Begin, End);
            Begin = Begin  - ((Begin & 15)<<1)+ 14;
            End = End  - ((End & 15) << 1)+ 14;
            Move = (End << 8) + Begin;

            PRINT(Move);
            return Move;
        }
        if(chooseFrom==1)
            return OpenBook[index1].Move;
        openBookFlag = 0;
    }/**/
    

    BestMove Move = {MIN_VAL,0};

    isNormalEnd = 1;
    DEPTH = 2;
    while (isNormalEnd&&DEPTH<=MAX_DEPTH)
    {
        BestMove tmpMove = PVS(DEPTH,MIN_VAL, MAX_VAL);
        if (isNormalEnd&&tmpMove.BestMove)
            Move = tmpMove;
        printf("depth=%d time=%lld val=%d move=", DEPTH,GetTime()-beginSearchTime,tmpMove.Val);
        PRINT(tmpMove.BestMove);
        DEPTH++;
        //��������
    }
    return Move.BestMove;/**/
}
