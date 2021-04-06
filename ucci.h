#pragma once

//��������Ԥ����
typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;
typedef long long       int64;



//FROMΪ��ʼ��� TOΪ�������
const int32 KING_FROM = 0;
const int32 ADVISOR_FROM = 1;
const int32 ADVISOR_TO = 2;
const int32 BISHOP_FROM = 3;
const int32 BISHOP_TO = 4;
const int32 KNIGHT_FROM = 5;
const int32 KNIGHT_TO = 6;
const int32 ROOK_FROM = 7;
const int32 ROOK_TO = 8;
const int32 CANNON_FROM = 9;
const int32 CANNON_TO = 10;
const int32 PAWN_FROM = 11;
const int32 PAWN_TO = 15;

const int maxInputLen = 8192; //������ָ�����󳤶�
const int maxMovesNum = 120;
//const int limitedTime = 60000; //�޶�ʱ�䣬������ʧ��


enum ucciComEnum {
	comUcci,comIsready,comPositon,comGoTime,comQuit,comUnkown
};



struct UcciComPositStruct {
	const char* fen;  //ָ���е�FEN������
	int movesNum;    //FEN���д������ĺ����ŷ���
	uint32_t* followUpMoves; //�����ŷ�
	
};

struct UcciComGoTimeStruct {
	int thinkingTime;   // �����˼��ʱ��
};




