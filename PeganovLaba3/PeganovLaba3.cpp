#include <iostream>
#include <fstream> 
#include <string>
#include <chrono> 
#define INITIAL_PERFORMANCE 2.95;
using namespace std;
using namespace std::chrono;

// ��������� ����, �������� ����������� �� ���������
struct Node
{
	int key;
	Node* left;
	Node* right;
	// ����������� �� ���������, ������� ���� �� ��������� �����
	Node(int k) { key = k; left = right = NULL; }
};

// ��������� ������ ������ root � ���� ���������� ���� k
// ������� ������� �� ������� ������ ���� �� ��������� �����
// ���������� ������ ���������������� ������
Node* delNodeFunc(Node* root, int k)
{
	// ��� 1 ������ ���������
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
	// ����� ���� 1 ������ ���������

	// ��� 2 ������ ���������
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
	// ����� ���� 2 ������ ���������

	// ��� 3 ������ ���������
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
	// ����� ���� 3 ������ ���������

	// ��� 4 ������ ���������
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
	// ����� ���� 4 ������ ���������
}

// ��������� ���������� ����� ������
// ������� � ������� ������� ���������� ������� �������� ��� ������������
//		�������� � ������������� ������ ����������
void predictFunc(int num)
{
	double c1_min = 0.0016,	// ����������� ��������� ������� ��� ������������ �������
		c1_avr,	c2_avr,		// ... ��� �������� �������
		c1_max,	c2_max,		// ... ��� ������������� �������
		perfFactor = 0;		// ��������� ������������������

	// ���������� ��������� ������������������
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (int i = 0; i < 1000000000; i++)
		perfFactor++;
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	perfFactor = duration_cast<duration<double>>(t2 - t1).count() / INITIAL_PERFORMANCE;

	// ����������� ������������� ��� �������� ������� � ����������� �� ���������
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

	// ����������� ������������� ��� ������������� ������� � ����������� �� ���������
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

	cout << "\n����������� �����: " << c1_min * perfFactor << " ������(�)";
	cout << "\n������� �����: " << (c1_avr * log2(num) + c2_avr) * perfFactor << " ������(�)";
	cout << "\n������������ �����: " << (c1_max * num + c2_max) * perfFactor << " ������(�)\n";
	return;
}

// ��������� ������ ������ root � ���� ������������ ���� newKey
// ��������� �� ��������� ����� ����� ���� � ������, �������� ��������� ��������� ������ ������
// ���������� ������ ���������������� ������ (c��� ����������� ���� ��� ���� � ������, �� ���������� NULL)
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

// ��������� �� ���� ��������� �� ������ � "0"
// ��������� ������������ ������� ������
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

// ��������� ������ ������
// �������� ����������� ������ � �������
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

// ��������� ������ ������ � ������� ��� �����
// �������� ���� ������ � �������
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

// ��������� ������ ������ � ����� �����
// �������� ������ � ������ ����
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

// ��������� ������ ������, ����� ����� � ������� �����
// �������� ������ � ������ ����
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

