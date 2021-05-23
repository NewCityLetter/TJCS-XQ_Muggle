#pragma once
#include "base.h"
#include "board.h"


const int maxInputLen = 8192; //������ָ�����󳤶�
const int maxMovesNum = 120;

/*����ȡ����ָ�������?*/
enum ucciComEnum 
{
	comUcci, comIsready, comPositon, comGoTime, comQuit, comUnkown
};



struct UcciComPositStruct 
{
	const char* fen;  //ָ���е�FEN������
	int movesNum;    //FEN���д�������ĺ����ŷ���?
	uint32_t* followUpMoves; //�����ŷ�
	int8 player;

};

ucciComEnum BootCom();
ucciComEnum IdleCom(UcciComPositStruct& UcciComPosit);
void Board(UcciComPositStruct& UcciComPosit, uint8* currentBoard);
void Moves(const UcciComPositStruct& UcciComPosit, uint8* currentBoard);
void PrintMoves(int Move, char* moves);
