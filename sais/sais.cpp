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
#include <bitset>
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

#ifdef LOCAL
    #define eprintf(...) fprintf(stderr, __VA_ARGS__)
#else
    #define eprintf(...) 42;
#endif

typedef long long int int64;

const int N = (int)2e6;
const int B = 5 * N;

const int S_TYPE = 0;
const int L_TYPE = 1;

int init_str[N];
int sa[N];
bool suff_type[N];
int l_ptr[N], r_ptr[N];
int new_sa[N];
int new_letter[N];

int buf_ptr;
int buf[B];

int* new_arr(int size)
{
	buf_ptr += size;
	assert(buf_ptr <= B);
	return buf + buf_ptr - size;
}

void calc_suff_types(int n, int str[])
{
	suff_type[n - 1] = S_TYPE;
	for (int i = n - 2; i >= 0; i--)
	{
		if (str[i] < str[i + 1] || (str[i] == str[i + 1] && suff_type[i + 1] == S_TYPE))
			suff_type[i] = S_TYPE;
		else
			suff_type[i] = L_TYPE;
	}
}

void find_backets_borders(int n, int str[])
{
	fill(l_ptr, l_ptr + n, 0);
	for (int i = 0; i < n; i++)
		l_ptr[str[i] + 1]++;
	for (int i = 1; i < n; i++)
		l_ptr[i] += l_ptr[i - 1];

	for (int i = 0; i < n; i++)
		r_ptr[i] = l_ptr[i + 1] - 1;
}

bool is_lms(int i)
{
	return i - 1 >= 0 && suff_type[i] == S_TYPE && suff_type[i - 1] == L_TYPE;
}

void left_run(int n, int str[])
{
	find_backets_borders(n, str);
	for (int i = 0; i < n; i++)
	{
		int cur_suff = sa[i];
		int prev_suff = cur_suff - 1;
		if (prev_suff >= 0 && suff_type[prev_suff] == L_TYPE)
			sa[l_ptr[str[prev_suff]]++] = prev_suff;
	}
}

void right_run(int n, int str[])
{
	find_backets_borders(n, str);
	for (int i = n - 1; i >= 0; i--)
	{
		int cur_suff = sa[i];
		int prev_suff = cur_suff - 1;
		if (prev_suff >= 0 && suff_type[prev_suff] == S_TYPE)
			sa[r_ptr[str[prev_suff]]--] = prev_suff;
	}
}

void left_right_run(int n, int str[])
{
	left_run(n, str);
	right_run(n, str);
}

int is_lms_eq(int str[], int a, int b)
{
	bool first = true;
	if (a == b)
		return true;
	while (true)
	{
		bool a_is_lms = is_lms(a);
		bool b_is_lms = is_lms(b);

		if (!first && a_is_lms && b_is_lms)
			return true;
		if (a_is_lms != b_is_lms)
			return false;
		first = false;
		if (str[a] != str[b])
			return false;
		if (suff_type[a] != suff_type[b])
			return false;
		a++;
		b++;
	}
	assert(false);
}

void build_sa(int n, int str[])
{
	if (n == 0)
		exit(0);

	if (n == 1)
	{
		sa[0] = 0;
		return;
	}

	calc_suff_types(n, str);
	find_backets_borders(n, str);
	
	bool can_finish = true;
	for (int i = 0; i < n; i++)
		if (l_ptr[i] != r_ptr[i])
			can_finish = false;
	
	fill(sa, sa + n, -1);
	for (int i = 0; i < n; i++)
		if (is_lms(i))
			sa[r_ptr[str[i]]--] = i;

	left_right_run(n, str);
	
	if (can_finish)
		return;

	int new_n = 0;
	for (int i = 0; i < n; i++)
		if (is_lms(i))
			new_n++;
	int* new_str = new_arr(new_n);
	int* lms_list = new_arr(new_n);

	new_n = 0;
	for (int i = 0; i < n; i++)
		if (is_lms(sa[i]))
			lms_list[new_n++] = sa[i];
	copy(lms_list, lms_list + new_n, new_str);
	
	int ptr = 0;
	for (int i = 1; i < new_n; i++)
	{
		int new_ptr = ptr;
		if (!is_lms_eq(str, new_str[i - 1], new_str[i]))
			new_ptr++;
		new_str[i - 1] = ptr;
		ptr = new_ptr;
	}
	new_str[new_n - 1] = ptr;

	for (int i = 0; i < new_n; i++)
		new_letter[lms_list[i]] = new_str[i];
	new_n = 0;
	for (int i = 0; i < n; i++)
	{
		if (is_lms(i))
		{
			lms_list[new_n] = i;
			new_str[new_n++] = new_letter[i];
		}
	}

	build_sa(new_n, new_str);
	copy(sa, sa + new_n, new_sa);

	calc_suff_types(n, str);
	find_backets_borders(n, str);
	fill(sa, sa + n, -1);

	for (int i = new_n - 1; i >= 0; i--)
	{
		int s = lms_list[new_sa[i]];
		sa[r_ptr[str[s]]--] = s;
	}

	left_right_run(n, str);
}

void init_and_run(int n)
{
	//add dollar
	init_str[n++] = 0;
	
	build_sa(n, init_str);

	//remove dollar
	for (int i = 0; i + 1 < n; i++)
		sa[i] = sa[i + 1];
	n--;
}

int main()
{
#ifdef LOCAL
	freopen ("input.txt", "r", stdin);
#endif

	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &init_str[i]);
		assert(1 <= init_str[i] && init_str[i] <= n);
	}
	
	init_and_run(n);

	for (int i = 0; i < n; i++)
		printf("%d ", sa[i]);
	printf("\n");

	return 0;
}
