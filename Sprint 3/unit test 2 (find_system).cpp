/*
Задание
Разработайте юнит-тесты для своей поисковой системы. Примените в них макрос assert, чтобы проверить работу основных функций, таких как:
Добавление документов. Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа.
Поддержка стоп-слов. Стоп-слова исключаются из текста документов.
Поддержка минус-слов. Документы, содержащие минус-слова поискового запроса, не должны включаться в результаты поиска.
Матчинг документов. При матчинге документа по поисковому запросу должны быть возвращены все слова из поискового запроса, присутствующие в документе. 
Если есть соответствие хотя бы по одному минус-слову, должен возвращаться пустой список слов.
Сортировка найденных документов по релевантности. Возвращаемые при поиске документов результаты должны быть отсортированы в порядке убывания релевантности.
Вычисление рейтинга документов. Рейтинг добавленного документа равен среднему арифметическому оценок документа.
Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
Поиск документов, имеющих заданный статус.
Корректное вычисление релевантности найденных документов.
Как будет происходить проверка этого задания
У разработанных вами тестов должна быть точка входа, заданная функцией TestSearchServer. Код поисковой системы должен успешно проходить тесты. 
Тренажёр проверит работу ваших тестов на нескольких предложенных реализациях класса SearchServer. Одна из реализаций будет корректной, в других есть ошибки 
в логике работы класса. Задача считается решённой при выполнении следующих условий:
Корректная реализация класса SearchServer успешно проходит разработанные вами тесты.
Ваши тесты выявляют не менее 50% некорректных реализаций класса SearchServer.
Тренажёр ожидает, что ваша реализация класса SearchServer будет содержать следующие публичные методы:
struct Document {
    int id;
    double relevance;
    int rating;
};
enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};
class SearchServer {
public:
    void SetStopWords(const string& text) {
        // Ваша реализация данного метода
    }
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        // Ваша реализация данного метода
    }
    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        // Ваша реализация данного метода
    }
    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        // Ваша реализация данного метода
    }
    vector<Document> FindTopDocuments(const string& raw_query) const {
        // Ваша реализация данного метода
    }
    int GetDocumentCount() const {
        // Ваша реализация данного метода
    }
    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        // Ваша реализация данного метода
    }
private:
    // Реализация приватных методов вашей поисковой системы
};  
Прекод
Перед вами исходный код с примером теста, который проверяет, что стоп-слова исключаются поисковой системой при добавлении документа.
На проверку отправьте только сами тесты, располагающиеся между комментариями:
// -------- Начало модульных тестов поисковой системы ---------- и
// --------- Окончание модульных тестов поисковой системы -----------.

Внимательно прочитайте описание функций поисковой системы в условии. Напишите тесты, которые проверят работу 
соответствующих методов класса. Уделите внимание проверке возвращаемых результатов методов. Ваши тесты должны сравнивать ожидаемые 
результаты работы методов класса с действительными. Поэтому желательно писать их, не используя результат работы тестируемого метода как ожидаемый.
*/
///---------------------------------------ИСХОДНИК------------------------------------------
#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/* Подставьте вашу реализацию класса SearchServer сюда */

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
}

/*
Разместите код остальных тестов здесь
*/

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}

///-------------------------------------РЕШЕНИЕ--------------------------------------------------
#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
 
using namespace std;
 
