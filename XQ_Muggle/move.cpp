#include "move.h"
#include "ucci.h"
#include "hash.h"
#include "base.h"
#include "buffer.h"
#include "sortmove.h"

const long long MAX_TIME =58000;        //�����ʱ��

extern boardStruct board;

extern int32 KillerMove[LIMIT_DEPTH][2];//ɱ���ŷ���

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
    //ɱ��ü�
    if (-MATE_VALUE + board.nowDepth >= Beta)
        return -MATE_VALUE + board.nowDepth;
    // �ظ��ü���
    int vlRep = board.RepStatus();
    if (vlRep)
        return board.RepValue(vlRep);
    //���ﵽ���������ȷ��ع�ֵ
    if (board.nowDepth == LIMIT_DEPTH)
        return board.Evaluate(Alpha,Beta);

    int Val = 0, bestVal = -MATE_VALUE;
    int numOfMoves=0;
    SearchMove Moves[MAX_MOVES];
    //����ǰ�ڽ������򷵻������ŷ�
    int isCheck=board.LastMove().ucbCheck;
    if(isCheck)
        numOfMoves = SortMove(Moves,HISTORY);
    else
    {
        Val = board.Evaluate(Alpha,Beta);
        if (Val >= Beta)
            return Val;
        bestVal = Val;
        Alpha = MAX(Alpha, Val);
        numOfMoves = SortMove(Moves,MVVLVA,true);//���򷵻����г����ŷ����������ֱ仯�ŷ�
    }
    
    for (int i = 0; i < numOfMoves; i++)//�����ŷ�
    {
        if(Moves[i].val<=0&&!isCheck)
            break;
        if(!board.MakeMove(Moves[i].val))
            continue;
        
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
    return (board.playerSide == 0 ? board.redVal : board.blackVal) > 200;
}

//�㴰����ȫ����
int SearchCut(int depth, int Beta, bool bNoNull = false)
{
    if (depth <= 0)
    {
#ifdef DEBUG
        nodeNum++;
#endif
        //return board.Evaluate(Beta - 1, Beta);
        return QuiescSearch(Beta - 1, Beta);
    }

    if (board.nowDepth == MAX_DEPTH)
        return board.Evaluate(Beta - 1, Beta);

    if (-MATE_VALUE + board.nowDepth >= Beta)
        return -MATE_VALUE + board.nowDepth;

    // �ظ��ü���
    int vlRep = board.RepStatus();
    if (vlRep)
        return board.RepValue(vlRep);


    //����ǰ�����û����в���
    int mvHash = 0;
    int vl = LookUpHash(Beta - 1, Beta, depth, mvHash);
    if (vl > -MATE_VALUE)
        return vl;/**/

    int val = 0, bestVal = -MATE_VALUE, bestMove = 0, hashf = HASH_ALPHA;
    //���Կ��Ųü���
    if (!bNoNull && !board.LastMove().ucbCheck && NullOkay())
    {
        board.NullMove();
        val = -SearchCut(depth - R - 1, -Beta + 1, NO_NULL);
        board.UndoNullMove();

        if (val >= Beta)
        {
            if (NullSafe())
            {
                //StoreHash(HASH_BETA, 0, val, MAX(depth, 3));
                return val;
            }
            if (SearchCut(depth - R, Beta, NO_NULL) >= Beta)
            {
                //StoreHash(HASH_BETA, 0, val, MAX(depth, 2));
                return val;
            }
        }
    }

    sortMoveStruct MoveSort;
    int isOnly = 0;
    if (board.LastMove().ucbCheck)
    {
        isOnly = MoveSort.InCheckMove(mvHash);
    }
    else
    {
        MoveSort.FullInit(mvHash);
    }
    int move;
    while ((move = MoveSort.Next()))//�����ŷ�
    {
        if (!board.MakeMove(move))
            continue;

        int nextDepth = (board.LastMove().ucbCheck||(isOnly!=0)) ? depth : depth - 1;

        val = -SearchCut(nextDepth, 1 - Beta);
        board.UndoMakeMove();

        if (val > bestVal)
        {
            bestVal = val;
            if (val >= Beta)//�ض�
            {
                bestMove = move;
                hashf = HASH_BETA;
                StoreHash(hashf, bestMove, bestVal, depth);//����û���
                SetBestMove(bestMove, depth, KillerMove[board.nowDepth]);
                return bestVal;
            }
        }

        if (GetTime() - beginSearchTime >= MAX_TIME)
        {
            isNormalEnd = 0;
            return bestVal;
        }
    }
    //���ض�
    if (isNormalEnd)
    {
        StoreHash(hashf, isOnly, bestVal, depth);//����û���
    }
    return bestVal==-MATE_VALUE?board.nowDepth-MATE_VALUE:bestVal;
}

