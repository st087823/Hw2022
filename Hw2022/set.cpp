/*#include<iostream>
#include<set>

using namespace std;

void function_1()
{
	int x;
	set <int> s;
	while (cin >> x)
	{
		s.insert(x);
	}
	set <int>::iterator i;
	for (i = s.begin(); i != s.end(); ++i)
		cout << *i << " ";
	cout << endl;
}

void function_2()
{
	set<int> s;
	set<int> s1;
	int x;
	while (cin >> x)
	{
		if (s.count(x)) s1.insert(x);
		s.insert(x);
	}
	set <int>::iterator i;
	for (i = s1.begin(); i != s1.end(); ++i)
		cout << *i << " ";
	cout << endl;
}

void function_3()
{
	int x;
	set<int> s2;
	set<int> s;
	set<int> s1;
	while (cin >> x)
	{
		if (s1.count(x)) s2.insert(x);
		if (s.count(x)) s1.insert(x);
		s.insert(x);
	}
	set <int>::iterator i;
	for (i = s1.begin(); i != s1.end(); ++i)
	{
		if (s1.count(*i) && !s2.count(*i))
		{
			cout << *i << " ";
		}
	}

	cout << endl;
}

template <typename T>
set <T> function_4(set <T> v, set <T> w) {
	set <T> result;
	for (int x : v)
	{
		result.insert(x);
	}
	for (int x : w)
	{
		result.insert(x);
	}
	return result;
}


int main()
{
	set <int> s;
	//function_1();
	//function_2();
	//function_3();
	set<int> s1;
	set<int> s2;
	int x;
	int k = 0;

	while (cin >> x)
	{
		if (k % 2 == 1)
		{
			s1.insert(x);
		}
		else
		{
			s2.insert(x);
		}
		++k;
	}

	s = function_4(s1, s2);
	set <int>::iterator i;
	for (i = s.begin(); i != s.end(); ++i)
		cout << *i << " ";
	cout << endl;

	return EXIT_SUCCESS;

}*/





#include<iostream>
#include"set"

using namespace std;

//�������, �������� � ���������� ����� ����� �����, � ��������� ����� �� ����� ������ � ������� ����������� � ��� ����������.
void func_1()
{
	int x;
	set <int> s;
	while (cin >> x) //��������� ��-�� � ����������
	{
		s.insert(x);
	}

	for (int x : s)
		cout << x << " ";
	cout << endl;
	//��������� ����, ��� ��������� �������� � ������������� ����, ��� �������� ����� �������� � ������� ����������� ��������.
}

//�������, �������� � ���������� ����� ����� �����, � ��������� ����� �� ����� ������, ������� ��� ����� ����������� �� ������ ���� ���.
void func_2()
{
	set<int> s;
	set<int> s1;
	int x;
	while (cin >> x)
	{
		if (s.count(x) == 1) s1.insert(x); //���� x ��� ���� � s, �� ���������� ��� � s1
		s.insert(x); //��������� s (���������������� ��-��)
	}
	for (int x : s1) //������� ��-�� s1
		cout << x << " ";
	cout << endl;
}

//�������, �������� � ���������� ����� ����� �����, � ��������� ����� �� ����� ������, ������� ��� ����� ����������� ����� ��� ����.
void func_3()
{
	int x;
	set<int> s;
	set<int> s2;
	set<int> s3;

	while (cin >> x)
	{
		if (s2.count(x)) s3.insert(x); //s3 �����������, ����� ��-� ����������� ������ ���
		if (s.count(x)) s2.insert(x); //s2 �����������, ����� ��-� ����������� ������ ���
		s.insert(x); 
	}
	set <int>::iterator i;
	for (i = s2.begin(); i != s2.end(); ++i) //��������� s2 � ������� ������������ ��� ��-��
	{
		if (s2.count(*i) && !s3.count(*i))
		{
			cout << *i << " ";
		}
	}
	cout << endl;


	for (int x : s2) //������� ��-�� s2, ������� ����������� ���� ��-� ������ ������ ������ ����
	{
		if(s3.count(x)==0) //�� ������� ��-�� ������� ����������� (��� ����) ����� ���� ���
		cout << x << " ";
	}
	cout << endl;
}


//��������� �������, ����������� ��� ��������� � ������������ �� �����������.
template <typename T>
set <T> uni (set <T> v, set <T> w) {
	set <T> u; // ������� ����� ��������� set u
	for (auto x : v) // ���������� ��� ��-�� ����-�� v � ���������� �� � u
		u.insert(x);
	for (auto x : w) // ���������� ��� ��-�� ����-�� w � ���������� �� � u
		u.insert(x);
	return u;
}

int main() {

	set <int> s;
	set <int> v;
	set <int> w;
	int a = 0;
	while (cin >> a) {
		if (s.count(a) == 0) {
			s.insert(a);
		}
		else {
			if (v.count(a) == 0) {
				v.insert(a);
			}
			else {
				w.insert(a);
			}
		}
	}

	for (auto x : v)
		if (w.count(x) == 0) {
			cout << x << " ";
		}
	return 0;
}
