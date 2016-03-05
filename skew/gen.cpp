#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <cmath>
#include <ctime>
#include <functional>
#include <sstream>
#include <fstream>
#include <valarray>
#include <complex>
#include <queue>
#include <cassert>
using namespace std;

int main(int, char **argv)
{
	int seed = atoi(argv[1]);
	srand(seed);

	int n = (int)1e6;
	printf("%d\n", n);
	vector<int> str = {1};
	while ((int)str.size() < n)
	{
		int old_len = (int)str.size();
		for (int i = 0; i < old_len; i++)
			str.push_back(str[i] ^ 1);
	}
	for (int i = 0; i < n; i++)
		printf("%d ", rand() % n + 1);

	return 0;
}
