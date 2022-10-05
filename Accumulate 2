/*Задание 2
Алгоритм accumulate может работать не только с вектором, но и с другими контейнерами — например, с set.Напишите функцию AddStopWords, которая принимает строку - запрос и
множество стоп - слов и возвращает новую строку.В новой строке после запроса идут стоп - слова через пробел, перед каждым стоит дефис.Передайте в accumulate четвёртым аргументом функцию,
сворачивающую множество стоп - слов в «хвост» нужного формата.
Пример
Запрос	Стоп - слова	Выходная строка
"some tasty oranges"	{"of", "in"}	"some tasty oranges -in -of"
Так как порядок ввода стоп - слов неважен, и вы используете контейнер set, порядок стоп - слов в выходной строке будет алфавитным, вне зависимости от того,
в каком порядке стоп - слова оказались в этом контейнере.
Функция - помощница должна склеивать две строки через " -"s*/

#include <set>
#include <string>
#include <numeric>
#include <iostream>


using namespace std;

string helper(string a, string b)
{
    return a + " -"s + b;
}
string AddStopWords(const string& query, const set<string>& stop_words)
{
    string Newstring = accumulate(stop_words.begin(), stop_words.end(), query, helper);
    if (stop_words.empty())
    {
        return query;
    }
   
    return Newstring;
}

int main()
{
    string query = "some tasty oranges";
    set<string> stop_words = { "of", "in" };
    cout << AddStopWords(query, stop_words) << endl;
}
