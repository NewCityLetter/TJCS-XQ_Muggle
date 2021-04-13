#ifndef BASE_H
#define BASE_H
#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/timeb.h>

//��������Ԥ����
typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;
typedef long long       int64;

const int MAX_MOVES = 512;				// ������ʷ�߷���
const int MIN_VAL = -0x7ffffff;         // ��Сֵ
const int MAX_VAL = 0x7ffffff;          // ���ֵ
const int MAX_DEPTH = 32;

extern int DEPTH;                          //��ǰ�������
extern bool isNormalEnd;				   //��־��ǰ�Ƿ���������
extern long long beginSearchTime;          //��ʼ������ʱ��
/*
��ȡ��ǰʱ�䣬��λΪ����
*/
inline long long GetTime()
{
	timeb tb;
	ftime(&tb);
	return (long long)tb.time * 1000 + tb.millitm;
}

#endif