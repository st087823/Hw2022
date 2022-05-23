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


//��������� � �������� ���������� ����� ������� � ������ �������, � ��������� �� �������� �� ��������� �������,
//�������� �������� �������� �� 10 �������� ���������.

double integr(double (*f)(double), double A, double B)
{
    double sum[2]{ 0 };
    // int n = 1000000;
    int n = 10;
    double h = (B - A) / n; // ����� ������� ���������, ���� �� ������

#pragma omp parallel
    {
#pragma omp single //������ ���� ����� ��������� ������; � ��������� ������. ������� ������� ����, ���������� single �� �����������. �����: �������������
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

    } // �.�. � ������ ����� �������� � i=1 f(A)�� ���������� � �� ������ �� ��������-�� i=n f(B) ���� �� �������� � ����� �����, �������� ���:
    return (sum[0] + sum[1] + f(A) + f(B)) * h / 2;

    // return (sum[0] + sum[1]) * h + h * (f(A) + f(B)) / 2;
}


//������� integral, ������� ��������� � �������� ���������� ����� ������� � ������ �������, � 
// ��������� �� �������� �� ��������� �������,

double integral_posl(double (*f)(double), double A, double B)
{
    // ���������� ���������� ����������
    double inter_full = integr(f, A, B); //  ������� ����������� �������� �� ������������ ��� �������
    // ����� ����������� �������� �� ���������� ������ �������,
    double inter_left_half = integr(f, A, (A + B) / 2);
    double inter_right_half = integr(f, (A + B) / 2, B);
    //������� ������� ����� ���������� �� ���� ������� � ������ ���������� �� ���������� �������
    double val = inter_full - (inter_left_half + inter_right_half);

    // ���� value ���������� �� �����, ��� �� 1e-10, ����� ��������� ���������� � ������������ inter_full
    if (-1e-10 < val && val < 1e-10)
    {
        return inter_full; // ����� ������� �� ��������, ��������������� ��������� �������� ������� � �������
    }
    // ���� ����� �� ������, �� ���������� �������� ���-��� integral_posl ��� ��������� � ��� ��� �������� ������ ��� ������ �� ���, �������� �����������
    // ������������ �� ���������, �����  ������ val ����� ������ ��� ������� ������������ ��������
    return integral_posl(f, A, (A + B) / 2) + integral_posl(f, (A + B) / 2, B);
}

//��� ������������ �������� ������ ��� ������� ���������� is_external
double integral_par(double (*f)(double), double A, double B, bool is_external = true)
{
    double inter_full = integr(f, A, B);
    double inter_left_half = integr(f, A, (A + B) / 2);
    double inter_right_half = integr(f, (A + B) / 2, B);
    double val = inter_full - (inter_left_half + inter_right_half);

    if (-1e-10 < val && val < 1e-10)
    {
        return inter_full; // ����� ������� �� ��������, ��������������� ��������� �������� ������� � �������
    }
    double a = 0;
    double b = 0;

    // ������������� ����������� ����� integral_par, ���� value �� �������
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
    // false ��� ����, ����� �� ������� � ��� ����� ��� ������ ��������, � �������� ����, �������� � ������� ���������� ������� integral_posl a � b
    else
    {
        a = integral_posl(f, A, (A + B) / 2);
        b = integral_posl(f, (A + B) / 2, B);
    }
    return a + b;
}

// ������� partition ����� ��������� ������� ��� ������������������ ������� �
// ���������� ��� ������������� ������� � ��������������� �������
int partition(int* a, int l, int r)
{
    int i = l - 1, j = r; // l � r � ����� � ������ �������
    int v = a[r]; // ������� ������� ������
    while (true)
    {
        while (a[++i] < v); // ������� i ������ ���� ����������� a[i] < a[r]
        //���� ��������������� ������� ������ �������� �������
        while (v < a[--j]) // ������� j ����� ���� ����������� a[r] < a[j] (���� ������� ��-� ������ ������ ����������� � ���� �������
        {
            if (j == l) // �������� ������ �� ������� �������
            {
                break;
            }
        }
        if (i >= j)
        {
            break;
        }
        swap(a[i], a[j]); // ������������ �������� a[i] � a[j]
    }
    swap(a[i], a[r]); // ������������ �������� a[i] � a[r]
    return i;
}

void qsort(int* a, int l, int r)
{
    if (l >= r)
    {
        return;
    }
    int i = partition(a, l, r);
    qsort(a, l, i - 1); // ��������� ����� ����� �������
    qsort(a, i + 1, r); // ��������� ������ ����� �������
}

void qsort_par(int* a, int l, int r)
{
    if (l >= r)
    {
        return;
    }
    int i = partition(a, l, r);

    // ������������� ����������� �����
#pragma omp parallel
    {
#pragma omp single //������ ���� ����� ��������� ������; � ��������� ������. ������� ������� ����, ���������� single �� �����������. �����: �������������
        {
#pragma omp task
            qsort(a, l, i - 1);
#pragma omp task
            qsort(a, i + 1, r);
#pragma omp taskwait  //�����, ������������ ������ �������� ���������� ���� ���������� �� ����� � ������ �����
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