// ��������� �� ���� ������ ������, �������� ������ � �������, ������������ "��������" ����
// ������ �� ��������� ���������� ������ ���������������� ������ � ������� � �������� ���������
//		� ������� ����� ��������� �� �����
// ���������� ������ ������������ ������
Node* createBalancedTree(Node* root, int start, int end, int p25, int p50, int p75)
{
	int dif = end - start;
	int mid = (end + start) / 2;

	switch (dif)
	{
	case 0:
		if(start == p25)
			cout << "\n�������� ������ ��������� �� 25%";
		if (start == p50)
			cout << "\n�������� ������ ��������� �� 50%";
		if (start == p75)
			cout << "\n�������� ������ ��������� �� 75%";
		root = addNode(root, start);
		break;
	case 1:
		if (start == p25 || end == p25)
			cout << "\n�������� ������ ��������� �� 25%";
		if (start == p50 || end == p50)
			cout << "\n�������� ������ ��������� �� 50%";
		if (start == p75 || end == p75)
			cout << "\n�������� ������ ��������� �� 75%";
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

// ��������� ����������, ������������ ������ ����� ������ � ���������� ����� � ������
// �� ������ ���������� ������������ ������������ �������� �������� ������ ������
// ���������� ������ ���������� ������
Node* createTree(int num, int inputType)
{
	Node* root = NULL,		// ������ ������������ ������
		* temp = NULL;		// ���������� ����������
	ifstream inp;			// ����� ��� ���������� �� �����
	int buf = 0,			// ����� ��� ���������� � �������/���������� �����
		p25 = num / 4,		// ����� ��� ����������� 25% ��������� �������� ������
		p50 = num / 2,		// ... 50% ���������
		p75 = num * 3 / 4;	// ... 75% ���������

	switch (inputType)
	{
	case 1:	// ���� � ����������
		cout << "\n������� ����� ����������� ������:\n";
		for (int i = 0; i < num; i++)
		{
			cin >> buf;
			temp = addNode(root, buf);
			if (temp)
				root = temp;
			else
			{
				cout << "� ������ ��� ���� ���� � ��������� ������ <" << buf << ">. ������� ������ ����.\n";
				i--;
			}
		}
		break;
	case 2:	// ���� �� �����
		inp.open("input.txt");
		if (inp.is_open())
		{
			for (int i = 1; i <= num; i++)
			{
				inp >> buf;
				root = addNode(root, buf);
				if (!root)
				{
					cout << "\n���� �������� ������� ����������� ������. ���������� ������ ���������.\n";
					inp.close();
					exit(0);
				}

				if(i == p25)
					cout << "\n�������� ������ ��������� �� 25%";
				else
				{
					if(i == p50)
						cout << "\n�������� ������ ��������� �� 50%";
					else
					{
						if(i == p75)
							cout << "\n�������� ������ ��������� �� 75%";
					}
				}
			}
			cout << "\n�������� ������ ��������� �� 100%\n";
			inp.close();
		}
		else
		{
			cout << "\n������ ��� �������� �����. ���������� ������ ���������.\n";
			exit(0);
		}
		break;
	case 3:	// ���� ������� ��������� ������
		for (int i = 1; i <= num; i++)
		{
			temp = addNode(root, rand() + 1);
			if (temp)
			{
				root = temp;
				if (i == p25)
					cout << "\n�������� ������ ��������� �� 25%";
				else
				{
					if (i == p50)
						cout << "\n�������� ������ ��������� �� 50%";
					else
					{
						if (i == p75)
							cout << "\n�������� ������ ��������� �� 75%";
					}
				}
			}
			else
				i--;
		}
		cout << "\n�������� ������ ��������� �� 100%\n";
		break;
	case 4:	// ���������� ������������ ������
		for (int i = num; i > 0; i--)
		{
			if (i == p25)
				cout << "\n�������� ������ ��������� �� 75%";
			else
			{
				if (i == p50)
					cout << "\n�������� ������ ��������� �� 50%";
				else
				{
					if (i == p75)
						cout << "\n�������� ������ ��������� �� 25%";
				}
			}
			root = addNode(root, i);
		}
		cout << "\n�������� ������ ��������� �� 100%\n";
		break;
	case 5:	// ���������� ����������������� ������
		p50 = (num + 1) / 2 - 1;
		p25 = (num + 1) / 4 - 1;
		p75 = ((num + 1) / 2 + num) / 2 - 1;
		root = createBalancedTree(root, 1, num, p25, p50, p75);
		cout << "\n�������� ������ ��������� �� 100%\n";
		break;
	default:
		cout << "\n�� ����� �������� ������. ���������� ������.\n";
		exit(0);
	}
	return root;
}

// ��������� ������������ ������� � �������������
void menu()
{
	Node* root = NULL;		// �������� ������
	ofstream out;			// ��� ������ � ���� ����������
	int k = 0,				// ���� ���������� ����
		num = 0,			// ���������� ����� ������
		isRepeat = 1,		// ������������� ��� ����� � ����������� �������� / ������
		flvl,				// ����� ��� �������� ������ �����
		slvl,				// ����� ��� ������� ������ �����
		ioputType = 0;		// ������ ������ ���������������� ������

	// ����� ������� ����� ������
	cout << "������� ������ ����� �������� ������\n"
		<< "1 - ���� � ����������, 2 - ����� �� �����, 3 - ������������� ������ ��������, 4 - �����������, 5 - ����������������: ";
	cin >> ioputType;

	cout << "\n������� ���������� ����� � ������: ";
	cin >> num;
	if (num < 1)
	{
		cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
		exit(0);
	}

	// ��������� ������ �������� ��������� �����������
	root = createTree(num, ioputType);

	cout << "\n������� ������� ������� ���������� ������� ��������?\n0 - �� ��������, 1 - �������: ";
	cin >> ioputType;
	switch (ioputType)
	{
	case 0:	// ��� ��������
		break;
	case 1:	// �������
		predictFunc(num);
		break;
	default:
		cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
		exit(0);
	}

	// ����� ����������� ������
	cout << "\n���� ������� ���������� ������?\n0 - �� ��������, 1 - � �������, 2 - � ����, 3 - �������� � �������, 4 - �������� � ����: ";
	cin >> ioputType;
	switch (ioputType)
	{
	case 0:	// ��� ������
		break;
	case 1:	// � �������
		cout << "\n��������� ������:\n";
		treePrint(root);
		break;
	case 2:	// � ����
		out.open("savein.txt");
		if (out.is_open())
			treeSaveToFile(root, out);
		else
		{
			cout << "\n������ ��� �������� �����. ���������� ������ ���������.\n";
			out.close();
			exit(0);
		}
		out.close();
		cout << "\n��������� ������ ���� �������� � ���� savein.txt.\n";
		break;
	case 3:	// �������� � �������
		num = treeLvlNum(root, 0);
		cout << "\n������ ������� �� " << num << " �������, �������� ���� ������ ��� �����������.\n���������� � ������: ";
		cin >> flvl;
		cout << "�� �������: ";
		cin >> slvl;
		if (flvl > slvl || flvl < 1 || slvl > num)
		{
			cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
			exit(0);
		}
		cout << "\n��������� ������:\n";
		customTreePrint(root, flvl, slvl);
		break;
	case 4:	// �������� � ����
		out.open("savein.txt");
		if (out.is_open())
		{
			num = treeLvlNum(root, 0);	// ������ num ���������� ���������� ������� ������
			cout << "\n������ ������� �� " << num << " �������, �������� ���� ������ ��� �����������.\n���������� � ������: ";
			cin >> flvl;
			cout << "�� �������: ";
			cin >> slvl;
			if (flvl > slvl || flvl < 1 || slvl > num)
			{
				cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
				exit(0);
			}
			customTreeSaveToFile(root, out, flvl, slvl);
		}
		else
		{
			cout << "\n������ ��� �������� �����. ���������� ������ ���������.\n";
			out.close();
			exit(0);
		}
		out.close();
		cout << "\n��������� ������ ��������� �������� � ���� savein.txt.\n";
		break;
	default:
		cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
		exit(0);
	}

	while (isRepeat) 
	{
		// ���� ����� ���������� ����
		cout << "\n������� ���� ���������� ����: ";
		cin >> k;
		if (!k)
		{
			cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
			exit(0);
		}

		// �������� ���� �� ������ �� ��� ����� � ���������� ������� ������ �������
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		root = delNodeFunc(root, k);
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		cout << "\n����� ���������� ������� ��������� : " << duration_cast<duration<double>>((t2 - t1) * 1000).count() << " ������(�).\n";

		// ����� ����� ������ ���������������� ������
		cout << "\n��������, ���� �������� ��������������� ������\n0 - �� ��������, 1 - � �������, 2 - � ����, 3 - �������� � �������, 4 - �������� � ����: ";
		cin >> ioputType;
		switch (ioputType)
		{
		case 0:	// ��� ������
			break;
		case 1:	// � �������
			cout << "\n��������������� ������:\n";
			treePrint(root);
			break;
		case 2:	// � ����
			out.open("output.txt");
			if (out.is_open())
				treeSaveToFile(root, out);
			else
			{
				cout << "\n������ ��� �������� �����. ���������� ������ ���������.\n";
				out.close();
				exit(0);
			}
			out.close();
			cout << "\n��������������� ������ �������� � ���� output.txt.";
			break;
		case 3:	// �������� � �������
			num = treeLvlNum(root, 0);
			cout << "\n������ ������� �� " << num << " �������, �������� ���� ������ ��� �����������.\n���������� � ������: ";
			cin >> flvl;
			cout << "�� �������: ";
			cin >> slvl;
			if (flvl > slvl || flvl < 1 || slvl > num)
			{
				cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
				exit(0);
			}
			cout << "\n��������� ������:\n";
			customTreePrint(root, flvl, slvl);
			break;
		case 4:	// �������� � ����
			out.open("output.txt");
			if (out.is_open())
			{
				num = treeLvlNum(root, 0);
				cout << "\n������ ������� �� " << num << " �������, �������� ���� ������ ��� �����������.\n���������� � ������: ";
				cin >> flvl;
				cout << "�� �������: ";
				cin >> slvl;
				if (flvl > slvl || flvl < 1 || slvl > num)
				{
					cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
					exit(0);
				}
				customTreeSaveToFile(root, out, flvl, slvl);
			}
			else
			{
				cout << "\n������ ��� �������� �����. ���������� ������ ���������.\n";
				out.close();
				exit(0);
			}
			out.close();
			cout << "\n��������� ������ ��������� �������� � ���� output.txt.\n";
			break;
		default:
			cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
			exit(0);
		}

		cout << "\n������� ���������� ������ � ������������ �������?" << "\n0 - ���, 1 - ��: ";
		cin >> isRepeat;
		if (isRepeat < 0 && isRepeat > 1)
		{
			cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
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
		cout << "\n������� ���������� ������ � ����������?\n0 - ���, 1 - ��: ";
		cin >> count;
		cout << endl;
		if (count > 1 || count < 0)
		{
			cout << "\n�� ����� �������� ��������. ���������� ������ ���������.\n";
			return 0;
		}
	}
}