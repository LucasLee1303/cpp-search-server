/*
Задание
Разработайте шаблонную функцию RunTestImpl и использующий её макрос RUN_TEST. Они должны запускать тесты и 
выводить сообщения об их завершении в стандартный поток ошибок.

Чтобы узнать имя функции, получите строковое представление параметра func так же, как получаете строковое значение аргументов в макросе ASSERT_EQUAL. 
Передайте его в функцию RunTestImpl вторым параметром. В качестве первого параметра функция RunTestImpl должна принимать функцию-тест. 
Для этого функцию RunTestImpl сделайте шаблонной — компилятор сможет самостоятельно вывести тип первого аргумента.
Чтобы проверить, как работает фреймворк, попробуйте внести логические ошибки в методы класса Synonyms, например:
bool AreSynonyms(const string& first_word, const string& second_word) const {
    if (synonyms_.count(first_word) != 0) {
        return synonyms_.at(first_word).count(second_word) != 0;
    }
    // Ошибка сделана специально и проверяет работу фреймворка
    return true; // следует возвращать false
*/

#include <iostream>
#include <string>
 
using namespace std;
 
template <typename TestFunc>
void RunTestImpl(TestFunc func_, const string& func) {
    cerr << func;
    func_();
    cerr << " OK"s << endl;
}
 
// напишите недостающий код
#define RUN_TEST(func) RunTestImpl((func), #func)  
 
void Test1() {
    
}
 
int main() {
    RUN_TEST(Test1);
}
