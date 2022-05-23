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

void sinPar(int n)
{
	double t = omp_get_wtime();
#pragma omp parallel sections
	{
#pragma omp section
		{
			for (int i = 0; i < n / 2; i++)  // i <= n/2
			{
				sinC[i] = sin(pi * i / (2 * n));
			}
		}
#pragma omp section
		{
			for (int i = n / 2; i <= n; i++)  // i = n/2 +1
			{
				sinC[i] = sin(pi * i / (2 * n));
			}
		}
	}
	cout << "Time par: " << omp_get_wtime() - t << endl;
}


double func(double x) {
	return (1 / (1 + x * x));
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

double piPar(int n)
{
	double t = omp_get_wtime();
	double sum = 0;
#pragma omp parallel sections reduction(+:sum) //переменная sum явл-ся частной для параллельных ветвей т.е. каждая ветвь работает со своей копией  sum
	{
#pragma omp section
		{
			for (int i = 0; i < n / 2; i++)
				sum += func((2 * i - 1.0) / (2 * n));		
		}
#pragma omp section
		{
			for (int i = n / 2; i <= n; i++)
				sum += func((2 * i - 1.0) / (2 * n));
		}
	}
	cout << "Sum: " << (4 * sum) / n << endl;
	cout << "Time: " << omp_get_wtime() - t << endl;
	return 0;
}

// проверка является ли число простым.
bool isPrime(int n)
{
	if (n == 1) return false;
	if (n % 2 == 0 && n != 2)
		return false;
	for (int i = 3; i * i <= n; i += 2) // перебираем все нечетные числа от 3 до √n.
		if (n % i == 0)
			return false;
	return true;
}

int prime_count_cons(int n)
{
	double t = omp_get_wtime();
	int count = 0;
	for (int i = 1; i <= n; ++i)
	{
		count += isPrime(i);
	}
	cout << "Time cons: " << omp_get_wtime() - t << endl;
	return count;
}

// Вычисление кол-ва простых чисел в интервале от 1 до n.
int numberOfPrimes(int n) {
	double t = omp_get_wtime();
	double quan = 0;

#pragma omp parallel sections reduction(+:sum) //
	{
#pragma omp section //рассматриваем только нечет числа, раскиданные на два потока. четные сразу выкидываем из рассматрения
		{
			for (int i = 1; i <= n; i += 4)
				(isPrime(i) ? quan += 1 : 0); //добавляем к общем количеству 1, если это простое число и 0, если это составное число
		}
#pragma omp section
		{
			for (int i = 3; i <= n; i += 4)
				(isPrime(i) ? quan += 1 : 0);
		}
	}
	cout << "Quantity: " << quan << endl;
	cout << "Time: " << omp_get_wtime() - t << endl;
	return 0;
}

//Для задания координат точки используем структуру Point, а набор точек будем хранить в переменной типа vector <Point>.
// функциz dist, вычисляющую расстояние между двумя точками.
struct Point
{
	double x;
	double y;
	double z;

	Point(double x = 0, double y = 0, double z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};
double dist(Point p1, Point p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
}


void fill_points(vector<Point>& v)
{
	double x = 0;
	double y = 0;
	double z = 0;
	while (cin >> x)
	{
		cin >> y >> z;
		v.push_back(Point(x, y, z));
	}
}

// последовательная функция, вычисляющую (диаметр) наибольшее расстояние между двумя точками из указанного набора.
//функция принимает на вход массив координат точек в трехмерном пространстве

double diamCons(vector<Point> v)
{
	double maxd = 0;
	double temp = 0;
	double t = omp_get_wtime();
	for (auto i = v.begin(); i != v.end(); ++i)
	{
		for (auto j = i; j != v.end(); ++j)
		{
			temp = dist(*i, *j);
			if (temp > maxd)
			{
				maxd = temp;
			}
		}
	}
	cout << "Time cons: " << omp_get_wtime() - t << endl;
	return sqrt(maxd); // т.к. в функции считаем квадрат расстояния
}

double diam2(vector<Point> v)
{
	double temp = 0;
	double maxd1 = 0;
	double maxd2 = 0;
	vector<Point>::iterator k = v.begin();
	k += v.size() / 2;
	double t = omp_get_wtime();
#pragma omp parallel sections
	{
#pragma omp section
		{
			for (auto i = v.begin(); i != k; i++)
			{
				for (auto j = i; j != v.end(); j++)
				{
					temp = dist(*i, *j);
					if (temp > maxd1)
					{
						maxd1 = temp;
					}
				}
			}
		}
#pragma omp section
		{
			for (auto i = k; i != v.end(); i++)
			{
				for (auto j = i; j != v.end(); j++)
				{
					temp = dist(*i, *j);
					if (temp > maxd2)
					{
						maxd2 = temp;
					}
				}
			}
		}
	}
	cout << "Time par: " << omp_get_wtime() - t << endl;
	if (maxd1 > maxd2)
	{
		return maxd1;
	}
	return maxd2;
}

double diam3(vector<Point> v)
{
	double t = omp_get_wtime();
	double maxd1 = 0;
	double maxd2 = 0;
	double maxd3 = 0;
	double temp = 0;
	vector<Point>::iterator k = v.begin();
	k += v.size() / 2;

#pragma omp sections
	{
#pragma omp section
		{
			for (auto i = v.begin(); i != k; ++i)
			{
				for (auto j = i; j != v.end(); ++j)
				{
					temp = dist(*i, *j);
					if (temp > maxd1)
					{
						maxd1 = temp;
					}
				}
			}
		}
#pragma omp section
		{
			for (auto i = k; i != v.end(); ++i)
			{
				for (auto j = i; j != v.end(); ++j)
				{
					temp = dist(*i, *j);
					if (temp > maxd2)
					{
						maxd2 = temp;
					}
				}
			}
		}
#pragma omp section
		{
			for (auto i = k; i != v.end(); ++i)
			{
				for (auto j = v.begin(); j != k; ++j)
				{
					temp = dist(*i, *j);
					if (temp > maxd3)
					{
						maxd3 = temp;
					}
				}
			}
		}
	}
	cout << "Time par: " << omp_get_wtime() - t << endl;
	if (maxd1 > maxd2)
	{
		if (maxd1 > maxd3)
		{
			return maxd1;
		}
	}
	else if (maxd2 > maxd3)
	{
		return maxd2;
	}
	return maxd3;
}

double diam4(vector<Point> v)
{
	double t = omp_get_wtime();
	double maxd1 = 0;
	double maxd2 = 0;
	double maxd3 = 0;
	double maxd4 = 0;
	double temp = 0;
	vector<Point>::iterator k = v.begin();
	k += v.size() / 2;

#pragma omp sections
	{
#pragma omp section
		{
			for (auto i = v.begin(); i != k; ++i)
			{
				for (auto j = i; j != k; ++j)
				{
					temp = dist(*i, *j);
					if (temp > maxd1)
					{
						maxd1 = temp;
					}
				}
			}
		}
#pragma omp section
		{
			for (auto i = v.begin(); i != k; ++i)
			{
				for (auto j = k; j != v.end(); ++j)
				{
					temp = dist(*i, *j);
					if (temp > maxd2)
					{
						maxd2 = temp;
					}
				}
			}
		}
#pragma omp section
		{
			for (auto i = k; i != v.end(); ++i)
			{
				for (auto j = i; j != v.end(); ++j)
				{
					temp = dist(*i, *j);
					if (temp > maxd3)
					{
						maxd3 = temp;
					}
				}
			}
		}
#pragma omp section
		{
			for (auto i = k; i != v.end(); ++i)
			{
				for (auto j = v.begin(); j != k; ++j)
				{
					temp = dist(*i, *j);
					if (temp > maxd4)
					{
						maxd4 = temp;
					}
				}
			}
		}
	}
	cout << "Time par: " << omp_get_wtime() - t << endl;

	maxd1 = (maxd1 > maxd2) ? maxd1 : maxd2;
	maxd1 = (maxd1 > maxd3) ? maxd1 : maxd3;
	return (maxd1 > maxd4) ? maxd1 : maxd4;
}

int main()
{
	int n = 0;
	cin >> n;
	sinCons(n);
	sinPar(n);
	cout << piCons(n) << endl;
	cout << piPar(n) << endl;
	cout << numberOfPrimes(n) << endl;

	vector<Point> v;
	fill_points(v);
	cout << diamCons(v) << endl;
	cout << diam2(v) << endl;
	cout << diam3(v) << endl;
	cout << diam4(v) << endl;

	return EXIT_SUCCESS;
}