/*Задание 2
Вы ловко вызвали из метода AddDocument внешнюю функцию SplitIntoWordsNoStop. Делать так можно, но передача множества стоп-слов в функцию — спорное решение. 
Всё-таки это множество — часть поисковой системы, а функция SplitIntoWordsNoStop за пределами класса не используется.
Сделайте функцию SplitIntoWordsNoStop приватным методом: тогда никто снаружи класса не сможет её вызвать, а доступ ко множеству стоп-слов она, как метод, 
получит «из коробки».
Перенесите функцию SplitIntoWordsNoStop внутрь класса, в самый его конец: под private: и поля. Оставьте у полученного метода один параметр — строку, 
а множество стоп-слов в теле метода получите из поля класса, как это уже сделано в AddDocument. Тогда при вызове SplitIntoWordsNoStop не нужно будет 
передавать стоп-слова.*/

#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/*const int MAX_RESULT_DOCUMENT_COUNT = 5;

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
*/
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
}*/

/*
class SearchServer {
// Разместите здесь метод AddDocument и данные класса (вектор документов и множество стоп-слов)
};
*/
class SearchServer 
{
public:
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
    } // here
    
    vector<string> SplitIntoWordsNoStop(const string& text) {
    vector<string> words;
    for (const string& word : SplitIntoWords(text)) {
        if (!IsStopWord(word)) {
                words.push_back(word);
            } // here
    }
    return words;
}
}; 
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
