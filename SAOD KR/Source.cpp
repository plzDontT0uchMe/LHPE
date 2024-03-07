#include<locale>
#include<iostream>
#include<vector>
#include<ctime>

using namespace std;


class field
{
public:
	vector<int> block;
	field* next = NULL;
};

class Hashing
{
public:
	int maxSize = 2; //5
	int startBlockCount = 2;
	int blockCount = 2; //2
	int pointerFirst = 0;
	int pointerSecond = startBlockCount / 2;
	double overflow = 0.75;
	int countEl = 0;
	Hashing()
	{
		init(blocks);
	}
	vector<field> blocks;
	void init(vector<field>& blocks)
	{
		blocks.resize(blockCount);
	}
	void add(int number, int indexBlock) 
	{
		if (blocks[indexBlock].block.size() == maxSize)
		{
			if (blocks[indexBlock].next == NULL)
			{
				field* newField = new field();
				newField->next = NULL;
				newField->block.push_back(number);
				countEl++;
				blocks[indexBlock].next = newField;
			}
			else
			{
				field* iter = blocks[indexBlock].next;
				while (iter->next != NULL && iter->block.size() == maxSize) //
				{
					iter = iter->next;
				}
				if (iter->block.size() != maxSize)
				{
					iter->block.push_back(number);
					countEl++;
				}
				else
				{
					field* newField = new field();
					newField->block.push_back(number);
					countEl++;
					iter->next = newField;
				}
			}
		}
		else
		{
			blocks[indexBlock].block.push_back(number);
			countEl++;
			//overflow
		}
	}
	bool search(int number, int indexBlock)
	{
		for (int i = 0; i < blocks[indexBlock].block.size(); i++)
		{
			if (number == blocks[indexBlock].block[i]) return true;
		}
		if (blocks[indexBlock].next == NULL) return false;
		field* iter = blocks[indexBlock].next;
		while (true)
		{
			for (int i = 0; i < (*iter).block.size(); i++)
			{
				if (number == (*iter).block[i]) return true;
			}
			if (iter->next == NULL) break;
			iter = iter->next;
		}
		return false;
	}
	int hashFuncFirst(int number)
	{
		return number % startBlockCount;
	}
	int hashFuncSecond(int number)
	{
		return number % blockCount;
	}
	void splitBlock(int pointer)
	{
		if (blocks[pointer].block.size() == 0)
		{
			return;
		}
		for (int i = 0; i < blocks[pointer].block.size(); i++)
		{
			int indexBlock = hashFuncFirst(blocks[pointer].block[i]);
			if (indexBlock <= pointer)
			{
				indexBlock = hashFuncSecond(blocks[pointer].block[i]);
				if (indexBlock > blockCount) indexBlock = hashFuncFirst(blocks[pointer].block[i]);
			}
			if (indexBlock == pointer) continue;
			int temp = blocks[pointer].block[i];
			blocks[pointer].block.erase(blocks[pointer].block.cbegin() + i, blocks[pointer].block.cbegin() + i + 1);
			countEl--;
			i--;
			add(temp, indexBlock);
		}
		field* iter = blocks[pointer].next;
		//cout << blocks[i].block.size();
		while (true)
		{
			if (iter == NULL) break;
			for (int i = 0; i < (*iter).block.size(); i++)
			{
				int indexBlock = hashFuncFirst((*iter).block[i]);
				if (indexBlock <= pointer)
				{
					indexBlock = hashFuncSecond((*iter).block[i]);
					if(indexBlock > blockCount) indexBlock = hashFuncFirst((*iter).block[i]);
				}
				if (indexBlock == pointer) continue;
				int temp = (*iter).block[i];
				(*iter).block.erase((*iter).block.cbegin() + i, (*iter).block.cbegin() + i + 1);
				countEl--;
				i--;
				add(temp, indexBlock);
			}
			iter = iter->next;
		}
	}
	void splitBlocks()
	{
		blockCount++;
		blocks.resize(blockCount);
		splitBlock(pointerFirst);
		splitBlock(pointerSecond);
		pointerFirst++;
		pointerSecond++;
		if (pointerFirst == startBlockCount)
		{
			startBlockCount = blockCount;
			pointerFirst = 0;
			pointerSecond = startBlockCount / 2;
		}
	}
	bool isOverFlow()
	{
		return ((double)countEl / (double)(blockCount * maxSize)) > overflow;
	}
	void printTable()
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			if (pointerFirst == i) cout << "->1 ";
			if (pointerSecond == i) cout << "->2 ";
			cout << i << ") ";
			field* iter = &blocks[i];
			while (true)
			{
				for (int j = 0; j < (*iter).block.size(); j++)
				{
					cout << (*iter).block[j] << " ";
					if (j == (*iter).block.size() - 1 && (*iter).next != NULL) cout << " ->  ";
				}
				if (iter->next == NULL) break;
				iter = iter->next;	
			}
			cout << endl;
		}
	}
	void fillSuccessively()
	{
		for (int i = 0; i < 20; i++)
		{
			int indexBlock = hashFuncFirst(i);
			add(i, indexBlock);
			if (isOverFlow())
			{
				splitBlocks();
			}
		}
	}
	void fillRandom()
	{
		for (int i = 0; i < 20; i++)
		{
			int num = rand() % 100;
			int indexBlock = hashFuncFirst(num);
			add(num, indexBlock);
			if (isOverFlow())
			{
				splitBlocks();
			}
		}
	}
	void fillFromKeyboard()
	{
		int num;
		cout << ">>> Введите элемент для вставки: ";
		cin >> num;
		int indexBlock = hashFuncFirst(num);
		add(num, indexBlock);
		if (isOverFlow())
		{
			splitBlocks();
		}
	}
	void clear()
	{
		blocks.clear();
		maxSize = 2; //5
		startBlockCount = 2;
		blockCount = 2; //2
		pointerFirst = 0;
		pointerSecond = startBlockCount / 2;
		overflow = 0.75;
		countEl = 0;
		init(blocks);
	}
};

int main()
{
	srand(time(NULL));
	setlocale(0, "");
	Hashing hashTable;

	bool quit = false;
	int choice;
	while (!quit)
	{
		cout << "Выберите что хотите сделать?\n 1 - Добавить элементы последовательно.\n 2 - Добавить элементы случайно.\n 3 - Добавить элемент вручную.\n 4 - Вывод хеш-таблицы на экран.\n 5 - Поиск элемента\n 6 - Очистка хеш-таблицы\n 0 - Выход.\n";
		cout << "Ваш выбор: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
		{
			hashTable.fillSuccessively();
			break;
		}
		case 2:
		{
			hashTable.fillRandom();
			break;
		}
		case 3:
		{
			hashTable.fillFromKeyboard();
			break;
		}
		case 4:
		{
			hashTable.printTable();
			break;
		}
		case 5:
		{
			int num, indexBlock;
			bool search = false;
			cout << "Введите элемент для поиска: ";
			cin >> num;
			indexBlock = hashTable.hashFuncFirst(num);
			search = hashTable.search(num, indexBlock);
			if (!search)
			{
				indexBlock = hashTable.hashFuncSecond(num);
				search = hashTable.search(num, indexBlock);
				if (!search)
				{
					cout << "Элемент не найден!\n";
				}
				else
				{
					cout << "Элемент найден!\n";
				}
			}
			else
			{
				cout << "Элемент найден!\n";
			}
			break;
		}
		case 6:
		{
			hashTable.clear();
			break;
		}
		case 0:
		{
			quit = true;
			break;
		}
		default:
			cout << "Введено неверное действие.\n";
			break;
		}
	}
	
	return 1;
}