#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include<cstdint>
#include "ucci.h"
#include "tools.h"

using namespace std;


static char chFen[maxInputLen];
static uint32_t movesList[maxMovesNum];

bool AnalyPositCom(char* strP, UcciComPositStruct& UcciComPosit)
{
	int movesPosit = MyStrstr(strP, " moves");
	if (MyStrcasencmp(strP, " fen ", strlen(" fen ")) == 0) {
		strP += strlen(" fen ");

		int m = movesPosit == 0 ? strlen(strP) : movesPosit - 1;
		memcpy(chFen, strP, m);
		UcciComPosit.fen = chFen;
	}
	else if (MyStrcasencmp(strP, " startpos", strlen(" startpos")) == 0) {
		UcciComPosit.fen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";

	}
	else {
		return false;
	}
	UcciComPosit.movesNum = 0;
	if (movesPosit != 0) {
		strP += (movesPosit - 1) + strlen(" moves");  // strP指针现在指向" moves"后的第一个字符
		UcciComPosit.movesNum = strlen(strP) / 5;
		strP++;
		for (int i = 0; i < UcciComPosit.movesNum; i++) {
			movesList[i] = *((uint32_t*)strP);
			strP += 5;
		}
		UcciComPosit.followUpMoves = movesList;

	}
	return true;


}


ucciComEnum bootCom()
{
	char readStr[maxInputLen];
	cin.getline(readStr, maxInputLen);
	if (MyStrcasencmp(readStr, "ucci", 4) == 0) {
		return comUcci;
	}
	else {
		return comUnkown;
	}

}




ucciComEnum idleCom(UcciComPositStruct& UcciComPosit, UcciComGoTimeStruct& UcciComGoTime)
{
	char readStr[maxInputLen];
	char* strP;
	cin.getline(readStr, maxInputLen);

	strP = readStr;
	if (MyStrcasencmp(strP, "isready", 7) == 0) {
		return comIsready;
	}
	else if (MyStrcasencmp(strP, "position", 8) == 0) {
		strP += strlen("position");
		if (AnalyPositCom(strP, UcciComPosit)) {
			return comPositon;
		}
		else {
			return comUnkown;
		}

	}
	else if (MyStrcasencmp(strP, "go time", 7) == 0) {

	}
	else if (MyStrcasencmp(strP, "quit", 4) == 0) {
		return comQuit;
	}
	else {
		return comUnkown;
	}

}


/*
小写：黑方
大写：红方

k/K:将/帅
a/A:士/仕
b/B:象/相
n/N:马
r/R:车
c/C:炮
p/P:卒/兵

*/


int JudgeChess(char ch)
{
	if (ch == 'k' || ch == 'K')
		return 0;
	else if (ch == 'a' || ch == 'A')
		return 1;
	else if (ch == 'b' || ch == 'B')
		return 2;
	else if (ch == 'n' || ch == 'N')
		return 3;
	else if (ch == 'r' || ch == 'R')
		return 4;
	else if (ch == 'c' || ch == 'C')
		return 5;
	else if (ch == 'p' || ch == 'P')
		return 6;
	else
		return 7;

}



void board(const UcciComPositStruct& UcciComPosit)
{
	const char* strP = UcciComPosit.fen;
	int i, j,spe;//x坐标，y坐标，棋子的种类
	int8 currentBoard[256];
	int pcRed[7];
	int pcBlack[7];
	const int addNum = 16;
	pcRed[0] = addNum+ KING_FROM;  //帅
	pcRed[1] = addNum + ADVISOR_FROM;//仕
	pcRed[2] = addNum + BISHOP_FROM;//相
	pcRed[3] = addNum + KNIGHT_FROM;//马
	pcRed[4] = addNum + ROOK_FROM;//车
	pcRed[5] = addNum + CANNON_FROM;//炮
	pcRed[6] = addNum + PAWN_FROM;//兵

	for (i = 0; i < 7; i++) {
		pcBlack[i] = pcRed[i] + 16;
	}



	for ( j=3; strP; strP++,j++) {
		for (i = 3; *strP != '/'; i++,strP++) {
			if (*strP >= 'A' && *strP <= 'Z') {
				spe = JudgeChess(*strP);
				if (spe < 7) {
					if (pcRed[spe] < 32) {
						currentBoard[CoordXY(j, i)] = pcRed[spe];
						pcRed[spe]++;
					}
				}

			}
			else if (*strP >= 'a' && *strP <= 'z') {
				spe = JudgeChess(*strP);
				if (spe < 7) {
					if (pcBlack[spe] < 48) {
						currentBoard[CoordXY(j, i)] = pcBlack[spe];
						pcBlack[spe]++;
					}
				}
			}
			else if (*strP >= '0' && *strP <= '9') {
				i += *strP - '0' - 1;
			}
			else if (*strP == '/') {
				break;
			}
		}

	}


}


