/******************************************
 *
 *	author	: Gerald Zhang
 *	date	: Tue Aug 27 14:26:15 2013
 *	company	: CST, Tsinghua Univ
 *
******************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <set>
#include <ctime>
#include <algorithm>
#define min(a,b)	((a)<(b)?(a):(b))
#define max(a,b)	((a)>(b)?(a):(b))
#define abs(a)	((a)<0?-(a):(a))
#define inf 214748364
#define pi 3.141592653589793
using namespace std;
typedef long long ll;
const int maxn = 1000;
bool g[maxn + 10][maxn + 10];
int main()
{
	srand(time(0));
	int n = rand()%500 + 500;
	printf("%d\n",n);
	while( n-- )
	{
		int x = rand()%maxn + 1;
		int y = rand()%maxn + 1;
		while( g[x][y] )
			x = rand()%maxn + 1, y = rand()%maxn + 1;
		printf("%d %d\n", x, y);
	}
	
	return 0;
}

