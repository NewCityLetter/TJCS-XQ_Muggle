#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include<cstdint>
#include "ucci.h"
#include "log.h"


using namespace std;


static char chFen[maxInputLen];//��ʱ����
static uint32_t movesList[maxMovesNum];

extern char fetFenMoves[5];

/***************************************************************************
  �������ƣ�AnalyPositCom
  ��    �ܣ��ֽ�position�����Ϣ������ǰ���洢����UcciComPosit.fen,��moves��ĺ����ŷ�������UcciComPosit.followUpMoves
  ���������char* strP��positionָ���ӵ��ַ�����UcciComPositStruct& UcciComPosit ����positionָ����������Ϣ�Ľṹ�壩
  �� �� ֵ����positionָ���ӵ���fen����startpos���򷵻�true�����򷵻�false
  ˵    ����
***************************************************************************/
static bool AnalyPositCom(char* strP, UcciComPositStruct& UcciComPosit)
{
	int movesPosit = MyStrstr(strP, " moves");
	if (MyStrcasencmp(strP, " fen ", strlen(" fen ")) == 0) 
	{
		strP += strlen(" fen ");
		
		int m = movesPosit == 0 ? strlen(strP) : movesPosit - 1;
		memcpy(chFen, strP, m);
		UcciComPosit.fen = chFen;
	}
	else if (MyStrcasencmp(strP, " startpos", strlen(" startpos")) == 0) 
	{
		UcciComPosit.fen = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR r - - 0 1";
	}
	else 
	{
		return false;
	}
	UcciComPosit.movesNum = 0;
	//printf("movesPosit=%d  sreing=%s\n", movesPosit, strP);
	if (movesPosit != 0) 
	{
		strP += (movesPosit - 1)+strlen(" ");  // strPָ������ָ��" moves"��ĵ�һ���ַ�������֮ǰstrP���ˡ� fen �����ȣ����Ե���
		//printf("movesPosit=%d  sreing=%s\n", movesPosit,strP);
		UcciComPosit.movesNum = strlen(strP) / 5;
		strP++;
		for (int i = 0; i < UcciComPosit.movesNum; i++) 
		{
			movesList[i] = *((uint32_t*)strP);
			strP += 5;
		}
		UcciComPosit.followUpMoves = movesList;

	}
	return true;


}

/***************************************************************************
  �������ƣ�BootCom
  ��    �ܣ���ȡ������ĵ�һ��ָ�����ȡ��ucci���򷵻�comUcci�����򷵻�comUnkown
  �����������
  �� �� ֵ������ȡ��ucci���򷵻�comUcci�����򷵻�comUnkown
  ˵    ����
***************************************************************************/
ucciComEnum BootCom()
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



/***************************************************************************
  �������ƣ�IdleCom
  ��    �ܣ����������״̬ʱ��ȡָ��
  ���������UcciComPositStruct& UcciComPosit������positionָ���а�������Ϣ����UcciComGoTimeStruct& UcciComGoTime������go timieָ���а�������Ϣ��
  �� �� ֵ�����ض�ȡ����ָ�������
  ˵    ����
***************************************************************************/
ucciComEnum IdleCom(UcciComPositStruct& UcciComPosit, UcciComGoTimeStruct& UcciComGoTime)
{
	char readStr[maxInputLen];
	char* strP;
	cin.getline(readStr, maxInputLen);
	
	strP = readStr;
	Log a;
	a.baseMsg();
	a.error(strP);

	if (MyStrcasencmp(strP, "isready", 7) == 0) 
	{
		return comIsready;
	}
	else if (MyStrcasencmp(strP, "position", 8) == 0) 
	{
		strP += strlen("position");
		if (AnalyPositCom(strP, UcciComPosit)) 
		{
			return comPositon;
		}
		else 
		{
			return comUnkown;
		}

	}
	else if (MyStrcasencmp(strP, "go time", 7) == 0) 
	{
		return comGoTime;
	}
	else if (MyStrcasencmp(strP, "quit", 4) == 0) 
	{
		return comQuit;
	}
	else {
		return comUnkown;
	}

}


/*
Сд���ڷ�
��д���췽

k/K:��/˧
a/A:ʿ/��
b/B:��/��
n/N:��
r/R:��
c/C:��
p/P:��/��

*/

/***************************************************************************
  �������ƣ�JudgeChess
  ��    �ܣ���FEN������ĸ����ʾ������ת������pcRed[7]��pcBlack[7]����������
  ���������һ����ĸ���������ӵ�����
  �� �� ֵ��int������1-6���������Ӳ������κ��࣬�򷵻�7
  ˵    ����
***************************************************************************/
static int JudgeChess(char ch)
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


