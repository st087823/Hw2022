#include<iostream>
#include<omp.h>
#include<cmath>

using namespace std;

void print(double** arr, int lines, int columns)
{
	for (int i = 0; i < lines; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

// divide, принимающую в качестве параметров двумерный массив(матрицу), номер строчки и вещественное число(коэффициент).
// Функция должна разделить все элементы соответствующей строчки на соответствующий коэффициент.

void divide(double** arr, int line_index, int line_length, double coef) // line_length - длина строчки
{
	for (int i = 0; i < line_length; ++i)
	{
		arr[line_index][i] /= coef;
	}
}


// subtract, принимающую в качестве параметров двумерный массив, два целых числа(номера строк), длина строчек и вещественное число(коэффициент)
// Функцию, вычитающая поэлементно в массиве-параметре строку с первым номером, умноженную на коэффициент, из строки со вторым номером.

void subtract(double** arr, int line_index1, int line_index2, int line_length, double coef)
{
	for (int i = 0; i < line_length; ++i)
	{
		// arr[line_index2][i] -= arr[line_index1][i] * coef; // вычитаем из второй строки первую, умноженную на коэф-т
		arr[line_index1][i] -= arr[line_index2][i] * coef;
	}
}



int column_max(double** arr, int column_index, int lines) // нахождение строки с наибольшим по модулю i-ым эл-ом (на первом шаге: первым эл-ом, далее вторым и тд)
{
	int max_index = column_index;
	for (int i = column_index; i < lines; ++i)
	{
		if (abs(arr[i][column_index]) > abs(arr[max_index][column_index]))
		{
			max_index = i;
		}
	}
	return max_index;
}
void swap(double* line_i, double* line_max) // меняем местами две строки
{
	double* temp = line_i;
	line_i = line_max;
	line_max = temp;
}


// transform, приводящую матрицу из вещественных чисел к единичному виду при помощи элементарных преобразований над строками
void transform_posl(double** arr, int lines, int columns) // lines - строчки, columns - столбцы
{
	for (int i = 0; i < columns - 1; ++i) // сдвигаем i по столбцам, на каждом шаге обращая в ноль каждый столбец и 
		// не обнуляем последний столбец, где нах-ся правые части уравнения в расширеннной матрице
	{
		// column_max(arr, i, lines) - На первом шаге мы выбираем строку расширенной матрицы системы с наибольшим по модулю iым (первым) эл-том 

		swap(arr[i], arr[column_max(arr, i, lines)]); // меняем ее местами с текущей i-ой строкой (на первом шаге: с первой строкой)
		divide(arr, i, columns, arr[i][i]); // после чего делим ее покоординатно на i-ый элемент (на первом шаге: на пер -вый элемент, новый 1ый эл-т = 1)


		// покоординатно вычитаем новую первую строку из каждой из остальных строк с таким коэффициентом, 
		// т.е. вычит. новую первую строку домноженную на такое число, чтобы в результате первые элементы всех остальных(кроме первой) строк оказались равны нулю
		for (int j = 0; j < lines; ++j) // проделываем со всеми строками
		{
			if (j != i) //для всех кроме той строчки, которую вычитаем из остальных с коэф-ом
			{
				subtract(arr, j, i, columns, arr[j][i]); // новый i-ый эл-т = 1,то коэф-т просто равен iму эл-ту строчки, которую мы преобразуем
			}
		}
		// первый элемент этого столбца равен единице, а все остальные – нулю.
		// Цель второго шага – получить аналогичный вид второго столбца; 
		// различие будет состоять в том, что единице будет равен второй элемент.
	}
}
// бывший столбец правых частей уравнений будет содержать искомые значения неизвестных.

// Распараллелить полученную программу для решения системы линейных уравнений
void transform_parl(double** arr, int lines, int columns)
{

	for (int i = 0; i < columns - 1; ++i)
	{
		swap(arr[i], arr[column_max(arr, i, lines)]);
		divide(arr, i, columns, arr[i][i]);
#pragma omp parallel for schedule(static, 5) // for schedule (тип, размер)
		//множетсво итераций делится на части указанного размера. номера частей, которые получает конкретный поток обр арифметич прогр.
		for (int j = 0; j < lines; ++j)
		{
			if (j != i)
			{
				subtract(arr, j, i, columns, arr[j][i]);
			}
		}
	}
}

int main()
{
	int lines = 0;
	int columns = 0;
	cout << "equations number" << endl;
	cin >> lines;
	columns = lines + 1;
	double** arr_posl = new double* [lines];
	double** arr_parl = new double* [lines];
	for (int i = 0; i < lines; ++i)
	{
		arr_posl[i] = new double[columns];
		arr_parl[i] = new double[columns];
	}
	cout << "cin >> matrix" << endl;
	for (int i = 0; i < lines; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			cin >> arr_posl[i][j];
			arr_parl[i][j] = arr_posl[i][j];
		}
	}
	cout << endl;
	transform_posl(arr_posl, lines, columns);
	double t = omp_get_wtime();
	transform_posl(arr_posl, lines, columns);
	cout << "Time posl:" << omp_get_wtime() - t << endl;
	print(arr_posl, lines, columns);
	t = omp_get_wtime();
	transform_posl(arr_parl, lines, columns);
	cout << "Time parl:" << omp_get_wtime() - t << endl;
	print(arr_parl, lines, columns);
	return 0;
}