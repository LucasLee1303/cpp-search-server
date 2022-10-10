/*
Задание
Научитесь обрабатывать запросы с минус-словами. Прежние условия сохраняются:
слова в документах и запросах разделены ровно одним пробелом;
перед первым словом и после последнего пробелов нет;
Единственное нововведение: запрос может содержать минус-слова — слова с минусом в начале. В документе минус-слов быть не может.
Пример входных и выходных данных
Ввод
и в на с
3
найден белый кот . на нём модный ошейник
пушистый кот ищет хозяина . особые приметы : пушистый хвост
в парке горького найден ухоженный пёс с выразительными глазами
пушистый ухоженный кот -ошейник 
Вывод
{ document_id = 1, relevance = 2 }
{ document_id = 2, relevance = 1 } 
Примечание
Документ 0 не подходит из-за слова «ошейник», запрещённого в запросе.
С документом 1 запрос пересекается по двум словам: «пушистый» и «кот», а с документом 2 — только по слову «ухоженный».
Как будет тестироваться ваш код
Тренажёр передаст вашей программе входные данные через стандартный поток ввода и сравнит результат её работы с ожидаемым.

Минус-слова запроса должны обрабатываться до плюс-слов. Каждый документ, где есть минус-слово, не должен включаться в результаты поиска, даже если в нём присутствуют плюс-слова.
Для хранения запроса удобно создать структуру Query с двумя множествами слов: плюс- и минус-словами. Возвращать эту структуру по строке запроса нужно в новом приватном методе — ParseQuery.
После сравнения первого символа с '-' не забудьте отрезать этот минус вызовом .substr(1), а затем проверить результат по словарю стоп-слов.
*/

#include <algorithm>
#include <iostream>
#include <map>
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
            words.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    words.push_back(word);
    
    return words;
}
    
struct Document {
    int id;
    int relevance;
};
 
class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }
    
    void AddDocument(int document_id, const string& document) {
        for (const string& word : SplitIntoWords(document)) {
            if (!IsStopWord(word)) {
                word_to_documents_[word].insert(document_id);
            }
        }
    }
 
    vector<Document> FindTopDocuments(const string& raw_query) const {        
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);
        
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
    
private:
    map<string, set<int>> word_to_documents_;
    set<string> stop_words_;
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }
    
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };
    
    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {
            text,
            is_minus,
            IsStopWord(text)
        };
    }
    
    struct Query {
        vector<string> plus_words;
        vector<string> minus_words;
    };
    
    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.push_back(query_word.data);
                } else {
                    query.plus_words.push_back(query_word.data);
                }
            }
        }
        return query;
    }
 
    vector<Document> FindAllDocuments(const Query& query) const {
        map<int, int> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_documents_.count(word) == 0) {
                continue;
            }
            for (const int document_id : word_to_documents_.at(word)) {
                ++document_to_relevance[document_id];
            }
        }
        
        for (const string& word : query.minus_words) {
            if (word_to_documents_.count(word) == 0) {
                continue;
            }
            for (const int document_id : word_to_documents_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }
 
        vector<Document> matched_documents;
        for (auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({document_id, relevance});
        }
        return matched_documents;
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
    for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s
             << endl;
    }
 
    return 0;
}
