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

//Функция, вводящая с клавиатуры набор целых чисел, и выводящая числа из этого набора в порядке возрастания и без повторений.
void func_1()
{
	int x;
	set <int> s;
	while (cin >> x) //считываем эл-ты с клавиатуры
	{
		s.insert(x);
	}

	for (int x : s)
		cout << x << " ";
	cout << endl;
	//Благодаря тому, что множества хранятся в упорядоченном виде, все элементы будут выведены в порядке возрастания значений.
}

//Функция, вводящая с клавиатуры набор целых чисел, и выводящая числа из этого набора, которые при вводе встречались не меньше двух раз.
void func_2()
{
	set<int> s;
	set<int> s1;
	int x;
	while (cin >> x)
	{
		if (s.count(x) == 1) s1.insert(x); //если x уже есть в s, то записываем его в s1
		s.insert(x); //заполняем s (неповторяющимися эл-ми)
	}
	for (int x : s1) //выводим эл-ты s1
		cout << x << " ";
	cout << endl;
}

//Функция, вводящая с клавиатуры набор целых чисел, и выводящую числа из этого набора, которые при вводе встречались ровно два раза.
void func_3()
{
	int x;
	set<int> s;
	set<int> s2;
	set<int> s3;

	while (cin >> x)
	{
		if (s2.count(x)) s3.insert(x); //s3 заполняется, когда эл-т встречается третий раз
		if (s.count(x)) s2.insert(x); //s2 заполняется, когда эл-т встречается второй раз
		s.insert(x); 
	}
	set <int>::iterator i;
	for (i = s2.begin(); i != s2.end(); ++i) //пробегаем s2 и выводим содержащиеся там эл-ты
	{
		if (s2.count(*i) && !s3.count(*i))
		{
			cout << *i << " ";
		}
	}
	cout << endl;


	for (int x : s2) //выводим эл-ты s2, который заполняется если эл-т входил больше одного раза
	{
		if(s3.count(x)==0) //не выводим эл-ты которые встречались (три раза) более двух раз
		cout << x << " ";
	}
	cout << endl;
}


//Шаблонная функция, принимающую два множества и возвращающую их объединение.
template <typename T>
set <T> uni (set <T> v, set <T> w) {
	set <T> u; // создаем новое множество set u
	for (auto x : v) // перебираем все эл-ты множ-ва v и записываем их в u
		u.insert(x);
	for (auto x : w) // перебираем все эл-ты множ-ва w и записываем их в u
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
