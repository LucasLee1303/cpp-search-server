/*
Задание
Поисковые системы логируют запросы, чтобы потом исследовать их и улучшать качество поиска. Смоделируйте такое исследование и напишите программу, 
которая изучает популярность определённой темы у пользователей.
На вход программа получает:
строку, содержащую число N — количество поисковых запросов,
N строк с текстом поисковых запросов,
слово W.
Программа должна вывести количество поисковых запросов, содержащих слово W. Если слово W — часть одного из слов поискового запроса, оно учитываться не должно.
Пример входных и выходных данных
Ввод:
4
apple juice 
bionic cat
funny little cat
transport catalogue
cat 
Вывод:
2 
Слово «cat» содержится в двух запросах: «bionic cat» и «funny little cat». Вхождение строки «cat» в слово «catalogue» учитываться не должно. 
Поэтому программа должна вывести число 2.
Как будет тестироватьс¤ ваш код
Тренажёр передаст вашей программе входные данные через стандартный поток ввода и сравнит выводимые данные с ожидаемыми.

Внутри лямбда-функции разделите поисковый запрос на слова, как делали в поисковой системе.
Верните true, если среди них содержится слово из переменной buzzword. Так алгоритм count_if сможет подсчитать, сколько запросов содержат искомое слово.
Чтобы использовать переменную buzzword внутри лямбда-функции, захватите её, указав имя переменной внутри квадратных скобок.

*/
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

int main() {
    const int queryCount = ReadLineWithNumber();

    vector<string> queries(queryCount);
    for (string& query : queries) {
        query = ReadLine();
    }
    const string buzzword = ReadLine();

    cout << count_if(queries.begin(), queries.end(), [buzzword](const string& query)
        {
            // Реализуйте эту лямбда-функцию
            // Верните true, если query содержит слово buzzword
            const vector <string> query_words = SplitIntoWords(query);
            return count (query_words.begin(), query_words.end(), buzzword) != 0;
            

        });
    cout << endl;
}
