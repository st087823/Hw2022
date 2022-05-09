#include <iostream>
#include <map>
using namespace std;

template <class T1, class T2>
void print_map(map<T1, T2> m)
{
	if (m.size() == 0) //map пустой
	{
		cout << "empty\n";
	}
	for (auto x : m)
	{
		cout << x.first « "->" << x.second « '\n';
	}
}


/*функцию, вводящую с клавиатуры набор целых чисел и выводящую числа из этого набора в порядке возрастания и без повторений, с указанием того, сколько каждое число раз встретилось в этом наборе.*/
void f1()
{
	map<int, int> m; //map всегда дает набор упорядоченных ключей
	int x;
	while (cin >> x)
	{
		if (m.count(x)) m[x]++;  // если x уже есть, то увеличиваем на 1 его кол-во m[ключ] = данные;
		else m[x] = 1; //если x встретился первый раз, то его кол-во 1.
	}
	print_map(m);
}

/* функцию, вводящую с клавиатуры отображение из целых чисел в целые числа (вводятся число пар и пары из двух целых чисел, 
первое из которых − ключ, а второе − данные), и возвращающую это отображение как результат типа map<int, int>. */
map<int, int> f2()
{
	map<int, int> m;
	int n = 0;
	cin >> n; // n - число пар
	for (int i = 0; i < n; i++)
	{
		int key = 0;
		cin >> key;
		int data = 0;
		cin >> data;
		m[key] = data;
	}
	return m;
}


/* функцию, вводящую отображение (с помощью f2), и затем набор чисел, и выводящее для каждого введенного числа его образ при введенном отображении, 
если введенное отображение определено на этом числе, и само введенное число без изменений, если не определено. */
void f3()
{
	map<int, int> m = f2();
	int x = 0;
	while (cin >> x)
	{
		if (m.count(x))
		{
			cout << m[x] << " ";
		}
		else
		{
			cout << x << " ";
		}
	}
}

template <class T1, class T2>
map <T1, T2> f4(map<T1, T2>& m1, map<T1, T2>& m2)
{
	map<T1, T2> m3;
	for (auto x : m1)
	{
		if (m2.count(x.first) == 1)
		{
			m3[x.first] = x.second + m2[x.first];
		}
	}
	return m3;
}

template <class T1, class T2, class T3>
map <T1, T3> f5(map<T1, T2>& m1, map<T2, T3>& m2)
{
	map<T1, T3> m3;
	for (auto x : m1)
	{
		if (m2.count(x.second) == 1)
		{
			m3[x.first] = m2[x.second];
		}
	}
	return m3;
}

int main()
{
	//f1();
	//print_map(f2());
	//f3();
	map<string, int> m1 = { {"Father", 65}, {"Mother", 62},
	{"Sister", 20}, {"Brother", 15} };
	map<string, int> m2 = { {"Father", 45}, {"Mother", 42},
	{"Sister", 19} };
	//print_map(f4(m1, m2));
	map<int, string> map1 = { {1, "aa"}, {2, "bb"}, {3, "cc"}, {4, "dd"} };
	map<string, int> map2 = { {"aa", 2}, {"bb", 3}, {"cc", 4}, {"dd", 6} };
	print_map(f5(map1, map2));
	return EXIT_SUCCESS;;
}