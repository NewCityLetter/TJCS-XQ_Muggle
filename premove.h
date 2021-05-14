#include "base.h"
#include "board.h"

#ifndef PREMOVE_H
#define PREMOVE_H

struct PreMove
{
    uint8 kingPreMove[8][256];
    uint8 advisorPreMove[8][256];
    uint8 bishopPreMove[8][256];
    uint8 bishopEye[8][256];
    uint8 knightPreMove[12][256];
    uint8 knightLeg[12][256];
    uint8 pawnPreMove[2][4][256];
    int16 rookLinePreMove[12][12][4096];  //数/位置/状态
    int16 rookColPreMove[12][12][8192];   //数/位置/状态
    int16 cannonLinePreMove[12][12][4096];//数/位置/状态
    int16 cannonColPreMove[12][12][8192]; //数/位置/状态
};
extern PreMove preMove;

void GetPreMove();

#endif 