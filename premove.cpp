#include "base.h"
#include "board.h"

void GetPreMove()
{
    //将、士、象、马、兵预置数组
    for(int beginPos=0,endPos,count;beginPos<256;beginPos++)
    {
        if(!IN_BOARD(beginPos))
            continue;

        //King Move
        if(IN_FORT(beginPos))
        {
            count=0;
            for(int i=0;i<4;i++)
            {
                endPos=beginPos+ccKingDelta[i];
                if(IN_FORT(endPos))
                    preMove.kingPreMove[++count][beginPos]=endPos;
            }
            preMove.kingPreMove[0][beginPos]=count;
        }
        

        //Advisor Move
        if(IN_FORT(beginPos))
        {
            count=0;
            for(int i=0;i<4;i++)
            {
                endPos=beginPos+ccAdvisorDelta[i];
                if(IN_FORT(endPos))
                    preMove.advisorPreMove[++count][beginPos]=endPos;
            }
            preMove.advisorPreMove[0][beginPos]=count;
        }
        

        //Bishop Move
        count=0;
        for(int i=0;i<4;i++)
        {
            endPos=beginPos+2*ccAdvisorDelta[i];
            if(IN_BOARD(endPos)&&SAME_HALF(beginPos,endPos))
            {
                preMove.bishopPreMove[++count][beginPos]=endPos;
                preMove.bishopEye[count][beginPos]=beginPos+ccAdvisorDelta[i];
            }
                
        }
        preMove.bishopPreMove[0][beginPos]=count;

        //Knight Move
        count=0;
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<2;j++)
            {
                endPos=beginPos+ccKnightDelta[i][j];
                if(IN_BOARD(endPos))
                {
                    preMove.knightPreMove[++count][beginPos]=endPos;
                    preMove.knightLeg[count][beginPos]=beginPos+ccKingDelta[i];
                }
            }  
        }
        preMove.knightPreMove[0][beginPos]=count;

        //Pawn Move
        for(int i=0;i<2;i++)//side
        {
            count=0;
            endPos=SQUARE_FORWARD(beginPos,i);
            if(IN_BOARD(endPos))
                preMove.pawnPreMove[i][++count][beginPos]=endPos;
            if(AWAY_HALF(beginPos,i))
            {
                for(int j=-1;j<=1;j+=2)
                {
                    endPos=beginPos+j;
                    if(IN_BOARD(endPos))
                        preMove.pawnPreMove[i][++count][beginPos]=endPos;
                }
            }
            preMove.pawnPreMove[i][0][beginPos]=count;
        }
    }


    //车、炮预置数组。原理同状压dp，记录下每行、每列每种状态（状态使用位棋盘表示）的着法，在实际进行着法时仅判断目标点是否为自己子。
    //一行状态即可表示所有棋盘

    //行
    for(int i=3;i<12;i++)//子所在位置
    {
        for(int j=0;j<4096;j++)
        {
            if(!(j&(1<<i)))
                continue;
            int k;
            preMove.rookLinePreMove[i][j][0]=3;
            preMove.rookLinePreMove[i][j][1]=11;
            preMove.cannonLinePreCap[i][j][0]=0;
            preMove.cannonLinePreCap[i][j][1]=0;
            for(k=i+1;k<12;k++)//向右寻找
            {
                if(j&(1<<k))
                {
                    preMove.rookLinePreMove[i][j][1]=k;//车吃子
                    break;
                }
            }
            for(k=k+1;k<12;k++)
            {
                if(j&(1<<k))
                {
                    preMove.cannonLinePreCap[i][j][1]=k;//炮吃子
                    break;
                }
            }

           for(k=i-1;k>=3;k--)//向左寻找
            {
                if(j&(1<<k))
                {
                    preMove.rookLinePreMove[i][j][0]=k;//车吃子
                    break;
                }
            }
            for(k=k-1;k>=3;k--)
            {
                if(j&(1<<k))
                {
                    preMove.cannonLinePreCap[i][j][0]=k;//炮吃子
                    break;
                }
            }

        }
    }

    //列
    for(int i=3;i<13;i++)//子所在位置
    {
        for(int j=0;j<8192;j++)
        {
            if(!(j&(1<<i)))
                continue;
            int k;
            preMove.rookColPreMove[i][j][0]=3;
            preMove.rookColPreMove[i][j][1]=12;
            preMove.cannonColPreCap[i][j][0]=0;
            preMove.cannonColPreCap[i][j][1]=0;
            for(k=i+1;k<13;k++)//向下寻找
            {
                if(j&(1<<k))
                {
                    preMove.rookColPreMove[i][j][1]=k;//车吃子
                    break;
                }
            }
            for(k=k+1;k<13;k++)
            {
                if(j&(1<<k))
                {
                    preMove.cannonColPreCap[i][j][1]=k;//炮吃子
                    break;
                }
            }

           for(k=i-1;k>=3;k--)//向上寻找
            {
                if(j&(1<<k))
                {
                    preMove.rookColPreMove[i][j][0]=k;//车吃子
                    break;
                }
            }
            for(k=k-1;k>=3;k--)
            {
                if(j&(1<<k))
                {
                    preMove.cannonColPreCap[i][j][0]=k;//炮吃子
                    break;
                }
            }

        }
    }
}