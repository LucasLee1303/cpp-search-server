/*
Задание 2
Вы могли заметить лишний пробел, который выводится после последнего элемента. Избавьтесь от него и замените пробел на запятую с пробелом. 
Код из предыдущей задачи теперь должен выводить 10, 5, 2, 12.

Выводите ", "s перед каждым элементом кроме первого. Понять, что элемент первый, поможет bool-переменная.
*/
#include <iostream>
#include <vector>
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

int main() {
    const vector<int> ages = {10, 5, 2, 12};
    cout << ages << endl;
    return 0;
}