/* Подставьте вашу реализацию класса SearchServer сюда (конец 2 спринта) */
/*
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
    double relevance;
    int rating;
};
 
enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};
 
class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }    
    
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, 
            DocumentData{
                ComputeAverageRating(ratings), 
                status
            });
    }
    
    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {            
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, document_predicate);
        
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                    return lhs.rating > rhs.rating;
                } else {
                    return lhs.relevance > rhs.relevance;
                }
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
 
    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(raw_query, [status](int document_id, DocumentStatus document_status, int rating) { document_id += rating ;
                return document_status == status;
            });
    }
 
    vector<Document> FindTopDocuments(const string& raw_query) const {            
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }
 
    int GetDocumentCount() const {
        return documents_.size();
    }
    
    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, documents_.at(document_id).status};
    }
    
private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
 
    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    
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
    
    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
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
        set<string> plus_words;
        set<string> minus_words;
    };
    
    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }
    
    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }
    
    template <typename LambdaType>
    vector<Document> FindAllDocuments(const Query& query, LambdaType lambda_function) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                if (lambda_function(document_id, documents_.at(document_id).status, documents_.at(document_id).rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
                
            }
        }
        
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }
 
        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({
                document_id,
                relevance,
                documents_.at(document_id).rating
            });
        }
        return matched_documents;
    }
};
 
*/
 
// -------- Начало модульных тестов поисковой системы ----------
 
// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }
 
    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
}
 
 
 
 
/*
Разместите код остальных тестов здесь
*/
 
// Тест проверяет, что добавленный документ находится по поисковому запросу, который содержит слова из документа.
void TestDocumentsAdding() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("cat"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }
 
    // Затем убеждаемся, что поиск слова не из документа, 
    // возвращает пустой результат
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("dog near red town"s);
        assert(found_docs.empty());
    }
}
 
 
void TestStopWordsSupporting() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const string stop_words = "in the"s;
    const vector<int> ratings = {1, 2, 3};
    
    // Сначала убеждаемся, что Стоп-слова исключаются из текста документов
    
    {
        SearchServer server;
        server.SetStopWords(stop_words);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in the town"s).empty());
    }
    
    {
        SearchServer server;
        server.SetStopWords(stop_words);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in the city"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }
 
}
 
void TestMinusWordsSupporting() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    
    // Сначала убеждаемся, что Стоп-слова исключаются из текста документов
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in the -city"s).empty());
    }
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in the city"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }
 
}
 
 
void TestMatching() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    
    // Сначала убеждаемся, что Стоп-слова исключаются из текста документов
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto [v, ds] = server.MatchDocument("dog in the big city"s, doc_id);
        assert(v.size() == 3);
    }
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto [v, ds] = server.MatchDocument("dog in the big -city"s, doc_id);
        assert(v.empty());
    }
}
 
void TestRelevanceSorting() {
 
    // Сначала убеждаемся, что релевантность отсортирована по убыванию
    
    {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = {1, 2, 3};
 
        const int doc_id_2 = 43;
        const string content_2 = "fox fox the city city city"s;
        const vector<int> ratings_2 = {4, 5, 6};
 
        const int doc_id_3 = 44;
        const string content_3 = "dog dog dog in the city"s;
        const vector<int> ratings_3 = {2, 4, 2};
 
        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        assert(found_docs.size() == 3);
        assert(found_docs[0].id == doc_id_3);
        assert(found_docs[1].id == doc_id_2);
        assert(found_docs[2].id == doc_id_1);
    }
    
    {
        const int doc_id_1 = 52;
        const string content_1 = "cat fox dog in the city"s;
        const vector<int> ratings_1 = {1, 2, 3};
 
        const int doc_id_2 = 53;
        const string content_2 = "fox dog the city city city"s;
        const vector<int> ratings_2 = {4, 5, 6};
 
        const int doc_id_3 = 54;
        const string content_3 = "dog in the city city city"s;
        const vector<int> ratings_3 = {2, 4, 2};
 
        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        assert(found_docs.size() == 3);
        assert(found_docs[0].id == doc_id_1);
        assert(found_docs[1].id == doc_id_2);
        assert(found_docs[2].id == doc_id_3);
    }
 
    
}
 
void TestRating() {
 
    // Сначала убеждаемся, что релевантность отсортирована по убыванию
    
    {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = {1, 2, 3};
 
        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        assert(!found_docs.empty());
        assert(found_docs[0].rating == round((1+2+3+0.0)/3));
    }
}
 
