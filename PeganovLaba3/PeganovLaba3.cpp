#include <iostream>
#include <fstream> 
#include <string>
#include <chrono> 
#define INITIAL_PERFORMANCE 2.95;
using namespace std;
using namespace std::chrono;

// Структура узла, содержит конструктор по умолчанию
struct Node
{
	int key;
	Node* left;
	Node* right;
	// Конструктор по умолчанию, создает лист по заданному ключу
	Node(int k) { key = k; left = right = NULL; }
};

// Принимает корень дерева root и ключ удаляемого узла k
// Функция удаляет из данного дерева узел по заданному ключу
// Возвращает корень преобразованного дерева
Node* delNodeFunc(Node* root, int k)
{
	// Шаг 1 общего алгоритма
	Node* delParent = NULL,
		* del = root;

	while (1)
	{
		if (!del)
			return root;

		if (del->key == k)
			break;

		if (del->key > k)
		{
			delParent = del;
			del = del->left;
		}
		else
		{
			delParent = del;
			del = del->right;
		}
	}
	// Конец шага 1 общего алгоритма

	// Шаг 2 общего алгоритма
	Node* delNext;
	delNext = del->right;

	if (!delNext)
	{
		if (!delParent)
		{
			root = del->left;
			return root;
		}

		if (delParent->left == del)
		{
			delParent->left = del->left;
			return root;
		}
		else
		{
			delParent->right = del->left;
			return root;
		}
	}
	// Конец шага 2 общего алгоритма

	// Шаг 3 общего алгоритма
	if (!delNext->left)
	{
		delNext->left = del->left;

		if (!delParent)
		{
			root = delNext;
			return root;
		}

		if (delParent->left == del)
		{
			delParent->left = delNext;
			return root;
		}
		else
		{
			delParent->right = delNext;
			return root;
		}
	}
	// Конец шага 3 общего алгоритма

	// Шаг 4 общего алгоритма
	Node* pParent = delNext,
		* p = pParent->left;

	while (p->left)
	{
		pParent = p;
		p = p->left;
	}

	p->left = del->left;
	pParent->left = p->right;
	p->right = del->right;

	if (!delParent)
	{
		root = p;
		return root;
	}

	if (delParent->left == del)
	{
		delParent->left = p;
		return root;
	}
	else
	{
		delParent->right = p;
		return root;
	}
	// Конец шага 4 общего алгоритма
}

// Принимает количество узлов дерева
// Выводит в консоль прогноз выполнения функции удаления для минимального
//		среднего и максимального времен выполнения
void predictFunc(int num)
{
	double c1_min = 0.0016,	// Коэффициент временной функции для минимального вреиени
		c1_avr,	c2_avr,		// ... для среднего времени
		c1_max,	c2_max,		// ... для максимального времени
		perfFactor = 0;		// Множитель проивзодительности

	// Нахождение множителя производительности
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (int i = 0; i < 1000000000; i++)
		perfFactor++;
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	perfFactor = duration_cast<duration<double>>(t2 - t1).count() / INITIAL_PERFORMANCE;

	// Определение коэффициентов для среднего времени в зависимости от диапазона
	if (num < 200)
	{
		c1_avr = 0.00009403;
		c2_avr = 0.00144279;
	}
	else
	{
		if (num < 10000)
		{
			c1_avr = 0.00014805;
			c2_avr = 0.00102381;
		}
		else
		{
			c1_avr = 0.00018385;
			c2_avr = 0.0005766;
		}
	}

	// Определение коэффициентов для максимального времени в зависимости от диапазона
	if (num < 4000)
	{
		c1_max = 0.00002131;
		c2_max = 0.00227238;
	}
	else
	{
		if (num < 20000)
		{
			c1_max = 0.00001578;
			c2_max = 0.00145535;
		}
		else
		{
			c1_max = 0.00001517;
			c2_max = 0.04562794;
		}
	}

	cout << "\nМинимальное время: " << c1_min * perfFactor << " секунд(ы)";
	cout << "\nСреднее время: " << (c1_avr * log2(num) + c2_avr) * perfFactor << " секунд(ы)";
	cout << "\nМаксимальное время: " << (c1_max * num + c2_max) * perfFactor << " секунд(ы)\n";
	return;
}

