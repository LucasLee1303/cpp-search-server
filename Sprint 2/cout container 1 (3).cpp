/*
Задание 3
Научите функцию вывода работать не только с векторами, но и со множествами.
Пример
const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
cout << cats << endl; 
Вывод
Белка, Георгий, Мурка, Рюрик 

Шаблонизировать функцию типом контейнера вместо типа элемента не получится. Теперь даже при выводе простого числа компилятор не знает, 
вызывать ли ему стандартную функцию вывода числа или сделать вид, что число — это контейнер. Поэтому для начала ограничьтесь двумя версиями функции: 
для вектора и для множества.
*/

#include <iostream>
#include <vector>
#include <set>
using namespace std;

template <typename Element> 
ostream& operator<<(ostream& out, const vector<Element>& container) {
bool is_first = true;
    for (const auto& element : container) {
    if (!is_first)
        out << ", "s;
    is_first = false;
        out << element;
    }
    return out;
}
template <typename Element> 
ostream& operator<<(ostream& out, const set<Element>& container) {
bool is_first = true;
    for (const auto& element : container) {
    if (!is_first)
        out << ", "s;
    is_first = false;
        out << element;
    }
    return out;
}

int main() {
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
cout << cats << endl; 
    return 0;
}
