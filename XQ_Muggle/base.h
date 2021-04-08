#ifndef BASE_H
#define BASE_H
#pragma once

#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

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
const int DEPTH = 6;                    //最大深度

#endif