/*
Задание 1
Внедрите лямбда-функцию в код метода FindTopDocuments вашей поисковой системы вместо функции HasDocumentGreaterRelevance. 
Других изменений в работе поисковой системы быть не должно.
Формат входных и выходных данных
Первая строка содержит стоп-слова, разделённые пробелом. Вторая строка содержит целое число N, задающее количество документов. 
Следующие N строк содержат текст документов, в которых слова разделяются одним пробелом. Затем идёт строка S поискового запроса.
Программа должна вывести информацию о топ-5 самых релевантных документов, соответствующих поисковому запросу S, отсортированных по убыванию релевантности, 
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
Кроме того, что тренажёр проверит работу поисковой системы, он также убедится, что вместо функции HasDocumentGreaterRelevance используется лямбда-функция.

Выполните ту же замену, которую мы сделали в тексте урока. После этого удалите функцию HasDocumentGreaterRelevance.
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

struct Document {
    int id;
    int relevance;
};

/*bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
    return lhs.relevance > rhs.relevance;
}*/

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        documents_.push_back({document_id, words});
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const set<string> query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(), [] (const Document& lhs, const Document& rhs){return lhs.relevance > rhs.relevance;}); //here
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };

    vector<DocumentContent> documents_;

    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    set<string> ParseQuery(const string& text) const {
        set<string> query_words;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            query_words.insert(word);
        }
        return query_words;
    }

    vector<Document> FindAllDocuments(const set<string>& query_words) const {
        vector<Document> matched_documents;
        for (const auto& document : documents_) {
            const int relevance = MatchDocument(document, query_words);
            if (relevance > 0) {
                matched_documents.push_back({document.id, relevance});
            }
        }
        return matched_documents;
    }

    static int MatchDocument(const DocumentContent& content, const set<string>& query_words) {
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
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
