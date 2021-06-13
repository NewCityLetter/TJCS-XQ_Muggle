﻿#include "log.h"
#include "base.h"
#include "move.h"
#include "ucci.h"
#include "board.h"
#include "buffer.h"
#include "zobrist.h"
#include "premove.h"
#include <stdlib.h> 
#include <windows.h>

using namespace std;

boardStruct board;
ZobristTable Zobrist;
PreMove preMove;
int DEPTH = 4;                          //当前搜索深度
bool isNormalEnd = 1;					//标志当前是否正常弹出
bool openBookFlag = 1;					//标记是否从开局库中查询
long long beginSearchTime = 0;          //开始搜索的时间
int historyTable[65536];
uint16 wmvKiller[LIMIT_DEPTH][2];//杀手着法表
uint64_t openBookKey;					//对称局面Key

extern void NewHash(int nHashScale);
extern void DelHash(void);

void printboard()
{
	HANDLE handle;//创建句柄 
	handle = GetStdHandle(STD_OUTPUT_HANDLE);//取标准输入输出句柄 


	printf("nowPlayer=%d red=%d balck=%d nowVal=%d\n", board.playerSide, board.redVal, board.blackVal, board.Evaluate());
	for (int i = 2; i <= 0xc; printf("\n"), i++)
		for (int j = 2; j <= 0xb; SetConsoleTextAttribute(handle, 15), j++)
		{
			if (i == 2 && j == 2)
			{
				printf("   ");
				continue;
			}
			if (i == 2)
			{
				printf("  %c ", 'A' + j - 3);
				continue;
			}
			if (j == 2)
			{
				printf(" %d ", 9 - i + 3);
				continue;
			}
			if (board.currentBoard[(i << 4) + j] == 0)
			{
				printf("[  ]");
				continue;
			}
			if (board.currentBoard[(i << 4) + j] < 32)
				SetConsoleTextAttribute(handle, FOREGROUND_RED);
			if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) == KING_FROM)
				printf(" 将 ");
			else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= ADVISOR_TO)
				printf(" 士 ");
			else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= BISHOP_TO)
				printf(" 象 ");
			else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= KNIGHT_TO)
				printf(" 马 ");
			else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= ROOK_TO)
				printf(" 车 ");
			else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= CANNON_TO)
				printf(" 炮 ");
			else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= PAWN_TO)
				printf(" 兵 ");

		}SetConsoleTextAttribute(handle, 15);
}

int main()
{	
	NewHash(24); // 24=16MB, 25=32MB, 26=64MB, ...
	bool isQuit = false;
	UcciComPositStruct UcciComPosit;
	UcciComGoTimeStruct UcciComGoTime;
	char getMoves[5];
	InitZobrist();
	if (BootCom() != comUcci)
		return 0;
	else
	{
		GetPreMove();
		std::cout << "id name XQ_Muggle" << std::endl;
		std::cout << "id copyright 2021-2022 TongJi" << std::endl;
		std::cout << "id copyright 2021-2022 TongJi" << std::endl;
		std::cout << "id author Team Ten" << std::endl;
		std::cout << "ucciok" << std::endl;
		fflush(stdout);
	}
	while (!isQuit)
	{
		ucciComEnum com = IdleCom(UcciComPosit, UcciComGoTime);
		if (com == comIsready)
		{
			std::cout << "readyok" << std::endl;
			fflush(stdout);
		}
		else if (com == comPositon)
		{
			DelHash();
			NewHash(24);
			board.ClearBoard();
			Board(UcciComPosit, board.currentBoard);
			Moves(UcciComPosit, board.currentBoard);
			board.GetCurrentPosition();
			board.playerSide = UcciComPosit.player;
			board.InitValue();
			cout << board.zobr.dwKey << endl;
			cout << openBookKey << endl;

			HANDLE handle;//创建句柄 
			handle = GetStdHandle(STD_OUTPUT_HANDLE);//取标准输入输出句柄 
			

			printf("nowPlayer=%d red=%d balck=%d nowVal=%d\n", board.playerSide, board.redVal, board.blackVal, board.Evaluate());
			for (int i = 2; i <= 0xc; printf("\n"), i++)
				for (int j = 2; j <= 0xb;  SetConsoleTextAttribute(handle, 15),j++)
				{
					if (i == 2 && j == 2)
					{
						printf("   ");
						continue;
					}
					if (i == 2)
					{
						printf("  %c ", 'A' + j - 3);
						continue;
					}
					if (j == 2)
					{
						printf(" %d ", 9 - i + 3);
						continue;
					}
					if (board.currentBoard[(i << 4) + j] == 0)
					{
						printf("[  ]");
						continue;
					}
					if(board.currentBoard[(i << 4) + j]<32)
						SetConsoleTextAttribute(handle, FOREGROUND_RED);
					if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) == KING_FROM)
						printf(" 将 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= ADVISOR_TO)
						printf(" 士 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= BISHOP_TO)
						printf(" 象 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= KNIGHT_TO)
						printf(" 马 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= ROOK_TO)
						printf(" 车 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= CANNON_TO)
						printf(" 炮 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= PAWN_TO)
						printf(" 兵 ");

				}SetConsoleTextAttribute(handle, 15);


		}
		else if (com == comGoTime)
		{
			int Move = MainSearch();
			PrintMoves(Move, getMoves);
			if (Move == 0)
			{
				std::cout << "nobestmove" << std::endl;
				fflush(stdout);
				break;
			}
			std::cout << "bestmove " << getMoves << std::endl;

			board.MakeMove(Move);
			HANDLE handle;//创建句柄 
			handle = GetStdHandle(STD_OUTPUT_HANDLE);//取标准输入输出句柄 
			for (int i = 2; i <= 0xc; printf("\n"),i++)
				for (int j = 2; j <= 0xb; SetConsoleTextAttribute(handle, 15),  j++)
				{
					if (i == 2 && j == 2)
					{
						printf("   ");
						continue;
					}
					if (i == 2)
					{
						printf("  %c ", 'A' + j - 3);
						continue;
					}
					if (j == 2)
					{
						printf(" %d ", 9 - i + 3);
						continue;
					}
					if (board.currentBoard[(i << 4) + j] == 0)
					{
						printf("[  ]");
						continue;
					}
					if (board.currentBoard[(i << 4) + j] < 32)
						SetConsoleTextAttribute(handle, FOREGROUND_RED);
					if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) == KING_FROM)
						printf(" 将 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= ADVISOR_TO)
						printf(" 士 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= BISHOP_TO)
						printf(" 象 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= KNIGHT_TO)
						printf(" 马 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= ROOK_TO)
						printf(" 车 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= CANNON_TO)
						printf(" 炮 ");
					else if (PIECE_INDEX(board.currentBoard[(i << 4) + j]) <= PAWN_TO)
						printf(" 兵 ");

				}SetConsoleTextAttribute(handle, 15);

			fflush(stdout);

		}
		else if (com == comQuit)
			isQuit = true;
	}
	printf("bye");
	return 0;
}