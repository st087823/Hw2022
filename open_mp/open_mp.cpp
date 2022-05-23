#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <cmath>
#include <vector>
using namespace std;

double pi = acos(-1); //задаем значение pi

double* sinC = new double[10000000]; // глобальное объясление массива
double* sinP = new double[10000000];
void sinCons(int n)
{
	double t = omp_get_wtime();
	for (int i = 0; i <= n; ++i) //при i=0 pi*i/(2n)=0   i=n: pi*i/(2n)= pi/2
	{
		sinC[i] = sin(pi * i / (2 * n));
	}
	cout << "Time sinCons: " << omp_get_wtime() - t << endl;
}

// parallel for параллелизация может автоматически подстраиваться под нужное число параллельных ветвей программы.
// #pragma omp parallel for schedule (тип, размер)
void sinPar(int n)
{
	double t = omp_get_wtime();
#pragma omp parallel for schedule(static, 5)
	for (int i = 0; i <= n; ++i)
	{
		sinC[i] = sin(pi * i / (2.0 * n));
	}
	cout << "Time stat: " << omp_get_wtime() - t << endl;

	double t1 = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 5)
	for (int i = 0; i <= n; ++i)
	{
		sinC[i] = sin(pi * i / (2.0 * n));
	}
	cout << "Time dinam: " << omp_get_wtime() - t1 << endl;

	double t2 = omp_get_wtime();
#pragma omp parallel for schedule(guided, 5)
	for (int i = 0; i <= n; ++i)
	{
		sinC[i] = sin(pi * i / (2.0 * n));
	}
	cout << "Time guided: " << omp_get_wtime() - t2 << endl;

	double t3 = omp_get_wtime();
	for (int i = 0; i <= n; ++i)
	{
		sinC[i] = sin(pi * i / (2.0 * n));
	}
	cout << "Time posl: " << omp_get_wtime() - t3 << endl;
}




double piCons(int n)
{
	double t = omp_get_wtime();
	double sum = 0;
	for (int i = 1; i <= n; ++i)
	{
		sum += func(((2 * i - 1.0) / (2 * n)));
	}
	cout << "Time cons: " << omp_get_wtime() - t << endl;
	return (4 * sum) / n;
}

double func(double x)
{
	return 1 / (1 + x * x);
}

double piPar(int n)
{
	double sum = 0;
	double t = omp_get_wtime();
#pragma omp parallel for schedule(static, 5) reduction(+: sum)
	for (int i = 1; i <= n; ++i)
	{
		sum += func((double(2) * i - 1) / (double(2) * n));
	}
	cout << "Time stat: " << omp_get_wtime() - t << endl;

	sum = 0;
	double t1 = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 5) reduction(+: sum)
	for (int i = 1; i <= n; ++i)
	{
		sum += func((double(2) * i - 1) / (double(2) * n));
	}
	cout << "Time dynam: " << omp_get_wtime() - t1 << endl;

	sum = 0;
	double t2 = omp_get_wtime();
#pragma omp parallel for schedule(guided, 5) reduction(+: sum)
	for (int i = 1; i <= n; ++i)
	{
		sum += func((double(2) * i - 1) / (double(2) * n));
	}
	cout << "Time guided: " << omp_get_wtime() - t2 << endl;

	sum = 0;
	double t3 = omp_get_wtime();
	for (int i = 1; i <= n; ++i)
	{
		sum += func((double(2) * i - 1) / (double(2) * n));
	}
	cout << "Time posl: " << omp_get_wtime() - t3 << endl;

	return sum * 4 / n;
}




bool prime(int n)
{
	if (n == 1) return false;
	if (n % 2 == 0 && n != 2)
		return false;
	for (int i = 3; i * i <= n; i += 2) // перебираем все нечетные числа от 3 до √n.
		if (n % i == 0)
			return false;
	return true;
}

int primePar(int n)
{
	int count = 0;
	double t = omp_get_wtime();
#pragma omp parallel for schedule(static, 5) reduction(+:count)
	for (int i = 1; i <= n; i += 2)
	{
		prime(i) ? count += 1 : 0;
	}
	cout << "Time stat: " << omp_get_wtime() - t << endl;


	count = 0;
	double t1 = omp_get_wtime();
#pragma omp parallel for schedule(dynamic, 5) reduction(+:count)
	for (int i = 1; i <= n; i += 2)
	{
		prime(i) ? count += 1 : 0;
	}
	cout << "Time dynam: " << omp_get_wtime() - t1 << endl;


	count = 0;
	double t2 = omp_get_wtime();
#pragma omp parallel for schedule(guided, 5) reduction(+:count)
	for (int i = 1; i <= n; i += 2)
	{
		prime(i) ? count += 1 : 0;
	}
	cout << "Time guided: " << omp_get_wtime() - t2 << endl;

	count = 0;
	double t3 = omp_get_wtime();
	for (int i = 1; i <= n; i += 2)
	{
		prime(i) ? count += 1 : 0;
	}
	cout << "Time cons: " << omp_get_wtime() - t3 << endl;
	return count;
}





bool indexValid(int x, int  y, int n, int k)
{
	return ((x >= 0 && x < n) && (y >= 0 && y < k));
}
int neightbros(int** field, int x, int y, int n, int k)
{
	int sum = 0;
	for (int i = x - 1; i <= x + 1; ++i)
	{
		for (int j = y - 1; j <= y + 1; ++j)
		{
			if (!(i == x && j == y) && indexValid(i, j, n, k))
			{
				sum += field[i][j];
			}
		}
	}
	return sum;
}
bool is_alive(int now, int neightbros)
{
	if (now == 1)
	{
		return neightbros < 2 ? false : neightbros > 3 ? false : true;
	}
	else
	{
		return neightbros == 3 ? true : false;
	}
}
void print(int** field, int n, int k)
{
	for (int j = 0; j < k; ++j)
	{
		for (int i = 0; i < n; ++i)
		{
			cout << field[i][j];
		}
		cout << endl;
	}
}


int main()
{
	/*int n = 0;
	cin >> n;
	sinCons(n);
	sinPar(n);
	cout << piCons(n) << endl;
	cout << piPar(n) << endl;
	cout << numberOfPrimes(n) << endl;*/
	//sin_func(100000000);
	//cout << pi_arctg(100000000) << endl;
	//cout << prime_count(1000000) << endl;
	int** arr = new int* [10000];
	for (int i = 0; i < 10000; ++i)
	{
		arr[i] = new int[10000]{ 0 };
	}
	delete[] sinC;
	for (int i = 0; i < 10000; ++i)
	{
		delete[] arr[i];
	}
	delete[]arr;
	return 0;
}