// Принимает корень дерева root и ключ добавляемого узла newKey
// Добавляет по заданному ключу новый узел в дерево, сохраняя структуру бинарного дерева поиска
// Возвращает корень преобразованного дерева (cсли добавляемый узел уже есть в дереве, то возвращает NULL)
Node* addNode(Node* root, int newKey)
{
	Node* p = root;
	if (!root)
		return new Node(newKey);

	while (1)
	{
		if (newKey < p->key)
		{
			if (p->left)
				p = p->left;
			else
			{
				p->left = new Node(newKey);
				return root;
			}
		}
		else
		{
			if (newKey > p->key)
			{
				if (p->right)
					p = p->right;
				else
				{
					p->right = new Node(newKey);
					return root;
				}
			}
			else
				return NULL;
		}
	}
}

// Принимает на вход указатель на дерево и "0"
// Вычисляет максимальную глубину дерева
int treeLvlNum(Node* root, int lvlCount)
{
	if (root)
		lvlCount++;
	else
		return lvlCount;

	if (treeLvlNum(root->left, lvlCount) > treeLvlNum(root->right, lvlCount))
		return treeLvlNum(root->left, lvlCount);
	else
		return treeLvlNum(root->right, lvlCount);
}

// Принимает корень дерева
// Печатает необходимое дерево в консоль
void treePrint(Node* root, const int count = 0)
{
	if (!root)
		return;
	if (root->right)
		treePrint(root->right, count + 1);

	for (int i = 0; i < count; i++)
		cout << "	";
	cout << "(" << root->key << ")" << endl;

	if (root->left)
		treePrint(root->left, count + 1);
	
	return;
}

// Принимает корень дерева и границы для среза
// Печатает срез дерева в консоль
void customTreePrint(Node* root, int flvl, int slvl, const int count = 0)
{
	if (!root)
		return;
	if (root->right)
		customTreePrint(root->right, flvl, slvl, count + 1);

	if ((count + 1) >= flvl && (count + 1) <= slvl)
	{
		for (int i = flvl; i < count; i++)
			cout << "	";
		cout << "(" << root->key << ")" << endl;
	}

	if (root->left)
		customTreePrint(root->right, flvl, slvl, count + 1);

	return;
}

// Принимает корень дерева и поток файла
// Печатает дерево в нужный файл
void treeSaveToFile(Node* root, ofstream& str, const int count = 0)
{
	if (!root)
		return;
	if (root->right)
		treeSaveToFile(root->right, str, count + 1);

	for (int i = 0; i < count; i++)
		str << "	";
	str << "(" << to_string(root->key) << ")" << endl;

	if (root->left)
		treeSaveToFile(root->left, str, count + 1);
		
	return;
}

// Принимает корень дерева, поток файла и границы среза
// Печатает дерево в нужный файл
void customTreeSaveToFile(Node* root, ofstream& str, int flvl, int slvl, const int count = 0)
{
	if (!root)
		return;
	if (root->right)
		customTreeSaveToFile(root->right, str, flvl, slvl, count + 1);

	if ((count + 1) >= flvl && (count + 1) <= slvl)
	{
		for (int i = flvl; i < count; i++)
			str << "	";
		str << "(" << to_string(root->key) << ")" << endl;
	}

	if (root->left)
		customTreeSaveToFile(root->left, str, flvl, slvl, count + 1);

	return;
}

