/*Задание 2
Внедрите структуру Document в код поисковой системы. Функции FindAllDocuments и FindTopDocuments должны возвращать vector<Document> вместо vector<pair<int, int>>. 
Для сортировки вектора структур по убыванию релевантности воспользуйтесь функцией sort с компаратором HasDocumentGreaterRelevance.
Формат входных и выходных данных
Первая строка содержит стоп-слова, разделённые пробелом. Вторая строка содержит целое число N, задающее количество документов. Следующие N строк содержат 
текст документов, в которых слова разделяются одним пробелом. Затем идёт строка S поискового запроса.
Программа должна вывести информацию о топ-5 самых релевантных документов, соответствующих поисковому запросу S и отсортированных по убыванию релевантности, 
как показано в примерах ниже.
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
Ввод
a the on cat
6
a fat cat sat on a mat and ate a fat rat
fat rat
a fat cat rat
a fat cat sat
a fat cat
a fat dog
funny fat cat on a mat with rat 
Вывод
{ document_id = 0, relevance = 3 }
{ document_id = 1, relevance = 2 }
{ document_id = 2, relevance = 2 }
{ document_id = 3, relevance = 1 }
{ document_id = 4, relevance = 1 } 
Возможен и другой порядок вывода документов с одинаковой релевантностью, например:
{ document_id = 0, relevance = 3 }
{ document_id = 2, relevance = 2 }
{ document_id = 1, relevance = 2 }
{ document_id = 5, relevance = 1 }
{ document_id = 4, relevance = 1 } 
Как будет тестироваться ваш код
Тренажёр сравнит выходные данные программы с ожидаемыми и проверит, что функции FindAllDocuments и FindTopDocuments теперь возвращают vector<Document>.

Замените все pair<int, int> на Document, а при сортировке вектора документов используйте компаратор из прошлой задачи:
bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
    return lhs.relevance > rhs.relevance;
} 
Вызов reverse больше не нужен!
*/


#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
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
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

set<string> ParseStopWords(const string& text) {
    set<string> stop_words;
    for (const string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}

struct DocumentContent {
    int id = 0;
    vector<string> words;
};

struct Document {
    int id;
    int relevance;
}; // here

vector<string> SplitIntoWordsNoStop(const string& text, const set<string>& stop_words) {
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (stop_words.count(word) == 0) {
            words.push_back(word);
        }
    }
    return words;
}

void AddDocument(vector<DocumentContent>& documents, const set<string>& stop_words, int document_id,
                 const string& document) {
    const vector<string> words = SplitIntoWordsNoStop(document, stop_words);
    documents.push_back({document_id, words});
}

set<string> ParseQuery(const string& text, const set<string>& stop_words) {
    set<string> query_words;
    for (const string& word : SplitIntoWordsNoStop(text, stop_words)) {
        query_words.insert(word);
    }
    return query_words;
}

int MatchDocument(const DocumentContent& content, const set<string>& query_words) {
    if (query_words.empty()) {
        return 0;
    }
    set<string> matched_words;
    for (const string& word : content.words) {
        if (matched_words.count(word) != 0) {
            continue;
        }
        if (query_words.count(word) != 0) {
            matched_words.insert(word);
        }
    }
    return static_cast<int>(matched_words.size());
}

// Для каждого документа возвращает его релевантность и id
vector<Document> FindAllDocuments(const vector<DocumentContent>& documents,
                                        const set<string>& query_words) {
    vector<Document> matched_documents;
    for (const auto& document : documents) {
        const int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            matched_documents.push_back({relevance, document.id});
        }
    }
    return matched_documents;
}

// Возвращает топ-5 самых релевантных документов в виде пар: {id, релевантность}
bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
    return lhs.relevance > rhs.relevance; //here
} 

vector<Document> FindTopDocuments(const vector<DocumentContent>& documents,
                                        const set<string>& stop_words, const string& raw_query) {
    const set<string> query_words = ParseQuery(raw_query, stop_words);
    auto matched_documents = FindAllDocuments(documents, query_words);

    sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance); // here
    //reverse(matched_documents.begin(), matched_documents.end());
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    for (auto& matched_document : matched_documents) {
        swap(matched_document.id, matched_document.relevance); // here
    }
    return matched_documents;
}

int main() {
    const string stop_words_joined = ReadLine();
    const set<string> stop_words = ParseStopWords(stop_words_joined);

    // Read documents
    vector<DocumentContent> documents;
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        AddDocument(documents, stop_words, document_id, ReadLine());
    }

    const string query = ReadLine();
    for (auto [document_id, relevance] : FindTopDocuments(documents, stop_words, query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s
             << endl;
    }
}
