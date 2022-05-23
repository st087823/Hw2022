#include <iostream>
#include <omp.h>
#include<cmath>
#include<list>
#include<cstdlib>

using namespace std;

double f(double x)
{
    return 1 / (1 + x * x);
}


//принимает в качестве параметров концы отрезка и другую функцию, и вычисляет ее интеграл на указанном отрезке,
//ользуясь формулой трапеций по 10 отрезкам разбиения.

double integr(double (*f)(double), double A, double B)
{
    double sum[2]{ 0 };
    // int n = 1000000;
    int n = 10;
    double h = (B - A) / n; // длина отрезка разбиения, если их десять

#pragma omp parallel
    {
#pragma omp single //только один поток порождает задачи; в остальных паралл. потоках участок кода, выделенный single не выполняется. функц: синхронизация
        {
#pragma omp task
            {
                for (int i = 1; i < n / 2; ++i)
                {
                    sum[0] += f(A + h * i);
                }
            }
#pragma omp task
            {
                for (int i = n / 2; i < n; ++i)
                {
                    sum[1] += f(A + h * i);
                }
            }
        }

    } // т.к. в первом цикле начинаем с i=1 f(A)не учитыватся и во втором не рассматр-ся i=n f(B) тоже не добавили в общую сумму, исправим это:
    return (sum[0] + sum[1] + f(A) + f(B)) * h / 2;

    // return (sum[0] + sum[1]) * h + h * (f(A) + f(B)) / 2;
}


//функцию integral, которая принимает в качестве параметров концы отрезка и другую функцию, и 
// вычисляет ее интеграл на указанном отрезке,

double integral_posl(double (*f)(double), double A, double B)
{
    // алгоритмом адаптивной квадратуры
    double inter_full = integr(f, A, B); //  сначала вычисляется интеграл на интересующем нас отрезке
    // далее вычисляется интеграл на половинках нашего отрезка,
    double inter_left_half = integr(f, A, (A + B) / 2);
    double inter_right_half = integr(f, (A + B) / 2, B);
    //считаем разницу между интегралом на всем отрезке и суммой интегралов на половинках отрезка
    double val = inter_full - (inter_left_half + inter_right_half);

    // если value отличается не более, чем на 1e-10, ответ считается правильным и возвращается inter_full
    if (-1e-10 < val && val < 1e-10)
    {
        return inter_full; // здесь выходим из рекурсии, последовательно возвращая значения обратно в фуекцию
    }
    // если ответ не верный, то рекурсивно вызываем фун-цию integral_posl для половинок и еще раз проводим анализ для каждой из них, оценивая погрешность
    // возвращаемся из реккурсии, когда  оценка val будет верной для каждого разветвления рекурсии
    return integral_posl(f, A, (A + B) / 2) + integral_posl(f, (A + B) / 2, B);
}

//для параллельной ффункции введем еще булевую переменную is_external
double integral_par(double (*f)(double), double A, double B, bool is_external = true)
{
    double inter_full = integr(f, A, B);
    double inter_left_half = integr(f, A, (A + B) / 2);
    double inter_right_half = integr(f, (A + B) / 2, B);
    double val = inter_full - (inter_left_half + inter_right_half);

    if (-1e-10 < val && val < 1e-10)
    {
        return inter_full; // здесь выходим из рекурсии, последовательно возвращая значения обратно в фуекцию
    }
    double a = 0;
    double b = 0;

    // распараллелим рекурсивный вызов integral_par, если value не подошло
    if (is_external)
    {
#pragma omp parallel
        {
#pragma omp single
            {
#pragma omp task
                a = integral_par(f, A, (A + B) / 2, false);

#pragma omp task
                b = integral_par(f, (A + B) / 2, B, false);
            }
        }
    }
    // false для того, чтобы не входить в эту ветку при вызове рекурсии, а попадать сюда, вычисляя с помощью последоват функции integral_posl a и b
    else
    {
        a = integral_posl(f, A, (A + B) / 2);
        b = integral_posl(f, (A + B) / 2, B);
    }
    return a + b;
}

// функция partition берет последний элемент еще неотсортированного массива и
// определяет его окончательную позицию в отсортированном массиве
int partition(int* a, int l, int r)
{
    int i = l - 1, j = r; // l и r – левая и правая границы
    int v = a[r]; // крайний элемент справа
    while (true)
    {
        while (a[++i] < v); // двигаем i вправо пока выполняется a[i] < a[r]
        //пока рассматриваемый элемент меньше крайнего правого
        while (v < a[--j]) // двигаем j влево пока выполняется a[r] < a[j] (пока крайний эл-т справа меньше сдвигаемого в лево правого
        {
            if (j == l) // проверка выхода за пределы массива
            {
                break;
            }
        }
        if (i >= j)
        {
            break;
        }
        swap(a[i], a[j]); // переставляем значения a[i] и a[j]
    }
    swap(a[i], a[r]); // переставляем значения a[i] и a[r]
    return i;
}

void qsort(int* a, int l, int r)
{
    if (l >= r)
    {
        return;
    }
    int i = partition(a, l, r);
    qsort(a, l, i - 1); // сортируем левую часть массива
    qsort(a, i + 1, r); // сортируем правую часть массива
}

void qsort_par(int* a, int l, int r)
{
    if (l >= r)
    {
        return;
    }
    int i = partition(a, l, r);

    // распараллелим рекурсивный вызов
#pragma omp parallel
    {
#pragma omp single //только один поток порождает задачи; в остальных паралл. потоках участок кода, выделенный single не выполняется. функц: синхронизация
        {
#pragma omp task
            qsort(a, l, i - 1);
#pragma omp task
            qsort(a, i + 1, r);
#pragma omp taskwait  //поток, генерирующий задачи дождется завершения двух запущенных им задач и только тогда
        }
    }
}


int main()
{
    double t = omp_get_wtime();
    cout << 4*integral_posl(f, 0, 1) << endl;
    cout << "Time posl:" << omp_get_wtime() - t <<endl;
    t = omp_get_wtime();
    cout << 4*integral_par(f, 0, 1) << endl;
    cout << "Time par:" << omp_get_wtime() - t <<endl;
    int* arr = new int[50000000]{0};
    t = omp_get_wtime();
    qsort(arr, 0, 50000000);
    cout << "qsort Time posl:" << omp_get_wtime() - t <<endl;
    t = omp_get_wtime();
    qsort_par(arr, 0, 50000000);
    cout << "qsort Time par:" << omp_get_wtime() - t <<endl;

    system("Pause");


    return 0;
}