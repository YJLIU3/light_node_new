/*************************************************************
FileName : testPrintfFunction.cpp
FileFunc : printf参数模拟
Version  : V0.1
Author   : Sunrier
Date     : 2011-08-29
Descp    : 简单模拟printf函数
*************************************************************/
#include <stdio.h>   
#include <stdarg.h>  
#include <stdlib.h>//itoa()......  
#include <string.h>  
#include <math.h>  
#include <ctype.h>  
#include <em_leuart.h>
#include "../board/leusart.h"
#include "../board/rcc.h"
#include "myprintf.h"
#pragma warning(disable:4996)



#define     MAXBUF      15  

#define     DEFAULT_PRECI       3   //符点型默认精度  


int add(const int, ...);


void ftoa(double InputData, char *OutputData, int  DEFAULT_Vaule);

//int main(void)
//{
//	int x, y;
//
//	//int x1=0xFEd278df;  
//	int x1 = 8989;
//	double doubleData = 89.78623;
//
//	int x3 = 0;
//	char *s1 = "1234567";
//
//	x = add(4, 1, 2, 3, 4);
//
//	y = add(3, 1, 2, 3);
//
//	printf("x=%B\n", x);
//
//	printf("y=%3\n", y);
//
//	myPrintf("Hello Sunrier!\n");
//
//	myPrintf("wwwwwwwww");
//
//	myPrintf("%s\n", s1);
//
//	myPrintf("x3=%x\n", x3);
//
//	myPrintf("x=%b\n", x);
//	printf("x1=%b\n", x1);
//	myPrintf("x1=%b\n", x1);
//
//	myPrintf("x1=0x%x s=%s y=%d\n", x1, s1, y);
//
//	myPrintf("doubleData=%f\n", doubleData);
//	myPrintf("doubleData=%3f\n", doubleData);
//	printf("doubleData=%3f\n", s1);
//	getchar();
//	return 0;
//}



int add(const int n, ...)
{
	va_list arg;//定义va_list类型变量arg  

	va_start(arg, n);//arg初始化,n是第一个可见参数名字;使arg指向第一个可选参数  

	int sum = 0;
	int tmp = 0;
	for (int i = 0; i < n; i++)
	{
		tmp = va_arg(arg, int);//返回参数列表中的当前参数并使arg指向参数列表中的下一个参数  
		sum += tmp;
	}

	va_end(arg);//把arg指针清为NULL  

	return sum;
}



