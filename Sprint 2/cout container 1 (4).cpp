/*
Задание 4
Избавьтесь от дублирования кода. Вынесите общую функциональность вывода в шаблонную функцию Print, принимающую ссылку на поток вывода и объект шаблонного типа, 
элементы которого нужно вывести. Это означает, что последовательный вывод элементов контейнера теперь должен располагаться внутри функции Print. Вызовите её из 
операторов вывода вектора и множества.
Пример
const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
cout << cats << endl;
const vector<int> ages = {10, 5, 2, 12};
cout << ages << endl; 
Вывод
Белка, Георгий, Мурка, Рюрик
10, 5, 2, 12 

Не забудьте назвать вспомогательную функцию Print, первым её аргументом сделать ссылку на поток, а вторым — контейнер шаблонного типа. Точно так же вы делали с 
набором документов Documents.
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
Print (out, container);

return out;
}

template <typename Element>
ostream& operator<<(ostream& out, const set<Element>& container) {
Print (out, container);
return out;
}

int main() {  
    const set<string> cats = {"Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s};
cout << cats << endl; 

const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    return 0;
}
