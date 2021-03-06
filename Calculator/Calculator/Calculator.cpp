﻿#include <iostream>
#include <bitset>
#include <locale>
#include <locale.h>
#include <clocale>

using namespace std;

bool CheckAddress(char* ip_)
{
	int points = 0,		// количество точек
		numbers = 0;	// значение октета
	char* buff;			// буфер для одного октета
	buff = new char[3]{};
	for (int i = 0; ip_[i] != '\0'; i++)
	{	// для строки IP-адреса
		if (isdigit(ip_[i])) // если цифра
		{
			if (numbers > 3) return false;
			//если больше трех чисел в октете – ошибка
			buff[numbers++] = ip_[i];
			//скопировать в буфер
		}
		else
			if (ip_[i] == '.') // если точка
			{
				if (atoi(buff) > 255)
					// проверить диапазон октета
					return false;
				if (numbers == 0)
					//если числа нет - ошибка
					return false;
				numbers = 0;
				points++;
				delete[]buff;
				buff = new char[3]{};
			}
			else return false;
	}
	if (points != 3)
		// если количество точек в IP-адресе не 3 - ошибка
		return false;
	if (numbers == 0 || numbers > 3)
		return false;
	return true;
}

unsigned long CharToLong(char* ip_)
{
	unsigned long out = 0;//число для IP-адреса
	char* buff;
	buff = new char[3]{};
	//буфер для хранения одного октета
	for (int i = 0, j = 0, k = 0; ip_[i] != '\0'; i++, j++)
	{
		if (ip_[i] != '.') //если не точка
			buff[j] = ip_[i]; // записать символ в буфер
		if (ip_[i] == '.' || ip_[i + 1] == '\0')
		{
			// если следующий октет или последний
			out <<= 8; //сдвинуть число на 8 бит
			if (atoi(buff) > 255)
				return NULL;
			// еcли октет больше 255 – ошибка
			out += (unsigned long)atoi(buff);
			//преобразовать и добавить
			//к числу IP-адреса
			k++;
			j = -1;
			delete[]buff;
			buff = new char[3]{};
		}
	}
	return out;
}

bool CheckMask(int mask)
{

	std::string binary = std::bitset<32>(mask).to_string(); //to binary

	if (binary[0] == '0') return false;

	if (binary[0] == '1')
	{
		for (int i = 1; i <= binary.length() - 2; i++)
		{
			if (binary[i] == '0')
			{
				for (int k = i; k <= binary.length(); k++)
				{
					if (binary[k] == '1')
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

void toBinary(unsigned long number)
{
	int bin, mod;
	int arr1[32]{};
	int arr2[32]{};
	int bitsCount = 0;

	while (number != 1)
	{
		arr1[bitsCount] = (number % 2 == 0) ? 0 : 1;
		number /= 2;
		bitsCount++;
	}
	arr1[bitsCount] = 1;
	bitsCount++;
	if (bitsCount != 32)
	{
		while (bitsCount != 32)
		{
			arr1[bitsCount] = 0;
			bitsCount++;
		}
	}
	for (int i = bitsCount - 1, j = 0; i >= 0; i--, j++)
	{
		arr2[j] = arr1[i];
	}
	int dir = 7;	// старшинство бита в октете
	int num = 0;
	int point = 0;
	for (int i = 0; i < bitsCount; i++)
	{
		num += arr2[i] * pow(2, dir);
		dir--;
		if (dir == -1)
		{
			point++;
			cout << num;
			if (point != 4)	//разделение октетов точками
				cout << '.';
			dir = 7;
			num = 0;
		}
	}
	cout << endl;
}

int main()
{
	setlocale(LC_ALL, "rus");
	unsigned long ip, mask, host, subnet, broadcast;
	char* ip_, * mask_;
	bool flag = true;
	ip_ = new char[16]{};
	mask_ = new char[16]{};
	do
	{
		if (!flag) cout << "Неверно введён адрес!" << endl;
		cout << "IP: ";
		cin >> ip_;
	} while (!(flag = CheckAddress(ip_)));
	ip = CharToLong(ip_);
	flag = true;
	do
	{
		if (!flag) cout << "Неправильная маска!" << endl;
		flag = true;
		do
		{
			if (!flag) cout << "Неверно введена маска!" << endl;
			cout << "Маска: ";
			cin >> mask_;
		} while (!(flag = CheckAddress(mask_)));
		mask = CharToLong(mask_);
	} while (!(flag = CheckMask(mask)));

	subnet = ip & mask;
	host = ip & ~mask;
	broadcast = ip & mask | ~mask;

	cout << "ID подсети: ";  toBinary(subnet);
	cout << "ID хоста: "; toBinary(host);
	cout << "Broadcast адрес: "; toBinary(broadcast);
	system("pause");
}