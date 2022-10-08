/* Урок 3 задание 1.
Дана функция PrintWorryCount. Перепишите ее используя count */

#include <algorithm>
#include <iostream>
#include <vector>


using namespace std;

void PrintWorryCount(const vector <bool>& is_nervous)
{
    //подсчитываем количество элементов в векторе is_nervous, равных true
    // int worry_count = 0;

    /*
    for (bool status : is_nervous)
    {
        if (status /// == true для меня///)
        {
        ++worry_count;
        }
    }
    */
    //cout << worry_count << endl;

    cout << count(is_nervous.begin(), is_nervous.end(), true); // в функии мы искали циклом значения тру
    // как и в алгоритме 
}


int main() 
{
    // не меняйте содержимое функции main
    PrintWorryCount({ true, true, false, true });
    return 0;
}
