/*
Задание 1
Доработайте оператор вывода вектора в поток, чтобы он работал с любым вектором — при условии, что сами его элементы можно выводить оператором <<. 
Так, для вектора целых чисел код из тренажёра должен выводить 10 5 2 12 .

Сделайте функцию шаблонной: замените тип string на шаблонный параметр Element.
*/
#include <iostream>
#include <vector>
using namespace std;
/*ostream& operator<<(ostream& out, const vector<int>& container) {
    for (const int& element : container) {
        out << element << " "s;
    }
    return out;
}  */

template <typename Element> 
ostream& operator<<(ostream& out, const vector<Element>& container) {
    for (const auto& element : container) {
        out << element << " "s;
    }
    return out;
}

int main() {
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    return 0;
}
