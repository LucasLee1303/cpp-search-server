/*
Задание 5
Сделайте вывод более нарядным: выводите содержимое вектора в квадратных скобках, а множества — в фигурных. Функция Print должна остаться 
прежней и не обрамлять вывод в скобки.
Пример
const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
cout << cats << endl;
const vector<int> ages = {10, 5, 2, 12};
cout << ages << endl; 
Вывод
{Белка, Георгий, Мурка, Рюрик}
[10, 5, 2, 12] 

Не правьте функцию Print: достаточно добавить вывод скобок в каждую версию перегруженного operator<<. Не забудьте, что выводить нужно в out, а не в cout.
*/
#include <iostream>
#include <vector>
#include <set>
using namespace std;

template <typename Element> 
void Print (ostream& out, const Element& container) {
bool is_first = true;
    for (const auto& element : container) {
    if (!is_first)
        out << ", "s;
    is_first = false;
        out << element;
    }
    return;
}

template <typename Element>
ostream& operator<<(ostream& out, const vector<Element>& container) {
out << "[";                                                                      //лучше ''
Print (out, container);
out << "]";
return out;
}

template <typename Element>
ostream& operator<<(ostream& out, const set<Element>& container) {
out << "{";
Print (out, container);
out << "}";
return out;
}

int main() {  
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
cout << cats << endl; 

const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    return 0;
}
