#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <cmath>
#include <set>
#include <map>
using namespace std;

#ifdef LOCAL
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#else
#define eprintf(...) 42
#endif

typedef long long ll;

const int N = (int)1e6 + 1000;

int add(int a, int b, int m)
{
	a += b;
	return a < m ? a : a - m;
}

int str[N];
int tmp[5][N];
int cnt[N];
int lcp[N];
int revArr[N];
int *pos = tmp[0];
int *arr = tmp[1], *narr = tmp[2];
int *col = tmp[3], *ncol = tmp[4];

void buildSA(int n)
{
	int classes = 0;
	memset(cnt, 0, sizeof(cnt));
	for (int i = 0; i < n; i++)
	{
		cnt[str[i]]++;
		classes = max(classes, str[i] + 1);	
	}
	pos[0] = 0;
	for (int i = 1; i < classes; i++)
		pos[i] = pos[i - 1] + cnt[i - 1];
	for (int i = 0; i < n; i++)
	{
		col[i] = str[i];
		arr[pos[col[i]]++] = i;
	}

	pos[0] = 0;
	for (int i = 1; i < classes; i++)
		pos[i] = pos[i - 1] + cnt[i - 1];

	for (int shift = 1; shift < n; shift *= 2)
	{
		for (int i = 0; i < n; i++)
		{
			int npos = arr[i] - shift;
			if (npos < 0)
				npos += n;
			narr[pos[col[npos]]++] = npos;
		}

		pos[0] = 0;
		ncol[narr[0]] = 0;
		for (int i = 1; i < n; i++)
		{
			ncol[narr[i]] = ncol[narr[i - 1]];
			if (col[narr[i]] != col[narr[i - 1]] || col[add(narr[i], shift, n)] != col[add(narr[i - 1], shift, n)])
				pos[++ncol[narr[i]]] = i;
		}

		swap(col, ncol);
		swap(arr, narr);

		if (col[arr[n - 1]] == n - 1)
			break;
	}
}

int main()
{
	freopen("input.txt", "r", stdin);

	int len;
	scanf("%d", &len);
	for (int i = 0; i < len; i++)
		scanf("%d", &str[i]);
	str[len++] = 0;
	buildSA(len);
	for (int i = 1; i < len; i++)
		printf("%d ", arr[i]);
	printf("\n");

	return 0;
}
