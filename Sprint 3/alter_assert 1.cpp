/*
Задание
Разработайте функцию Assert, проверяющую значение на равенство true. Если значение равно false, пусть выводится сообщение Assertion failed., 
а программа аварийно прекращает работу. Для завершения работы примените функцию abort. При непустом значении параметра hint дополнительно должен выводиться 
текст подсказки. Так, заготовка кода должна выводить сообщение:
Assertion failed. Hint: Greeting must be non-empty 

Реализуйте Assert аналогично функции AssertEqual.
*/
#include <cstdlib>
#include <iostream>
#include <string>
#include <iomanip> 

using namespace std;

void Assert(bool value, const string& hint) {
    // Реализуйте тело функции Assert
    if (value == false){
        cout << boolalpha;
        cout << "Assertion failed.";
        if (!hint.empty())
        {
            cout << " Hint: " << hint;
        }
        cout << endl;
        abort();
    }
}

int main() {
    const string greeting = "Hello"s;
    // Намеренная ошибка в условии, чтобы показать работу Assert
    Assert(greeting.empty(), "Greeting must be non-empty"s);
    // Следующая строка не должна выполниться, так как Assert аварийно завершит работу программы
    cout << "This line will not be printed"s << endl;
}
