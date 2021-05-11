#include "base.h"
#include "board.h"

void GetPreMove()
{
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
}