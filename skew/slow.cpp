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

#ifdef LOCAL
	#define debug_flag true
#else
	#define debug_flag false
#endif

#define dbg(args...) { if (debug_flag) { _print(_split(#args, ',').begin(), args); cerr << endl; } else { void(0);} }

vector<string> _split(const string& s, char c) {
	vector<string> v;
	stringstream ss(s);
	string x;
	while (getline(ss, x, c))
		v.emplace_back(x);
	return v;
}

void _print(vector<string>::iterator) {}
template<typename T, typename... Args>
void _print(vector<string>::iterator it, T a, Args... args) {
    string name = it -> substr((*it)[0] == ' ', it -> length());
    if (isalpha(name[0]))
	    cerr << name  << " = " << a << " ";
	else
	    cerr << name << " ";
	_print(++it, args...);
}

typedef long long int int64;

int main()
{
#ifdef LOCAL
	freopen ("input.txt", "r", stdin);
#endif

	int n;
	scanf("%d", &n);
	vector<int> str(n);
	for (int i = 0; i < n; i++)
		scanf("%d", &str[i]);

	auto cmp = [&] (int a, int b)
	{
		while (a < n && b < n)
		{
			if (str[a] != str[b])
				return str[a] < str[b];
			a++;
			b++;
		}
		return a > b;
	};

	vector<int> sa(n);
	for (int i = 0; i < n; i++)
		sa[i] = i;

	sort(sa.begin(), sa.end(), cmp);

	for (int x : sa)
		printf("%d ", x);
	printf("\n");

	return 0;
}
