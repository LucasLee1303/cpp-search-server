/*Объявите в функции main переменную типа SearchServer и обратитесь к одному из её полей — например, выведите количество стоп-слов во множестве. 
Вы получите ошибку компиляции об обращении к приватному полю. Это нормально.
Подчёркивания в названиях необязательны. Но они показывают, что обратиться к полям напрямую не получится:
#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;
class SearchServer {
private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };
    DocumentContent documents_;
    set<string> stop_words_;
}; 
Напишите прямо внутри класса функцию GetStopWordsSize, которая будет возвращать размер множества стоп-слов. Такая функция называется метод. 
Так как метод находится внутри класса, он имеет доступ к его полям. Чтобы метод можно было вызвать снаружи класса, мы снабдили его меткой public: 
в заготовке кода .
Сделайте свободную функцию GetStopWordsSize. Вызовите в ней вышеописанный метод у переданного в функцию по константной ссылке сервера.
Как будет тестироваться ваш код
Тренажёр проверит, что функция GetStopWordsSize и класс SearchServer работают так, как требуется в задании.

Вызовите метод size множества stop_words_ в методе GetStopWordsSize класса SearchServer.
Вызовите метод GetStopWordsSize сервера в функции GetStopWordsSize.*/

#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

class SearchServer 
{
    // Содержимое раздела public: доступно для вызова из кода вне класса
public:
    int GetStopWordsSize() const 
    {
        return stop_words_.size();// Верните количество стоп-слов
    }

    // Содержимое раздела private: доступно только внутри методов самого класса
private:
    struct DocumentContent 
    {
        int id = 0;
        vector<string> words;
    };

    DocumentContent documents_;
    set<string> stop_words_;
};

int GetStopWordsSize(const SearchServer& server) {
    return server.GetStopWordsSize (); // Верните количество стоп-слов у server
}

int main() {
    // 1. Создайте переменную server типа SearchServer
    SearchServer server;
    // 2. Вызовите функцию GetStopWordsSize, передав ей объект server
    GetStopWordsSize (server);
    // 3. Выведите результат, возвращёный функцией GetStopWordsSize в cout
    cout << GetStopWordsSize (server) << endl;
}
