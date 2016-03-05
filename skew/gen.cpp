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
	for (int i = 0; i < n; i++)
		printf("%d ", rand() % n + 1);

	return 0;
}