/***************************************************************************
  �������ƣ�Board
  ��    �ܣ���UcciComPosit.fen������������һ�γԹ��ӵľ���ת��Ϊһ��16x16������
  ���������UcciComPositStruct& UcciComPosit������positionָ���а�������Ϣ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void Board(UcciComPositStruct& UcciComPosit, uint8* currentBoard)
{
	const char* strP = UcciComPosit.fen;
	int i, j,spe;//x���꣬y���꣬���ӵ�����
	//int8 currentBoard[256];
	int pcRed[7];
	int pcBlack[7];
	const int addNum = 16;
	pcRed[0] = addNum+ KING_FROM;  //˧
	pcRed[1] = addNum + ADVISOR_FROM;//��
	pcRed[2] = addNum + BISHOP_FROM;//��
	pcRed[3] = addNum + KNIGHT_FROM;//��
	pcRed[4] = addNum + ROOK_FROM;//��
	pcRed[5] = addNum + CANNON_FROM;//��
	pcRed[6] = addNum + PAWN_FROM;//��

	for (i = 0; i < 7; i++) {
		pcBlack[i] = pcRed[i] + 16;
	}



	for (j = 3; *strP != ' '; strP++, j++)
	{
		for (i = 3; *strP != '/' && *strP != ' '; i++, strP++)
		{
			if (*strP >= 'A' && *strP <= 'Z')
			{
				spe = JudgeChess(*strP);
				if (spe < 7)
				{
					if (pcRed[spe] < 32)
					{
						currentBoard[CoordXY(j, i)] = pcRed[spe];
						pcRed[spe]++;
					}
				}

			}
			else if (*strP >= 'a' && *strP <= 'z')
			{
				spe = JudgeChess(*strP);
				if (spe < 7) {
					if (pcBlack[spe] < 48)
					{
						currentBoard[CoordXY(j, i)] = pcBlack[spe];
						pcBlack[spe]++;
					}
				}
			}
			else if (*strP >= '0' && *strP <= '9')
			{
				i += *strP - '0' - 1;
			}
			else if (*strP == '/' || *strP == ' ')
			{
				break;
			}
		}
		if (*strP == ' ')
			break;

	}
	 
	UcciComPosit.player = (UcciComPosit.movesNum+(( *(strP + 1) == 'b') ? 1 : 0))%2;

}

/***************************************************************************
  �������ƣ�Moves
  ��    �ܣ���moves��ĺ����ŷ���16x16����������ʾ
  ���������const UcciComPositStruct& UcciComPosit���ں��ŷ��б��ŷ�������int8* currentBoard����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void Moves(const UcciComPositStruct& UcciComPosit, uint8* currentBoard)
{
	int i;
	//int fourthBit, thirdBit, secondBit, firstBit;  //һ��uint32_t�����ҵ���ĵ���λ������λ���ڶ�λ����һλ
	int lineBeforeMove, colBeforeMove, lineAfterMove, colAfterMove;
	for (i = 0; i < UcciComPosit.movesNum; i++) 
	{
		//fourthBit = UcciComPosit.followUpMoves[i] / 1000;
		char* p = (char*)(UcciComPosit.followUpMoves) + 4 * i;
		colBeforeMove = *p - 'a' + 3;
		lineBeforeMove = 9 - (*(p+1)-'0') + 3;
		colAfterMove = *(p + 2) - 'a' + 3;
		lineAfterMove = 9 - (*(p + 3)-'0') + 3;
		//printf("UcciComPosit.movesNum=%d move %d %d %d %d\n", UcciComPosit.movesNum, lineBeforeMove, colBeforeMove, lineAfterMove, colAfterMove);
		if (currentBoard[CoordXY(lineBeforeMove, colBeforeMove)] != 0
			&& currentBoard[CoordXY(lineAfterMove, colAfterMove)]==0) 
		{
			currentBoard[CoordXY(lineAfterMove, colAfterMove)] = currentBoard[CoordXY(lineBeforeMove, colBeforeMove)];
			currentBoard[CoordXY(lineBeforeMove, colBeforeMove)] = 0;
		}

	}
}



static char NumberToChar(int8 number)
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


/***************************************************************************
  �������ƣ�GenerateFen
  ��    �ܣ�����ǰ��������fen��
  ���������const int8* currentBoard��ǰ���棬
  �� �� ֵ��
  ˵    ����
***************************************************************************/
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


/***************************************************************************
  �������ƣ�GetMoves
  ��    �ܣ�������˼���õ�������ŷ�ת�����ַ���
  ���������int Move���ŷ�
  �� �� ֵ��ת���ɵ��ַ���
  ˵    ����
***************************************************************************/
void PrintMoves(int Move,char*moves)
{
	int endPos = GETEND(Move);
	int beginPos = GETBEGIN(Move);
	int beginLine = beginPos >> 4;
	int beginCol = beginPos & 15;
	int endLine = endPos >> 4;
	int endCol = endPos & 15;
	//char moves[5];
	moves[0] = beginCol - 3 + 'a';
	moves[1] = 12 - beginLine+'0';
	moves[2] = endCol - 3 + 'a';
	moves[3] = 12 - endLine+'0';
	moves[4] = '\0';
	return ;
}