void TestLambdaFilter() {
 
    // Сначала убеждаемся, что релевантность отсортирована по убыванию
    const int doc_id = 42;
    const string content = "cat in the city city city"s;
    const vector<int> ratings = {1, 2, 3};
    const DocumentStatus document_status = DocumentStatus::ACTUAL;
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        assert(!found_docs.empty());
        assert(found_docs[0].id == doc_id);
    }
 
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::BANNED, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        assert(found_docs.empty());
    }
 
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, [] (int document_id, DocumentStatus status, int rating) {
            document_id +=1;
            if (status == DocumentStatus::ACTUAL) {document_id +=1;};
            return rating > 1;
        });
        assert(!found_docs.empty());
        assert(found_docs[0].id == doc_id);
    }
 
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, [] (int document_id, DocumentStatus status, int rating) {
                     document_id +=1;
                     if (status == DocumentStatus::ACTUAL) {document_id +=1;};
                     return rating > 3;
        });
        assert(found_docs.empty());
    }
 
}
 
void TestStatus() {
 
    // Сначала убеждаемся, что релевантность отсортирована по убыванию
    const int doc_id = 42;
    const string content = "cat in the city city city"s;
    const vector<int> ratings = {1, 2, 3};
    const DocumentStatus document_status = DocumentStatus::BANNED;
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        assert(found_docs.empty());
    }
 
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::BANNED, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::BANNED);
        assert(!found_docs.empty());
        assert(found_docs[0].id == doc_id);
    }
 
 
}
 
void TestRelevanceAccuracy() {
 
    // Сначала убеждаемся, что релевантность отсортирована по убыванию
    
    {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = {1, 2, 3};
 
        const int doc_id_2 = 43;
        const string content_2 = "fox fox the city city city"s;
        const vector<int> ratings_2 = {4, 5, 6};
 
        const int doc_id_3 = 44;
        const string content_3 = "dog dog dog in the city"s;
        const vector<int> ratings_3 = {2, 4, 2};
 
        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        assert(found_docs.size() == 3);
        assert(found_docs[0].id == doc_id_3);
        assert(abs(found_docs[0].relevance - log(3)/2) < 1e-6);
        assert(found_docs[1].id == doc_id_2);
        assert(abs(found_docs[1].relevance - log(3)/3) < 1e-6);
        assert(found_docs[2].id == doc_id_1);
        assert(abs(found_docs[2].relevance - log(3)/6) < 1e-6);
    }
    
    {
        const int doc_id_1 = 52;
        const string content_1 = "cat fox dog in the city"s;
        const vector<int> ratings_1 = {1, 2, 3};
 
        const int doc_id_2 = 53;
        const string content_2 = "fox dog the city city city"s;
        const vector<int> ratings_2 = {4, 5, 6};
 
        const int doc_id_3 = 54;
        const string content_3 = "dog in the city city city"s;
        const vector<int> ratings_3 = {2, 4, 2};
 
        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        assert(found_docs.size() == 3);
        assert(found_docs[0].id == doc_id_1);
       // cout << (log(3)/6 + log(3/2)/6 + log(1)/6) << endl; 
        assert(abs(found_docs[0].relevance - 0.25068) < 1e-6);
        assert(found_docs[1].id == doc_id_2);
        assert(abs(found_docs[1].relevance - 0.0675775) < 1e-6);
       //assert(abs(found_docs[1].relevance - log(3/2)/6 - log(1)/6) < 1e-6);
        assert(found_docs[2].id == doc_id_3);
        assert(abs(found_docs[2].relevance - 0) < 1e-6);
       //assert(abs(found_docs[2].relevance - log(1)/6) < 1e-6);
    }
}
 
 
 
 
// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    TestDocumentsAdding();
    TestStopWordsSupporting();
    TestMinusWordsSupporting();
    TestMatching();
    TestRelevanceSorting();
    TestRating();
    TestLambdaFilter();
    TestStatus();
    TestRelevanceAccuracy();
    
    
    // Не забудьте вызывать остальные тесты здесь
}
/*
 
// --------- Окончание модульных тестов поисковой системы -----------
 
int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}
*/