int myPrintf(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	//STRING  
	//char ch;  
	const char *str;
	//const char *str1;  

	//OCT  
	int value;
	char buf[MAXBUF];

	//HEX  
	int offset;
	unsigned char ucI, ucLen;
	unsigned long ulValue;
	//unsigned char ucBuf[MAXBUF];  
	//unsigned long *ulpValue;  

	//FLOAT  
	double fdata;

	//BINARY  



	while (*format)
	{
		if ('%' != *format)
		{
			LEUART_Tx(LEUART0, *format++);
			continue;
		}

		switch (* ++format)
		{
		case    's':
			str = NULL;
			str = va_arg(ap, const char *);
			for (; *str != '\0'; str++)
			{
				LEUART_Tx(LEUART0, *str);
			}
			++format;//越过当前参数，移到下一个参数  
			break;

		case    'd':
			value = va_arg(ap, const int);
			memset(buf, 0, sizeof(buf));
			itoa(value, buf, 10);
			for (str = buf; *str != '\0'; str++)
			{
				LEUART_Tx(LEUART0, *str);
			}
			++format;
			break;
		case    'x':
			offset = 0;
			ucLen = 0;
			ulValue = va_arg(ap, const int);
			memset(buf, 0, sizeof(buf));

			if (!ulValue)
			{
				buf[ucLen] = ulValue;
				str = buf;
				LEUART_Tx(LEUART0, (*str + 48));
				++format;
				break;
			}

			for (ucI = 0; ucI < MAXBUF; ucI++)
			{
				if (pow(16, ucI) > ulValue)
				{
					ucLen = ucI;
					break;
				}
			}


			for (ucI = 0; ucI < ucLen; ucI++)
			{

				buf[ucI] = ulValue / pow(16, ucLen - ucI - 1);
				ulValue = ulValue % (unsigned long)pow(16, ucLen - ucI - 1);

			}

			//puts("0x");  
			for (str = buf; *str != '\0'; str++)
			{
				if ((*str >= 0) && (*str <= 9))
				{
					LEUART_Tx(LEUART0, (*str + 48));
				}
				else
				{
					LEUART_Tx(LEUART0, (*str + 87));
				}
			}
			++format;
			break;
		case    'X':
			offset = 0;
			ucLen = 0;
			ulValue = va_arg(ap, const int);
			memset(buf, 0, sizeof(buf));


			if (!ulValue)
			{
				buf[ucLen] = ulValue;
				str = buf;
				LEUART_Tx(LEUART0, (*str + 48));
				++format;
				break;
			}

			for (ucI = 0; ucI < MAXBUF; ucI++)
			{
				if (pow(16, ucI) > ulValue)
				{
					ucLen = ucI;
					break;
				}
			}


			for (ucI = 0; ucI < ucLen; ucI++)
			{

				buf[ucI] = ulValue / pow(16, ucLen - ucI - 1);
				ulValue = ulValue % (unsigned long)pow(16, ucLen - ucI - 1);

			}

			//puts("0X");  
			for (str = buf; *str != '\0'; str++)
			{
				if ((*str >= 0) && (*str <= 9))
				{
					LEUART_Tx(LEUART0, (*str + 48));
				}
				else
				{
					LEUART_Tx(LEUART0, (*str + 55));
				}
			}
			++format;
			break;
		case    'f':
			fdata = va_arg(ap, double);
			memset(buf, 0, sizeof(buf));
			ftoa(fdata, buf, DEFAULT_PRECI);
			for (str = buf; *str != '\0'; str++)
			{
				LEUART_Tx(LEUART0, *str);
			}
			++format;
			break;
		case    '.':
			if (isdigit(* ++format))
			{
				if ('f' == (* ++format))
				{
					--format;
					fdata = va_arg(ap, double);
					memset(buf, 0, sizeof(buf));
					int preci = *format - '0';
					ftoa(fdata, buf, preci);
					for (str = buf; *str != '\0'; str++)
					{
						LEUART_Tx(LEUART0, *str);
					}
					++format;
					++format;
				}
			}
			else
			{
				--format;
				if ('f' == (* ++format))
				{
					fdata = va_arg(ap, double);
					memset(buf, 0, sizeof(buf));
					int preci = 1;
					ftoa(fdata, buf, preci);
					for (str = buf; *str != '\0'; str++)
					{
						LEUART_Tx(LEUART0, *str); 
					}
					++format;
				}
			}
			break;
		case    'b':
			value = va_arg(ap, const int);
			memset(buf, 0, sizeof(buf));
			itoa(value, buf, 2);
			for (str = buf; *str != '\0'; str++)
			{
				LEUART_Tx(LEUART0, *str); 
			}
			++format;
			break;


		default:
			break;

		}



	}

	va_end(ap);

	return 0;
}



const double eps = 1e-12;

char s[30];

void reverse(char *s)
{
	int i, j;
	int len = strlen(s);
	for (i = 0, j = len - 1; i < j; i++, j--)
		s[i] ^= s[j] ^= s[i] ^= s[j];
}

/* ftoa:   convert double n to string s with digits*/
void ftoa(double n, char *s, int  digits)
{
	memset(s, 0, sizeof(s));
	int i = 0, p;
	bool sign = true;
	if (n < 0)
	{
		sign = false;
		n = -n;
	}
	long   t = (long)n;
	n = n - t;
	if (t == 0)
	{
		s[i++] = '0';
	}
	else
	{
		while (t != 0)
		{
			p = t % 10;
			t /= 10;
			s[i++] = p + '0';
		}
		if (!sign) s[i++] = '-';
		reverse(s);
	}
	if (fabs(n - 0.0) > eps)
	{
		s[i++] = '.';
		int cur_d = 0;
		while (cur_d < digits)
		{
			cur_d++;
			n *= 10.0;
			p = (long)n;
			n = n - p;
			s[i++] = p + '0';
		}
	}
	s[i] = '\0';
}