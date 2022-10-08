/*Задание
Пусть ваша поисковая система возвращает не любые документы, а максимум 5 документов с самой высокой релевантностью.Напишите функцию FindTopDocuments, которая должна 
находить не более MAX_RESULT_DOCUMENT_COUNT = 5 результатов.Объявите эту глобальную константу вверху своего решения.
Функция FindTopDocuments имеет следующий вид :
// Возвращает самые релевантные документы в виде вектора пар {id, релевантность}
vector<pair<int, int>> FindTopDocuments(
    const vector<pair<int, vector<string>>>&documents,
    const set<string>&stop_words, const string & raw_query) {
    ...
}
Вызов функции FindTopDocuments поместите там же, где для вывода итогового результата мы вызывали FindDocuments.Саму FindDocuments замените на функцию FindAllDocuments, 
которая будет возвращать все результаты поиска в виде вектора пар из релевантности документа и его идентификатора{ relevance, document_id }. Такой порядок следования элементов 
в паре облегчит сортировку пар.
// Для каждого документа возвращает пару {релевантность, id}
vector<pair<int, int>> FindAllDocuments(
    const vector<pair<int, vector<string>>>& documents,
    const set<string>& query_words) {
    ...
}
Функция FindTopDocuments должна использовать функцию FindAllDocuments и возвращать документы, отсортированные по убыванию релевантности.Документы с одинаковой релевантностью 
должны быть упорядочены по убыванию id.
Формат входных и выходных данных
Первая строка содержит стоп - слова, разделённые пробелом.Вторая строка содержит целое число N, задающее количество документов.Следующие N строк содержат текст документов, 
в которых слова разделяются одним пробелом.Затем идёт строка S поискового запроса.
Программа должна вывести информацию о топ - 5 самых релевантных документов соответствующих поисковому запросу S, отсортированных по убыванию релевантности, как показано в примере ниже.
Ввод
a an on the in is has been are with for from have be was
4
a small curly guinea pig with grey hair has been found
a young 50 year old crocodile wants to make friends
a strange brown creature was seen in the box of oranges
a strange animal with big ears is building a house for its friends
cheburashka with big ears likes oranges
Вывод
{ document_id = 3, relevance = 2 }
{ document_id = 2, relevance = 1 }
Как будет тестироваться ваш код
Тренажёр передаст программе на вход различные варианты документов и поисковых запросов и сравнит выводимые данные с ожидаемыми.Чтобы ваша программа успешно прошла проверку, 
она должна упорядочивать документы с одинаковой релевантностью по убыванию id. (нужен алгоритм)
Также тренажёр проверит, что в программе есть функция FindTopDocuments.*/

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>


using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() 
{
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() 
{
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) 
{
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

set<string> ParseStopWords(const string& text) 
{
    set<string> stop_words;
    for (const string& word : SplitIntoWords(text)) 
    {
        stop_words.insert(word);
    }
    return stop_words;
}

vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words) 
{
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) 
        {
            words.push_back(word);
        }
    }
    return words;
}

void AddDocument(vector<pair<int, vector<string>>>& documents, const set<string>& stop_words,
    int document_id, const string& document) 
{
    const vector<string> words = SplitIntoWordsNoStop(document, stop_words);
    documents.push_back(pair<int, vector<string>>{document_id, words});
}

set<string> ParseQuery(const string& text, const set<string>& stop_words) 
{
    set<string> query_words;
    for (const string& word : SplitIntoWordsNoStop(text, stop_words)) 
    {
        query_words.insert(word);
    }
    return query_words;
}

int MatchDocument(const pair<int, vector<string>>& content, const set<string>& query_words) 
{
    if (query_words.empty()) 
    {
        return 0;
    }
    set<string> matched_words;
    for (const string& word : content.second) 
    {
        if (matched_words.count(word) != 0) 
        {
            continue;
        }
        if (query_words.count(word) != 0) 
        {
            matched_words.insert(word);
        }
    }
    return static_cast<int>(matched_words.size());
}

//// Для каждого документа возвращает его id и релевантность
//vector<pair<int, int>> FindDocuments(const vector<pair<int, vector<string>>>& documents,
//    const set<string>& stop_words, const string& query) 
//{
//    const set<string> query_words = ParseQuery(query, stop_words);
//    vector<pair<int, int>> matched_documents;
//    for (const auto& document : documents) 
//    {
//        const int relevance = MatchDocument(document, query_words);
//        if (relevance > 0) 
//        {
//            matched_documents.push_back({ document.first, relevance });
//        }
//    }
//    return matched_documents;
//}


// Для каждого документа возвращает его релевантность и id
vector<pair<int, int>> FindAllDocuments(const vector<pair<int, vector<string>>>& documents,
    const set<string>& query_words)
{
    // Превратите функцию FindDocuments в FindAllDocuments
    // Первым элементом возвращаемых пар идёт релевантность документа, а вторым - его id
    vector<pair<int, int>> matched_documents;
    for (const auto& document : documents) {
        const int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            matched_documents.push_back({ relevance, document.first });
        }
    }
    return matched_documents;
}




// Возвращает топ-5 самых релевантных документов в виде пар: {id, релевантность}
vector<pair<int, int>> FindTopDocuments(const vector<pair<int, vector<string>>>& documents,
                                        const set<string>& stop_words, const string& raw_query) 
{
    // Напишите функцию, используя FindAllDocuments
    const set<string> query_words = ParseQuery(raw_query, stop_words);
    auto matched_documents = FindAllDocuments(documents, query_words);

    // Сортируем документы по возрастанию релевантности и id
    sort(matched_documents.begin(), matched_documents.end());
    // Меняем порядок следования документов, чтобы вначале оказались самые релевантные
    reverse(matched_documents.begin(), matched_documents.end());

    // Оставляем MAX_RESULT_DOCUMENT_COUNT самых релевантных документов
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    // Массив matched_documents содержит пары {релевантность, id}
    // FindTopDocuments должна вернуть пары {id, релевантность}
    // Поэтому нужно обменять элементы пар, перед тем как вернуть результат
    for (auto& matched_document : matched_documents) {
        swap(matched_document.first, matched_document.second);
    }
    return matched_documents;
}


int main() 
{
    const string stop_words_joined = ReadLine();
    const set<string> stop_words = ParseStopWords(stop_words_joined);

    // Read documents
    vector<pair<int, vector<string>>> documents;
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) 
    {
        AddDocument(documents, stop_words, document_id, ReadLine());
    }

    const string query = ReadLine();
    // Вместо FindDocuments используйте FindTopDocuments
    for (auto [document_id, relevance] : FindTopDocuments(documents, stop_words, query)) 
    {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s
            << endl;
    }
}
