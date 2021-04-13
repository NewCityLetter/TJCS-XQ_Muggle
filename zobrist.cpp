#include "zobrist.h"

void InitZobrist(void)
{
    int32 i, j;
    RC4Struct rc4;

    rc4.InitZero();
    Zobrist.Player.InitRC4(rc4);
    for (i = 0; i < 14; i++)
    {
        for (j = 0; j < 256; j++)
        {
            Zobrist.Table[i][j].InitRC4(rc4);
        }
    }
}