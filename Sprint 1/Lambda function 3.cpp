/*
Задание
В приют привозят собак разных пород. В словаре shelter по названию хранится количество особей каждого вида на текущий момент. 
Приют может принять определённое количество собак каждой породы. Оно указано в словаре max_amount.
Напишите функцию, которая будет на основании вектора с именами поступивших собак обновлять словарь shelter и возвращать общее количество попавших в приют собак. 
Гарантируется, что никакие другие собаки, кроме известных из словаря max_amount, в приют не приедут.
Пример входных данных для функции
map<string, int>& shelter = {{"landseer"s, 1}, {"otterhound"s, 2}, {"pekingese"s, 2}, {"pointer"s, 3}};
const map<string, int>& max_amount = {{"landseer"s, 2}, {"otterhound"s, 3}, {"pekingese"s, 4}, {"pointer"s, 7}};
const vector<string>& new_dogs = {"landseer"s, "otterhound"s, "otterhound"s, "otterhound"s, "pointer"s}; 
Пример вывода программы
3 
Как будет тестироваться ваш код
Тренажёр проверит работу функции CountAndAddNewDogs, передавая ей различные комбинации входных данных и сравнивая возвращаемое значение и 
содержимое контейнера shelter c ожидаемым.

Примените count_if и захват по ссылке в лямбда-выражении.
*/
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int CountAndAddNewDogs(const vector<string>& new_dogs, const map<string, int>& max_amount,
    map<string, int>& shelter) {
    //...
    return count_if(new_dogs.begin(), new_dogs.end(), [&max_amount, &shelter](const string& name)
        {
            int& current_amount = shelter[name];
            if (current_amount < max_amount.at(name))
            {
                ++current_amount;
                return true;
            }
            else {
                return false;
            }
        });
}

int main() 
{
    map<string, int> shelter
    {
        {"shepherd"s, 1},
        {"retriever"s, 2},
        {"dane"s, 3},
    };
    map<string, int> max_amount
    {
        {"shepherd"s, 3},
        {"dane"s, 5},
    };
    vector<string> new_dogs
    {
        {"shepherd"s},
        {"retriever"s},
        {"dane"s},
    };
    cout << CountAndAddNewDogs(new_dogs, max_amount, shelter) << endl;
}