// Принимает на вход корень дерева, диапазон ключей и маркеры, определяющие "ключевые" узлы
// Строит на основании полученных данных сбалансированное дерево с ключами в заданном диапазоне
//		и выводит шкалу прогресса на экран
// Возвращает корень построенного дерева
Node* createBalancedTree(Node* root, int start, int end, int p25, int p50, int p75)
{
	int dif = end - start;
	int mid = (end + start) / 2;

	switch (dif)
	{
	case 0:
		if(start == p25)
			cout << "\nСоздание дерева завершено на 25%";
		if (start == p50)
			cout << "\nСоздание дерева завершено на 50%";
		if (start == p75)
			cout << "\nСоздание дерева завершено на 75%";
		root = addNode(root, start);
		break;
	case 1:
		if (start == p25 || end == p25)
			cout << "\nСоздание дерева завершено на 25%";
		if (start == p50 || end == p50)
			cout << "\nСоздание дерева завершено на 50%";
		if (start == p75 || end == p75)
			cout << "\nСоздание дерева завершено на 75%";
		root = addNode(root, start);
		root = addNode(root, end);
		break;
	default:
		root = addNode(root, mid);
		root = createBalancedTree(root, start, mid - 1, p25, p50, p75);
		root = createBalancedTree(root, mid + 1, end, p25, p50, p75);
		break;
	}
	return root;
}

// Принимает переменную, определяющую способ ввода дерева и количество узлов в дереве
// По данной переменной конструирует определенным способом бинарное дерево поиска
// Возвращает корень созданного дерева
Node* createTree(int num, int inputType)
{
	Node* root = NULL,		// Корень создаваемого дерева
		* temp = NULL;		// Временнаая переменная
	ifstream inp;			// Нужна для считывания из файла
	int buf = 0,			// Нужна для считывания с консоли/генератора чисел
		p25 = num / 4,		// Метка для отображения 25% прогресса создания дерева
		p50 = num / 2,		// ... 50% прогресса
		p75 = num * 3 / 4;	// ... 75% прогресса

	switch (inputType)
	{
	case 1:	// Ввод с клавиатуры
		cout << "\nВведите ключи собираемого дерева:\n";
		for (int i = 0; i < num; i++)
		{
			cin >> buf;
			temp = addNode(root, buf);
			if (temp)
				root = temp;
			else
			{
				cout << "В дереве уже есть узел с введенным ключом <" << buf << ">. Введите другой ключ.\n";
				i--;
			}
		}
		break;
	case 2:	// Ввод из файла
		inp.open("input.txt");
		if (inp.is_open())
		{
			for (int i = 1; i <= num; i++)
			{
				inp >> buf;
				root = addNode(root, buf);
				if (!root)
				{
					cout << "\nФайл содержит неверно построенное дерево. Завершение работы программы.\n";
					inp.close();
					exit(0);
				}

				if(i == p25)
					cout << "\nСоздание дерева завершено на 25%";
				else
				{
					if(i == p50)
						cout << "\nСоздание дерева завершено на 50%";
					else
					{
						if(i == p75)
							cout << "\nСоздание дерева завершено на 75%";
					}
				}
			}
			cout << "\nСоздание дерева завершено на 100%\n";
			inp.close();
		}
		else
		{
			cout << "\nОшибка при открытии файла. Завершение работы программы.\n";
			exit(0);
		}
		break;
	case 3:	// Ввод методом генерации ключей
		for (int i = 1; i <= num; i++)
		{
			temp = addNode(root, rand() + 1);
			if (temp)
			{
				root = temp;
				if (i == p25)
					cout << "\nСоздание дерева завершено на 25%";
				else
				{
					if (i == p50)
						cout << "\nСоздание дерева завершено на 50%";
					else
					{
						if (i == p75)
							cout << "\nСоздание дерева завершено на 75%";
					}
				}
			}
			else
				i--;
		}
		cout << "\nСоздание дерева завершено на 100%\n";
		break;
	case 4:	// Построение вырожденного дерева
		for (int i = num; i > 0; i--)
		{
			if (i == p25)
				cout << "\nСоздание дерева завершено на 75%";
			else
			{
				if (i == p50)
					cout << "\nСоздание дерева завершено на 50%";
				else
				{
					if (i == p75)
						cout << "\nСоздание дерева завершено на 25%";
				}
			}
			root = addNode(root, i);
		}
		cout << "\nСоздание дерева завершено на 100%\n";
		break;
	case 5:	// Построение сбалансированного дерева
		p50 = (num + 1) / 2 - 1;
		p25 = (num + 1) / 4 - 1;
		p75 = ((num + 1) / 2 + num) / 2 - 1;
		root = createBalancedTree(root, 1, num, p25, p50, p75);
		cout << "\nСоздание дерева завершено на 100%\n";
		break;
	default:
		cout << "\nВы ввели неверные данные. Завершение работы.\n";
		exit(0);
	}
	return root;
}

