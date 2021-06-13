#include "move.h"
#include "ucci.h"
#include "hash.h"
#include "base.h"
#include "buffer.h"
#include "sortmove.h"

const long long MAX_TIME = 1000;               //最长搜索时间

extern boardStruct board;

extern uint16 wmvKiller[LIMIT_DEPTH][2];//杀手着法表

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
// 重复裁剪
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
静态搜索，用法同AlphaBeta。在主要变例搜索到叶节点时使用静态搜索代替直接返回局面估值，防止水平线效应。
*/
int QuiescSearch(int Alpha, int Beta)
{
    /*int vl = RepPruning(Beta);
    if (vl > -MATE_VALUE)
        return vl;
    */
    if (board.nowDepth == LIMIT_DEPTH)//若达到最大搜索深度返回估值
    {
        return board.Evaluate();
    }
    int Val = 0, bestVal = MIN_VAL;
    int Moves[MAX_MOVES];
    int numOfMoves=0;
    if(board.InCheck())//若当前在将军，则返回所有着法
        numOfMoves = SortMove(Moves,HISTORY);
    else
    {
        Val = board.Evaluate();
        if (Val >= Beta)
            return Val;
        bestVal = Val;
        Alpha = MAX(Alpha, Val);
        numOfMoves = SortMove(Moves,MVVLVA,true);//否则返回所有吃子着法，即局面大分变化着法
    }
    
    for (int i = 0; i < numOfMoves; i++)//遍历着法
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


/*空着裁剪*/
const bool NO_NULL = true; // "SearchCut()"的参数，是否禁止空着裁剪
const int NULL_MARGIN = 400;   // 空步裁剪的子力边界，过于粗糙

#define R 2

extern int historyTable[65536];

inline void UpdateHistory(int move, int depth)
{
    historyTable[move] += depth * depth;
}

bool NullOkay()// 判断是否允许空步裁剪
{                 
    return (board.playerSide == 0 ? board.redVal : board.blackVal) > NULL_MARGIN;
}

bool NullSafe()// 判断是否允许空步裁剪
{                 
    return (board.playerSide == 0 ? board.redVal : board.blackVal) > 230;
}

/*
BestMove PVS(boardStruct& board,int Alpha, int Beta)
主要变例搜索，调用方法同AlphaBeta
带置换表裁剪、空着裁剪、历史启发
*/
BestMove PVS(int depth, int Alpha, int Beta, bool bNoNull)
{

    if (depth<=0)
        return { QuiescSearch(Alpha, Beta), 0 };

    // 重复裁剪；
    int vl = RepPruning(Beta);
    if (vl > -MATE_VALUE) 
        return { vl,0 };
    
    //搜索前先在置换表中查找
    int mv=0;
    /*vl = LookUpHash(Alpha, Beta, depth, bNoNull,mv);
    if (vl > -MATE_VALUE)
        return { vl,mv };*/
    
    int val = 0, bestVal = MIN_VAL, bestMove = 0,hashf = HASH_ALPHA;
    //尝试空着裁剪；
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
    while ((move = MoveSort.Next()))//遍历着法
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
        StoreHash(hashf,bestMove, bestVal, depth);//添加置换项
        SetBestMove(bestMove, depth, wmvKiller[board.nowDepth]);
    }
    return { bestVal,bestMove };
}


/*
int MainSearch(boardStruct& board)
将所有零散变量整合在MainSearch中，调用MainSearch，传入棋盘，返回着法
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
        //对称局面，所以要反转
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
        //迭代加深
    }
    return Move.BestMove;/**/
}
