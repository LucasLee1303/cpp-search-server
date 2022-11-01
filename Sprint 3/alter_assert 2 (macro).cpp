/*
Задание
Усовершенствуйте фреймворк. Реализуйте макросы ASSERT и ASSERT_HINT. Пусть в случае их срабатывания выводится информация об имени файла, номере строки и функции, 
где макрос сработал.
Пример диагностической информации, выводимой макросом ASSERT(2 + 2 == 5):
assert.cpp(48): main: ASSERT(2 + 2 == 5) failed. 
Пример диагностической информации, выводимой макросом ASSERT_HINT(2 + 2 == 5, "This will fail"s):
assert.cpp(48): main: ASSERT(2 + 2 == 5) failed. Hint: This will fail 

Выделите функцию AssertImpl, принимающую диагностические параметры так же, как функция AssertEqualImpl в уроке. 
Макросы ASSERT и ASSERT_HINT должны содержать код вызова этой функции с передачей нужных значений параметров.
*/
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void AssertImpl(bool value, const string& exp, const string& file,
    const string& func, unsigned line, const string& hint="") {
    if (value == false) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s << "ASSERT(" << exp << ") failed.";

        if (!hint.empty())
        {
            cout << " Hint: " << hint;
        }
        cout << endl;
        abort();
    }
}


#define ASSERT(expr) AssertImpl (expr, #expr, __FILE__, __FUNCTION__, __LINE__) /* реализовать самостоятельно */

#define ASSERT_HINT(expr, hint) AssertImpl (expr, #expr, __FILE__, __FUNCTION__, __LINE__, hint) /* реализовать самостоятельно */

int main() {
    string hello = "hello"s;
    ASSERT(!hello.empty());
    ASSERT_HINT(2 + 2 == 5, "This will fail"s);
    return 0;
}