// Процедура осуществляет общение с пользователем
void menu()
{
	Node* root = NULL;		// Исходное дерево
	ofstream out;			// Для записи в файл результата
	int k = 0,				// Ключ удаляемого узла
		num = 0,			// Количество узлов дерева
		isRepeat = 1,		// Переключатель для цикла с повторением удаления / вывода
		flvl,				// Метка для верхнего уровня среза
		slvl,				// Метка для нижнего уровня среза
		ioputType = 0;		// Способ вывода преобразованного дерева

	// Вывоб способа ввода дерева
	cout << "Укажите способ ввода исходных данных\n"
		<< "1 - ввод с клавиатуры, 2 - выбор из файла, 3 - сгенерировать данные случайно, 4 - вырожденное, 5 - сбалансированное: ";
	cin >> ioputType;

	cout << "\nВведите количество узлов в дереве: ";
	cin >> num;
	if (num < 1)
	{
		cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
		exit(0);
	}

	// Получение дерева согласно введенным инструкциям
	root = createTree(num, ioputType);

	cout << "\nВывести прогноз времени выполнения функции удаления?\n0 - не выводить, 1 - вывести: ";
	cin >> ioputType;
	switch (ioputType)
	{
	case 0:	// Без прогноза
		break;
	case 1:	// Прогноз
		predictFunc(num);
		break;
	default:
		cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
		exit(0);
	}

	// Вывод полученного дерева
	cout << "\nКуда вывести полученное дерево?\n0 - не выводить, 1 - в консоль, 2 - в файл, 3 - частично в консоль, 4 - частично в файл: ";
	cin >> ioputType;
	switch (ioputType)
	{
	case 0:	// Без вывода
		break;
	case 1:	// В консоль
		cout << "\nВведенное дерево:\n";
		treePrint(root);
		break;
	case 2:	// В файл
		out.open("savein.txt");
		if (out.is_open())
			treeSaveToFile(root, out);
		else
		{
			cout << "\nОшибка при открытии файла. Завершение работы программы.\n";
			out.close();
			exit(0);
		}
		out.close();
		cout << "\nВведенное дерево было записано в файл savein.txt.\n";
		break;
	case 3:	// Частично в консоль
		num = treeLvlNum(root, 0);
		cout << "\nДерево состоит из " << num << " уровней, выберите срез дерева для отображения.\nОтобразить с уровня: ";
		cin >> flvl;
		cout << "По уровень: ";
		cin >> slvl;
		if (flvl > slvl || flvl < 1 || slvl > num)
		{
			cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
			exit(0);
		}
		cout << "\nВведенное дерево:\n";
		customTreePrint(root, flvl, slvl);
		break;
	case 4:	// Частично в файл
		out.open("savein.txt");
		if (out.is_open())
		{
			num = treeLvlNum(root, 0);	// Теперь num отображает количество уровней дерева
			cout << "\nДерево состоит из " << num << " уровней, выберите срез дерева для отображения.\nОтобразить с уровня: ";
			cin >> flvl;
			cout << "По уровень: ";
			cin >> slvl;
			if (flvl > slvl || flvl < 1 || slvl > num)
			{
				cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
				exit(0);
			}
			customTreeSaveToFile(root, out, flvl, slvl);
		}
		else
		{
			cout << "\nОшибка при открытии файла. Завершение работы программы.\n";
			out.close();
			exit(0);
		}
		out.close();
		cout << "\nВведенное дерево выборочно записано в файл savein.txt.\n";
		break;
	default:
		cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
		exit(0);
	}

	while (isRepeat) 
	{
		// Ввод ключа удаляемого узла
		cout << "\nВведите ключ удаляемого узла: ";
		cin >> k;
		if (!k)
		{
			cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
			exit(0);
		}

		// Удаление узла из дерева по его ключу и вычисление времени работы функции
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		root = delNodeFunc(root, k);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		cout << "\nВремя выполнения функции составило : " << duration_cast<duration<double>>((t2 - t1) * 1000).count() << " секунд(ы).\n";

		// Выбор места вывода преобразованного дерева
		cout << "\nВыберите, куда выводить преобразованные данные\n0 - не выводить, 1 - в консоль, 2 - в файл, 3 - частично в консоль, 4 - частично в файл: ";
		cin >> ioputType;
		switch (ioputType)
		{
		case 0:	// Без вывода
			break;
		case 1:	// В консоль
			cout << "\nПреобразованное дерево:\n";
			treePrint(root);
			break;
		case 2:	// В файл
			out.open("output.txt");
			if (out.is_open())
				treeSaveToFile(root, out);
			else
			{
				cout << "\nОшибка при открытии файла. Завершение работы программы.\n";
				out.close();
				exit(0);
			}
			out.close();
			cout << "\nПреобразованное дерево записано в файл output.txt.";
			break;
		case 3:	// Частично в консоль
			num = treeLvlNum(root, 0);
			cout << "\nДерево состоит из " << num << " уровней, выберите срез дерева для отображения.\nОтобразить с уровня: ";
			cin >> flvl;
			cout << "По уровень: ";
			cin >> slvl;
			if (flvl > slvl || flvl < 1 || slvl > num)
			{
				cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
				exit(0);
			}
			cout << "\nВведенное дерево:\n";
			customTreePrint(root, flvl, slvl);
			break;
		case 4:	// Частично в файл
			out.open("output.txt");
			if (out.is_open())
			{
				num = treeLvlNum(root, 0);
				cout << "\nДерево состоит из " << num << " уровней, выберите срез дерева для отображения.\nОтобразить с уровня: ";
				cin >> flvl;
				cout << "По уровень: ";
				cin >> slvl;
				if (flvl > slvl || flvl < 1 || slvl > num)
				{
					cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
					exit(0);
				}
				customTreeSaveToFile(root, out, flvl, slvl);
			}
			else
			{
				cout << "\nОшибка при открытии файла. Завершение работы программы.\n";
				out.close();
				exit(0);
			}
			out.close();
			cout << "\nВведенное дерево выборочно записано в файл output.txt.\n";
			break;
		default:
			cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
			exit(0);
		}

		cout << "\nЖелаете продолжить работу с используемым деревом?" << "\n0 - нет, 1 - да: ";
		cin >> isRepeat;
		if (isRepeat < 0 && isRepeat > 1)
		{
			cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
			exit(0);
		}
	}
	return;
}

int main()
{
	int count = 1;
	setlocale(LC_ALL, "ru");

	while (count)
	{
		menu();
		cout << "\nЖелаете продолжить работу с программой?\n0 - нет, 1 - да: ";
		cin >> count;
		cout << endl;
		if (count > 1 || count < 0)
		{
			cout << "\nВы ввели неверное значение. Завершение работы программы.\n";
			return 0;
		}
	}
}