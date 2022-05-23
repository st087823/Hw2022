#include <iostream>
#include <math.h>
#include <future>
#include <chrono>
#include <vector>
using namespace std;
using namespace std::chrono;

//vector <double> v(8, 0);
const double N = 10000000;
const double n = N + 1;
vector <double> v(n,0);

// ��� ������ n, ��� ���� �������� ������������� ���������� ���������.
double f(double x) {
	return (1 / (1 + x * x));
}

double piCons(int n)
{
	double sum = 0;
	for (int i = 1; i <= n; ++i)
	{
		sum += f(((2 * i - 1.0) / (2 * n)));
	}
	return (4 * sum) / n;
}

double block_size(int threads) // (����� ������� � ��������)
{
	return n / threads; //������� ��������� ������� ������
}

void Pi(double bl_size, double x, int i)
// i - ����� ������, ������� ���������� ���� ���������� �������
// x - ����� ������, ���������� � ������ ������� �� ������ ������ ��������
// ����� n �� t=thread (����� �������) ������ � ������ ����� ������ ������

{
	double y = min(x + bl_size, n); // ���� x + bl_size>n �� �� ����� �� ������� n
	for (double p = x; p < y; p++) //�� x �� x + bl_size �������
	{
		v[i] += f(((2 * p - 1) / (2 * n)));
	}
	v[i] = 4.0 * v[i] / n;
	//���������� ����� � ������, ������ ��� ������ ����� ������������ �� ����� � ���� � �� �� ���������� ���-�� ����������, ������� ��� ����� k ������ ����������
}

double multi_t_Pi(int t)
{
	vector<future <void>> fut(t);
	double bl_size = block_size(t); //������ ������ ������ - ������� ��������� ������� ������

	double a = 0; //����� ������, ���������� � ������ ������� �� ������ ������ ��������
	int i = 0; // ����� ������, � �������� ��������
	while (a < n) //���� �� ������� a �� ��� ��� (bl_size) �� �����
	{
		fut[i] = async(Pi, bl_size, a, i); //���������� � fut[i] ��������� ���������� �������, ������� ����� ������ ��� ���������
		// ������� async ��������� � �������� ���������� ������ ������� pi, ������� ��� ��������� �����������, � ����� ��������� ���� �������.
		a += bl_size;
		i++;
	}

	for (int j = 0; j < i; j++)
	{
		fut[j].get(); // ���������� ����������� ������������ ����������
	}
	/*double pi = 0;
	for (auto x : v) pi += x;*/
	double p = 0;
	for (int j = 0; j < i; ++j)
	{
		p += fut[j];
	}

	return  p * 4 / n;
}

int main()
{
	int t = 4;
	auto start = steady_clock::now();

	cout << multi_t_Pi(t) << endl;

	auto finish = steady_clock::now();
	cout << duration_cast <milliseconds> (finish - start).count() << " ms";
	return 0;
}