void moves(const UcciComPositStruct& UcciComPosit, int8* currentBoard)
{
	int i;
	//int fourthBit, thirdBit, secondBit, firstBit;  //一个uint32_t数从右到左的第四位，第三位，第二位，第一位
	int lineBeforeMove, colBeforeMove, lineAfterMove, colAfterMove;
	for (i = 0; i < UcciComPosit.movesNum; i++) {
		//fourthBit = UcciComPosit.followUpMoves[i] / 1000;
		colBeforeMove = UcciComPosit.followUpMoves[i] / 1000 - 'a' + 3;
		lineBeforeMove = 9 - (UcciComPosit.followUpMoves[i] / 100 % 10) + 3;
		colAfterMove = UcciComPosit.followUpMoves[i] / 10%10 - 'a' + 3;
		lineAfterMove = 9 - (UcciComPosit.followUpMoves[i]% 10) + 3;
		if (currentBoard[CoordXY(lineBeforeMove, colBeforeMove)] != 0
			&& currentBoard[CoordXY(lineAfterMove, colAfterMove)]==0) {
			currentBoard[CoordXY(lineAfterMove, colAfterMove)] = currentBoard[CoordXY(lineBeforeMove, colBeforeMove)];
			currentBoard[CoordXY(lineBeforeMove, colBeforeMove)] = 0;
		}

	}
}

/*
小写：黑方
大写：红方

k/K:将/帅
a/A:士/仕
b/B:象/相
n/N:马
r/R:车
c/C:炮
p/P:卒/兵

*/
char NumberToChar(int8 number)
{
	const int addNum = 16;
	if (number == addNum + KING_FROM)
		return 'K';
	else if (number >= addNum + ADVISOR_FROM && number <= addNum + ADVISOR_TO)
		return 'A';
	else if (number >= addNum + BISHOP_FROM && number <= addNum + BISHOP_TO)
		return 'B';
	else if (number >= addNum + KNIGHT_FROM && number <= addNum + KNIGHT_TO)
		return 'N';
	else if (number >= addNum + ROOK_FROM && number <= addNum + ROOK_TO)
		return 'R';
	else if (number >= addNum + CANNON_FROM && number <= addNum + CANNON_TO)
		return 'C';
	else if (number >= addNum + PAWN_FROM && number <= addNum + PAWN_TO)
		return 'P';
	else if (number == addNum + KING_FROM + 16)
		return 'k';
	else if (number >= addNum + ADVISOR_FROM + 16 && number <= addNum + ADVISOR_TO + 16)
		return 'a';
	else if (number >= addNum + BISHOP_FROM + 16 && number <= addNum + BISHOP_TO + 16)
		return 'b';
	else if (number >= addNum + KNIGHT_FROM + 16 && number <= addNum + KNIGHT_TO + 16)
		return 'n';
	else if (number >= addNum + ROOK_FROM + 16 && number <= addNum + ROOK_TO + 16)
		return 'r';
	else if (number >= addNum + CANNON_FROM + 16 && number <= addNum + CANNON_TO + 16)
		return 'c';
	else if (number >= addNum + PAWN_FROM + 16 && number <= addNum + PAWN_TO + 16)
		return 'p';
	else
		return 0;

}

void GenerateFen(const int8* currentBoard,const UcciComPositStruct UcciComPosit)
{
	int i, j,m=0,n;
	char chFen[maxInputLen];
	for (j = 3; j <= 12; j++) {
		for (i = 3; i <= 11; i++) {
			if (currentBoard[CoordXY(j, i)] != 0) {
				if (NumberToChar(currentBoard[CoordXY(j, i)]) != 0) {
					chFen[m] = NumberToChar(currentBoard[CoordXY(j, i)]) != 0;
					m++;
				}
			}
			else {
				for (n = 0; currentBoard[CoordXY(j, i+n)]==0&& i + n<=11; n++) {
					;
				}
				chFen[m] = n;
				m++;
				i += n-1;
			}

		}
		chFen[m] = '/';
		m++;
	}


}