/*#include <iostream>
#include <string>
#include <vector>
#include <future>
#include <chrono>
using namespace std;
using namespace std::chrono;
const int n = 16;
int answer[8];
int t = 8;

int block_size(int t)
{
	return n / t;
}

//Find, которая ищет число вхождений первой строки (what) во вторую (s), 
//причем индекс первого символа вхождения искомой строки what лежит в интервале[a, b).
int find(string what, string s, int a, int b)
{
	int ans = 0;
	int tmp = -1;
	for (int i = a; i < b; i++)
	{
		if (s.substr(i, what.length()) == what) ans++;
	}
	return ans;
}

void find_t(string what, string s, int i, int f_index)
{
	//auto start = steady_clock::now();
	int bl = block_size(t);
	answer[i] = find(what, s, f_index, f_index + bl);
	//auto finish = steady_clock::now();
}

int multi_t_find(int t, string what, string s)
{
	int bl_size = block_size(t);
	vector < future < void>> fut(t);
	int first_index = 0;
	int i = 0;
	while (first_index < n)
	{
		fut[i] = async(find_t, what, s, i, first_index);
		first_index += bl_size;
		i++;
	}
	for (int j = 0; j < i; j++)
	{
		fut[j].get();
	}
	int ans = 0;
	for (auto x : answer) ans += x;
	return ans;
}
int main()
{
	int t = 8;
	auto start = steady_clock::now();
	string s = "albbbaabbcccaadd";
	string what = "a";
	cout « "answer = " « multi_t_find(t, what, s) « endl;
	//multi_t_find(t, what, s);
	auto finish = steady_clock::now();
	cout « duration_cast <milliseconds> (finish - start).count() << " ms";
	return 0;
}*/