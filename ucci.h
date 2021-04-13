#pragma once
#include "tools.h"
#include"base.h"
#include"board.h"


const int maxInputLen = 8192; //所接受指令的最大长度
const int maxMovesNum = 120;

/*所读取到的指令的类型*/
enum ucciComEnum {
	comUcci, comIsready, comPositon, comGoTime, comQuit, comUnkown
};



struct UcciComPositStruct {
	const char* fen;  //指令中的FEN串或者
	int movesNum;    //FEN串中代表局面的后续着法数
	uint32_t* followUpMoves; //后续着法
	int8 player;

};

struct UcciComGoTimeStruct {
	int thinkingTime;   // 引擎的思考时间
};


ucciComEnum BootCom();
ucciComEnum IdleCom(UcciComPositStruct& UcciComPosit, UcciComGoTimeStruct& UcciComGoTime);
void Board(UcciComPositStruct& UcciComPosit, uint8* currentBoard);
void Moves(const UcciComPositStruct& UcciComPosit, uint8* currentBoard);
void PrintMoves(int Move, char* moves);