/*
BestMove PVS(boardStruct& board,int Alpha, int Beta)
��Ҫ�������������÷���ͬAlphaBeta
���û���ü������Ųü�����ʷ����
*/
int PVS(int depth, int Alpha, int Beta, bool bNoNull)
{
    
    if (depth<=0)
    {
#ifdef DEBUG
        nodeNum++;
#endif
        return QuiescSearch(Alpha, Beta);
    }
    if (board.nowDepth == MAX_DEPTH)
        return board.Evaluate(Alpha,Beta);

    if (-MATE_VALUE + board.nowDepth >= Beta)
        return -MATE_VALUE + board.nowDepth;
    // �ظ��ü���
    int vlRep = board.RepStatus();
    if (vlRep)
        return board.RepValue(vlRep);

#ifdef DEBUG
    if(debugflag)
        printf("before hash\n");
#endif
    

    //����ǰ�����û����в���
    int mvHash=0;
    int vl = LookUpHash(Alpha, Beta, depth ,mvHash);
    if (vl > -MATE_VALUE)
        return vl;/**/

#ifdef DEBUG
    if(debugflag)
        printf("before null\n");
#endif   
    
    int val = 0, bestVal = -MATE_VALUE, bestMove = 0,hashf = HASH_ALPHA;

    sortMoveStruct MoveSort;
    int isOnly=0;
    if(board.LastMove().ucbCheck)
    {
        isOnly=MoveSort.InCheckMove(mvHash);
    }
    else
    {
        MoveSort.FullInit(mvHash);
    }
    int move;
    //if(debugflag)
    //    printf("before search\n");
    while ((move = MoveSort.Next()))//�����ŷ�
    {
        //PRINT(move);
        //printf(" side=%d depth=%d pvs\n",board.playerSide,depth);
        if (!board.MakeMove(move))
            continue;

#ifdef DEBUG
        if(debugflag)
        {
            PRINT(move);
            printf("before depth=%d val=%d bestval=%d\n",depth,val,bestVal);
        }
#endif
        
        int nextDepth=(board.LastMove().ucbCheck||(isOnly!=0))?depth:depth-1;
        if (bestVal == -MATE_VALUE)
            val = -PVS(nextDepth, -Beta, -Alpha);
        else
        {
            val = -SearchCut(nextDepth, -Alpha);
            //val = -PVS(nextDepth, -bestVal - 1, -bestVal);
            if (val > Alpha&& val < Beta)
                val = -PVS(nextDepth,-Beta, -Alpha);
        }
        board.UndoMakeMove();
#ifdef DEBUG
        if(debugflag)
        {
            PRINT(move);
            printf(" depth=%d val=%d bestval=%d\n",depth,val,bestVal);
        }
#endif
        

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
        SetBestMove(bestMove, depth, KillerMove[board.nowDepth]);
    }
    
    return bestVal==-MATE_VALUE?board.nowDepth-MATE_VALUE:bestVal;
}

int searchRoot(int depth)
{
    int val = 0, bestVal = -MATE_VALUE, bestMove = 0;
    sortMoveStruct MoveSort;
    int isOnly=0;
    if(board.InCheck())
    {
        isOnly=MoveSort.InCheckMove(0);
    }
    else
    {
        MoveSort.FullInit(0);
    }
    int move;
    while ((move = MoveSort.Next()))//�����ŷ�
    {
        if (!board.MakeMove(move))
            continue;
        //�ж��Ƿ�Խ�
        if (!board.currentPosition[SELF_SIDE(board.playerSide) + KING_FROM]) 
        {
            board.UndoMakeMove();
            return move;
        }

        int nextDepth = board.LastMove().ucbCheck ? depth : depth - 1;
        if (bestVal == -MATE_VALUE)
            val = -PVS(nextDepth, -MATE_VALUE, MATE_VALUE,1);
        else
        {
            val = -SearchCut(nextDepth, -bestVal);
            //val = -PVS(nextDepth, -bestVal - 1, -bestVal);
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
        SetBestMove(bestMove, depth, KillerMove[board.nowDepth]);
    return bestMove;
}

/*
int MainSearch(boardStruct& board)
��������ɢ����������MainSearch�У�����MainSearch���������̣������ŷ�
*/
int MainSearch()
{
    beginSearchTime= GetTime();
#define OPENBOOK
#ifdef OPENBOOK
    if (openBookFlag)
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
            //printf("begin=%d end=%d\n", Begin, End);
            Begin = Begin  - ((Begin & 15)<<1)+ 14;
            End = End  - ((End & 15) << 1)+ 14;
            Move = (End << 8) + Begin;

            //PRINT(Move);
            return Move;
        }
        if(chooseFrom==1)
            return OpenBook[index1].Move;
        openBookFlag = 0;
    }
#endif
    

    int Move = 0;
    isNormalEnd = 1;
    int depth = 1;
    //long long lastTime=beginSearchTime;

    while (isNormalEnd&&depth<=MAX_DEPTH)
    {
        
        
        int tmpMove = searchRoot(depth/*,rootMoveSort*/);
        if (!isNormalEnd)
            break;
#ifdef DEBUG
        printf("depth=%d thistime=%lld tottime=%lld nodeNum=%d move=%d ", depth,GetTime()-lastTime,GetTime()-beginSearchTime,nodeNum,tmpMove);
        lastTime=GetTime();
        PRINT(tmpMove);
        printf("\n");
        int i;
        for (i = 0;path[i].BestMove; i++)
        {
            PRINT(path[i].BestMove);
            printf("(%d) -> ",path[i].Val);
        }
        printf(" i=%d\n\n",i);
        nodeNum=0;
        memset(path, 0, sizeof(path));
#endif
        if (isNormalEnd&&tmpMove)
            Move = tmpMove;
        depth++;

        //��������
    }
    return Move;/**/
}
