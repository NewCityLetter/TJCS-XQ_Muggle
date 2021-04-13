#ifndef BASE_H
#define BASE_H
#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/timeb.h>

//数据类型预定义
typedef char            int8;
typedef unsigned char   uint8;
typedef short           int16;
typedef unsigned short  uint16;
typedef int             int32;
typedef unsigned int    uint32;
typedef long long       int64;

const int MAX_MOVES = 512;				// 最大的历史走法数
const int MIN_VAL = -0x7ffffff;         // 最小值
const int MAX_VAL = 0x7ffffff;          // 最大值
const int MAX_DEPTH = 32;

extern int DEPTH;                          //当前搜索深度
extern bool isNormalEnd;				   //标志当前是否正常弹出
extern long long beginSearchTime;          //开始搜索的时间
/*
获取当前时间，单位为毫秒
*/
inline long long GetTime()
{
	timeb tb;
	ftime(&tb);
	return (long long)tb.time * 1000 + tb.millitm;
}

#endif