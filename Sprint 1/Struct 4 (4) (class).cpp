/*
Задание 4
Звание интерфейса мечты нужно доказать. Ориентируясь на старый код функции main, напишите функцию CreateSearchServer, 
которая считает стоп-слова и документы из стандартного ввода и вернёт готовый к использованию экземпляр SearchServer.
// считывает из cin стоп-слова и документ и возвращает настроенный экземпляр поисковой системы
SearchServer CreateSearchServer() {
    ...
} 
Формат входных и выходных данных
Первая строка содержит стоп-слова, разделённые пробелом. Вторая строка содержит целое число N, задающее количество документов. 
Следующие N строк содержат текст документов, в которых слова разделяются одним пробелом.
Функция CreateSearchServer должна считать стоп-слова и документы и вернуть готовый объект типа SearchServer.
Ввод
a an on the in is has been are with for from have be was
4
a small curly guinea pig with grey hair has been found
a young 50 year old crocodile wants to make friends
a strange brown creature was seen in the box of oranges
a strange animal with big ears is building a house for its friends
cheburashka with big ears likes oranges 
Вывод
Не требуется.
Как будет тестироваться ваш код
Тренажёр проверит, что CreateSearchServer считывает данные из cin и возвращает SearchServer.
Функция CreateSearchServer должна иметь пустой список параметров и возвращать SearchServer.
В первую очередь создайте объект для заполнения: SearchServer search_server;.
Затем считайте и задайте стоп-слова: search_server.SetStopWords(/* тут строка из ввода */);.
И, наконец, считайте количество документов и в цикле с соответствующим числом итераций вызывайте метод search_server.AddDocument, 
передавая id документа и его содержимое.
*/
  
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/*const int MAX_RESULT_DOCUMENT_COUNT = 5;*/

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
/*
struct DocumentContent {
    int id = 0;
    vector<string> words;
};

struct Document {
    int id;
    int relevance;
};

bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
    return lhs.relevance > rhs.relevance;
}
*/

/*
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

vector<Document> FindAllDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& query_words) {
    vector<Document> matched_documents;
    for (const auto& document : documents) {
        const int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            matched_documents.push_back({document.id, relevance});
        }
    }
    return matched_documents;
}

vector<Document> FindTopDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& stop_words, const string& raw_query) {
    const set<string> query_words = ParseQuery(raw_query, stop_words);
    auto matched_documents = FindAllDocuments(documents, query_words);

    sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance);
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
}
*/
class SearchServer 
{
public:
    void SetStopWords(const string& text) 
    {
    set<string> stop_words;
    for (const string& word : SplitIntoWords(text)) 
    {
        stop_words.insert(word);
    }
    } //here
    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        documents_.push_back({document_id, words});
    }
    
   
private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };
    vector<DocumentContent> documents_;
    set<string> stop_words_;
    
     bool IsStopWord(const string& word) {
        return stop_words_.count(word) > 0;
    }
    
    vector<string> SplitIntoWordsNoStop(const string& text) {
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (!IsStopWord(word)) {
                words.push_back(word);
            }
    }
    return words;
}
}; 

SearchServer CreateSearchServer () 
{
    SearchServer search_server;
    search_server.SetStopWords (ReadLine());
    
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) 
    {
        search_server.AddDocument(document_id, ReadLine());
    }
    return search_server;
} // here

/*
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
}*/
