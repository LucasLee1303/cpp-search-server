// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь: 

#include <iostream>
#include <conio.h>
#include <ctime>


using namespace std;



int main()
{
	setlocale(LC_ALL, "Russian");
	
	cout << "Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3? " << endl;

	int kol = 0;
			
	for (int i = 0; i < 1000; i++)
	{
		
		int units = i % 10;
		int tens = (i / 10) % 10;
		int hundreds = i / 100;

		if (units == 3 || tens == 3 || hundreds == 3)
		{
			kol++;
			
		}

	}
	cout << "Ответ на загадку: " << kol << endl;

	

	_getch();
}
