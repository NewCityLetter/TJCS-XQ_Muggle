#pragma once
#include<string.h>


/***************************************************************************
  函数名称：MyStrcasencmp
  功    能：比较字符串s1和s2前len个字符是否相等，不区分大小写
  输入参数：
  返 回 值：相等返回0；不相等返回第一个不相等字符的ASCII差值
  说    明：
***************************************************************************/
inline int MyStrcasencmp(const char s1[], const char s2[], const int len)
{
	int i;
	for (i = 0; i < len && (s2[i] != '\0' || s1[i] != '\0'); i++) {
		if (s1[i] != s2[i])
		{
			if (((s1[i] >= 'a' && s1[i] <= 'z') || (s1[i] >= 'A' && s1[i] <= 'Z')) &&
				((s2[i] >= 'a' && s2[i] <= 'z') || (s2[i] >= 'A' && s2[i] <= 'Z')) &&
				((s1[i] - s2[i] == 32) || (s1[i] - s2[i] == -32)))
				;

			else
				return s1[i] - s2[i];
		}
	}
	return 0;
}


/***************************************************************************
  函数名称：MyStrstr
  功    能：在字符串str中寻找substr串第一次出现的位置，从左到右寻找，区分字母的大小写
  输入参数：
  返 回 值：若找到，则返回1—n，若未找到，则返回0
  说    明：
***************************************************************************/
inline int MyStrstr(const char* str, const char* substr)
{
	int i, j, k, n;

	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == substr[0]) {
			n = i;
			for (j = 0, k = 0; substr[j] != '\0'; j++, n++) {
				if (str[n] != substr[j])
					k++;
			}
			if (k == 0) {
				return i + 1;
			}
		}
	}
	return 0; //return值可根据需要修改
}


/***************************************************************************
  函数名称：CoordXY
  功    能：
  输入参数：棋子的行列坐标
  返 回 值：棋子在一维数组中的坐标
  说    明：
***************************************************************************/
inline int CoordXY(int line, int col)
{
	return col + (line << 4);

}
