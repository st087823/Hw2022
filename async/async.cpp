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

// Чем больше n, тем выше точность приближенного вычисления интеграла.
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

double block_size(int threads) // (число потоков – параметр)
{
	return n / threads; //сколько досдается каждому потоку
}

void Pi(double bl_size, double x, int i)
// i - номер потока, который записывает свою переменную вектора
// x - идекс строки, запоминает с какого индекса мы отдаем потоку работать
// делим n на t=thread (число потоков) частей и каждую часть отдаем потоку

{
	double y = min(x + bl_size, n); // если x + bl_size>n то мы вышли за границы n
	for (double p = x; p < y; p++) //от x до x + bl_size считаем
	{
		v[i] += f(((2 * p - 1) / (2 * n)));
	}
	v[i] = 4.0 * v[i] / n;
	//записываем сумму в вектор, потому что каждый поток одновременно не может в одну и ту же переменную что-то записывать, поэтому нам нужно k разных переменных
}

double multi_t_Pi(int t)
{
	vector<future <void>> fut(t);
	double bl_size = block_size(t); //размер одного потока - сколько досдается каждому потоку

	double a = 0; //идекс строки, запоминает с какого индекса мы отдаем потоку работать
	int i = 0; // номер потока, с которого начинаем
	while (a < n) //пока не сдвинем a на наш шаг (bl_size) до конца
	{
		fut[i] = async(Pi, bl_size, a, i); //записываем в fut[i] результат нескольких потоков, поэтому нужен вектор для обработки
		// Функция async принимает в качестве параметров другую функцию pi, которую она запускает параллельно, а также параметры этой функции.
		a += bl_size;
		i++;
	}

	for (int j = 0; j < i; j++)
	{
		fut[j].get(); // дожидаемся результатов параллельных вычислений
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