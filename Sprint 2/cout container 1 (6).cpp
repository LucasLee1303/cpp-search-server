/*
Задание 6
Научитесь выводить словари. Содержимое словаря должно быть заключено в угловые скобки, а каждая пара ключ-значение — в круглые.
Пусть функция Print выводит словарь без угловых скобок. Ни для каких других типов, кроме векторов, множеств и словарей, вызывать Print не нужно.
Пример
const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
cout << cats << endl;
const vector<int> ages = {10, 5, 2, 12};
cout << ages << endl;
const map<string, int> cat_ages = {
    {"Мурка"s, 10}, 
    {"Белка"s, 5},
    {"Георгий"s, 2}, 
    {"Рюрик"s, 12}
};
cout << cat_ages << endl; 
Вывод
{Белка, Георгий, Мурка, Рюрик}
[10, 5, 2, 12]
<(Белка, 5), (Георгий, 2), (Мурка, 10), (Рюрик, 12)> 

Напишите ещё одну версию operator<< с двумя шаблонными параметрами — для map<Key, Value>.
Функция Print, итерируясь по словарю, будет пытаться вывести пару — напишите и для неё operator<<.
При вызове одной функции из другой компилятор «видит» только те из них, что написаны выше. Поэтому выше всех должен быть оператор вывода пары, 
а сразу за ним — универсальная Print.
*/

#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;

template <typename First, typename Second> 
ostream& operator<< (ostream& out, const pair <First, Second>& pair) {
return out << "(" << pair.first << ", " << pair.second << ")";
}

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
out << "[";
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

template <typename Key, typename Value>
ostream& operator<<(ostream& out, const map<Key, Value>& container) {
out << "<";
Print (out, container);
out << ">";
return out;
}

int main() {  
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
cout << cats << endl; 

const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    
    const map<string, int> cat_ages = {
    {"Мурка"s, 10}, 
    {"Белка"s, 5},
    {"Георгий"s, 2}, 
    {"Рюрик"s, 12}
};
cout << cat_ages << endl; 
    return 0;
}
