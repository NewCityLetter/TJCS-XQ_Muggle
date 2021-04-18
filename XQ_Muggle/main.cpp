#include "log.h"
#include "base.h"
#include "move.h"
#include "ucci.h"
#include "board.h"
#include "zobrist.h"

using namespace std;

ZobristTable Zobrist;
int DEPTH = 4;                          //当前搜索深度
bool isNormalEnd = 1;					//标志当前是否正常弹出
long long beginSearchTime = 0;          //开始搜索的时间
boardStruct board;
int additionalDepth;//若最深层结点是置换表中搜索得到的，此变量记录该局面的已搜深度+DEPTH，否则为DEPTH

int main()
{
	board.ClearBoard();
	bool isQuit = false;
	UcciComPositStruct UcciComPosit;
	UcciComGoTimeStruct UcciComGoTime;
	char getMoves[5];
	InitZobrist();
	if (BootCom() != comUcci)
	{
		return 0;
	}
	else
	{
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
			board.ClearBoard();
			Board(UcciComPosit, board.currentBoard);
			Moves(UcciComPosit, board.currentBoard);
			board.GetCurrentPosition();
			board.playerSide = UcciComPosit.player;
			/*for (int i = 16; i < 48; i++)
			{
				PrintMoves(board.currentPosition[i], getMoves);
				printf("i=%d pos=%s\n",i, getMoves);
			}*/
			board.InitValue();


			printf("nowPlayer=%d red=%d balck=%d nowVal=%d\n", board.playerSide, board.redVal, board.blackVal, board.Evaluate());

			/*if (board.InCheck())
			{
				printf("IN CHECK!\n");
			}
			else printf("NOT IN CHECK!\n");*/
			for (int i = 2; i <= 0xc; printf("\n"), i++)
				for (int j = 2; j <= 0xb; j++)
				{
					if (i == 2 && j == 2)
					{
						printf("  ");
						continue;
					}
					if (i == 2)
					{
						printf(" %c ", 'A' + j - 3);
						continue;
					}
					if (j == 2)
					{
						printf("%d ", 9 - i + 3);
						continue;
					}/**/
					if (board.currentBoard[(i << 4) + j] == 0)
						printf("0");
					printf("%d ", board.currentBoard[(i << 4) + j]);

				}


		}
		else if (com == comGoTime)
		{
			int Move = MainSearch(board);
			cout << "这个着法：" << Move << endl;
			PrintMoves(Move, getMoves);
			std::cout << "bestmove " << getMoves << std::endl;

			board.MakeInCheckMove(Move);
			for (int i = 2; i <= 0xc; printf("\n"), i++)
				for (int j = 2; j <= 0xb; j++)
				{
					if (i == 2 && j == 2)
					{
						printf("  ");
						continue;
					}
					if (i == 2)
					{
						printf(" %c ", 'A' + j - 3);
						continue;
					}
					if (j == 2)
					{
						printf("%d ", 9 - i + 3);
						continue;
					}
					if (board.currentBoard[(i << 4) + j] == 0)
						printf("0");
					printf("%d ", board.currentBoard[(i << 4) + j]);

				}

			fflush(stdout);

		}
		else if (com == comQuit)
		{
			isQuit = true;
		}
	}
	printf("bye");
	return 0;
}