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

const int K = 5;
const int LIM = 6;

const int N = (int)2e6;
const int B = (int)2e7;

int buf_ptr;
int buf[B];

int head[N];
int new_str12[N];
int new_sa0[N];
int rev_sa12[N];
int sa0[N];

int* create_arr(int size)
{
	buf_ptr += size;
	assert(buf_ptr <= B);
	return buf + buf_ptr - size;
}

int add_zeros(int &n, int str[])
{
	int added = 0;
	while (n % 3 != 0)
	{
		str[n++] = 0;
		added++;
	}

	str[n] = str[n + 1] = 0;

	return added;
}

int get_pos(int suff, int n)
{
	assert(suff % 3 != 0);
	if (suff % 3 == 1)
		return suff / 3;
	return suff / 3 + n / 3;
}

int get_suff(int pos, int n)
{
	if (pos < n / 3)
		return pos * 3 + 1;
	return (pos - n / 3) * 3 + 2;
}

bool build_str12(int n, int str[], int str12[])
{
	int n12 = 0;
	for (int mod = 1; mod <= 2; mod++)
		for (int i = mod; i < n; i += 3)
				str12[n12++] = i;
	
	int head_size = n + K;

	for (int sh = 2; sh >= 0; sh--)
	{
		fill(head, head + head_size, 0);
		
		for (int i = 0; i < n12; i++)
		{
			int c = str[str12[i] + sh];
			head[c + 1]++;
		}

		for (int i = 1; i < head_size; i++)
			head[i] += head[i - 1];

		for (int i = 0; i < n12; i++)
		{
			int c = str[str12[i] + sh];
			new_str12[head[c]++] = str12[i];
		}

		copy(new_str12, new_str12 + n12, str12);
	}

	int k = 1;

	for (int i = 0; i < n12; i++)
	{
		bool equals = true;
		int cur_suff = str12[i];
		
		if (i != 0)
		{
			int prev_suff = str12[i - 1];
			for (int sh = 0; sh < 3; sh++)
				if (str[prev_suff + sh] != str[cur_suff + sh])
					equals = false;
		}

		if (!equals)
			k++;

		int cur_pos = get_pos(cur_suff, n);
		new_str12[cur_pos] = k;
	}

	copy(new_str12, new_str12 + n12, str12);
	
	return k == n12;
}

void build_sa0(int n, int str[])
{
	int n0 = 0;
	for (int i = 0; i < n; i += 3)
		sa0[n0++] = i;

	int head_size = n + K;

	for (int sh = 1; sh >= 0; sh--)
	{
		fill(head, head + head_size, 0);
		
		for (int i = 0; i < n0; i++)
		{
			int suff = sa0[i];
			int c = (sh == 0 ? str[suff] : rev_sa12[get_pos(suff + 1, n)]);
			head[c + 1]++;
		}

		for (int i = 1; i < head_size; i++)
			head[i] += head[i - 1];

		for (int i = 0; i < n0; i++)
		{
			int suff = sa0[i];
			int c = (sh == 0 ? str[suff] : rev_sa12[get_pos(suff + 1, n)]);
			new_sa0[head[c]++] = suff;
		}

		copy(new_sa0, new_sa0 + n0, sa0);
	}
}

void build_sa(int n, int str[], int sa[])
{
	if (n <= LIM)
	{
		auto slow_cmp = [&] (int suff1, int suff2)
		{
			for (int sh = 0; max(suff1 + sh, suff2 + sh) < n; sh++)
			{
				if (str[suff1 + sh] != str[suff2 + sh])
					return str[suff1 + sh] < str[suff2 + sh];
			}
			return suff1 > suff2;
		};

		for (int i = 0; i < n; i++)
			sa[i] = i;
		sort(sa, sa + n, slow_cmp);
		
		return;
	}

	int added = add_zeros(n, str);

	int n12 = 2 * n / 3;
	int *str12 = create_arr(n12 + K);
	int* sa12 = create_arr(n12 + K);
	if (build_str12(n, str, str12))
	{
		for (int i = 0; i < n12; i++)
			sa12[str12[i] - 1] = i;
	}
	else
		build_sa(n12, str12, sa12);

	for (int i = 0; i < n12; i++)
		rev_sa12[sa12[i]] = i;

	int n0 = n / 3;
	build_sa0(n, str);

	auto cmp = [&] (int suff1, int suff2)
	{
		while (suff1 < n && suff2 < n)
		{
			if (str[suff1] != str[suff2])
				return str[suff1] < str[suff2];
			if (suff1 % 3 == 0 || suff2 % 3 == 0)
			{
				suff1++;
				suff2++;
				continue;
			}
			int pos1 = get_pos(suff1, n); 
			int pos2 = get_pos(suff2, n);
			return rev_sa12[pos1] < rev_sa12[pos2];
		}
		return suff1 > suff2;
	};
	
	for (int i = 0; i < n12; i++)
		sa12[i] = get_suff(sa12[i], n);

	merge(sa0, sa0 + n0, sa12, sa12 + n12, sa, cmp);
	int ptr = 0;
	for (int i = 0; i < n; i++)
		if (sa[i] < n - added)
			sa[ptr++] = sa[i];
}

int main()
{
#ifdef LOCAL
	freopen ("input.txt", "r", stdin);
#endif

	int n;
	scanf("%d", &n);
	int* str = create_arr(n + K);
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &str[i]);
		assert(1 <= str[i] && str[i] <= n);
	}

	int* sa = create_arr(n + K);
	build_sa(n, str, sa);

	for (int i = 0; i < n; i++)
		printf("%d ", sa[i]);
	printf("\n");

	return 0;
}
