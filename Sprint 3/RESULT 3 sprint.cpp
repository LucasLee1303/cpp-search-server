/*
Задание
Это задание — итоговый проект третьего спринта. Вы будете сдавать его на проверку через репозиторий на GitHub. А пока сохраните решение в своей IDE.
Примените макросы ASSERT, ASSERT_HINT, ASSERT_EQUAL и ASSERT_EQUAL_HINT для проверки работы основных функций поисковой системы, таких как:
Добавление документов. Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа.
Поддержка стоп-слов. Стоп-слова исключаются из текста документов.
Поддержка минус-слов. Документы, содержащие минус-слова из поискового запроса, не должны включаться в результаты поиска.
Соответствие документов поисковому запросу. При этом должны быть возвращены все слова из поискового запроса, присутствующие в документе. 
Если есть соответствие хотя бы по одному минус-слову, должен возвращаться пустой список слов.
Сортировка найденных документов по релевантности. Возвращаемые при поиске документов результаты должны быть отсортированы в порядке убывания релевантности.
Вычисление рейтинга документов. Рейтинг добавленного документа равен среднему арифметическому оценок документа.
Фильтрация результатов поиска с использованием предиката, задаваемого пользователем.
Поиск документов, имеющих заданный статус.
Корректное вычисление релевантности найденных документов.
Как будет происходить проверка этого задания
Разработанные вами тесты должны иметь точку входа, заданную функцией TestSearchServer. Код поисковой системы должен успешно проходить ваши тесты. 
Тренажёр проверит работу тестов на нескольких реализациях класса SearchServer. Одна из реализаций будет корректной, в других будут ошибки в логике работы класса. 
Задача считается решённой при выполнении следующих условий:
Корректная реализация класса SearchServer успешно проходит тесты, которые вы разработали;
Ваши тесты выявляют не менее 50% некорректных реализаций класса SearchServer;
Ваши тесты используют макросы фреймворка вместо стандартного макроса assert.
Тренажёр ожидает, что ваша реализация класса SearchServer будет содержать следующие публичные методы.
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
Перед вами исходный код с примером теста, проверяющего, что стоп-слова при добавлении документа исключаются поисковой системой. 
На проверку отправьте только сами тесты, которые надо поместить между комментариями:
// -------- Начало модульных тестов поисковой системы ---------- и
// --------- Окончание модульных тестов поисковой системы -----------.
Не отправляйте объявления макроса ASSERT и других макросов, иначе ваши тесты не пройдут проверку.

Используйте свои готовые юнит-тесты поисковой системы. Макрос assert в них замените на подходящие по смыслу макросы ASSERT и ASSERT_EQUAL. 
ASSERT_EQUAL выдаёт больше полезной информации, когда нарушается условие равенства проверяемых значений.
*/
-----------------------------------------------РЕШЕНИЕ----------------------------------------------

#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "search_server.h"

using namespace std;

/* Подставьте вашу реализацию класса SearchServer сюда */

/*
   Подставьте сюда вашу реализацию макросов
   ASSERT, ASSERT_EQUAL, ASSERT_EQUAL_HINT, ASSERT_HINT и RUN_TEST
*/

/*template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
    const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}


void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
    const string& hint) {
    if (!value) {
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}*/



/*template <typename TestFunc>
void RunTestImpl(TestFunc func_, const string& func) {
    cerr << func;
    func_();
    cerr << " OK"s << endl;
}*/
 
//#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

//#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

//#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

//#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

//#define RUN_TEST(func) RunTestImpl((func), #func)  

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(),
                    "Stop words must be excluded from documents"s);
    }
}

/*
Разместите код остальных тестов здесь
*/
void TestDocumentsAdding() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("cat"s);
        ASSERT(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        ASSERT(doc0.id == doc_id);
    }

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("dog near red town"s);
        ASSERT(found_docs.empty());
    }
}
 
 
void TestStopWordsSupporting() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const string stop_words = "in the"s;
    const vector<int> ratings = {1, 2, 3};

    {
        SearchServer server;
        server.SetStopWords(stop_words);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT(server.FindTopDocuments("in the town"s).empty());
    }
    
    {
        SearchServer server;
        server.SetStopWords(stop_words);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in the city"s);
        ASSERT(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        ASSERT(doc0.id == doc_id);
    }
 
}
 
void TestMinusWordsSupporting() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT(server.FindTopDocuments("in the -city"s).empty());
    }
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in the city"s);
        ASSERT(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        ASSERT(doc0.id == doc_id);
    }
 
}
 
 
void TestMatching() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto [v, ds] = server.MatchDocument("dog in the big city"s, doc_id);
        ASSERT(v.size() == 3);
    }
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto [v, ds] = server.MatchDocument("dog in the big -city"s, doc_id);
        ASSERT(v.empty());
    }
}
 
void TestRelevanceSorting() {

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
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_3);
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(found_docs[2].id == doc_id_1);
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
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_1);
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(found_docs[2].id == doc_id_3);
    }
 
    
}
 
void TestRating() {

    {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = {1, 2, 3};
 
        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].rating == round((1+2+3+0.0)/3));
    }
}
 
void TestPredicate() {

    const int doc_id = 42;
    const string content = "cat in the city city city"s;
    const vector<int> ratings = {1, 2, 3};
    const DocumentStatus document_status = DocumentStatus::ACTUAL;
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].id == doc_id);
    }
 
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::BANNED, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        ASSERT(found_docs.empty());
    }
 
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, [] (int document_id, DocumentStatus status, int rating) {
            document_id +=1;
            if (status == DocumentStatus::ACTUAL) {document_id +=1;};
            return rating > 1;
        });
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].id == doc_id);
    }
 
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, [] (int document_id, DocumentStatus status, int rating) {
                     document_id +=1;
                     if (status == DocumentStatus::ACTUAL) {document_id +=1;};
                     return rating > 3;
        });
        ASSERT(found_docs.empty());
    }
 
}
 
void TestStatus() {

    const int doc_id = 42;
    const string content = "cat in the city city city"s;
    const vector<int> ratings = {1, 2, 3};
    const DocumentStatus document_status = DocumentStatus::BANNED;
    
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        ASSERT(found_docs.empty());
    }
 
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::BANNED, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::BANNED);
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].id == doc_id);
    }
 
 
}
 
void TestRelevanceAccuracy() {

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
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_3);
        ASSERT(abs(found_docs[0].relevance - log(3)/2) < 1e-6);
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(abs(found_docs[1].relevance - log(3)/3) < 1e-6);
        ASSERT(found_docs[2].id == doc_id_1);
        ASSERT(abs(found_docs[2].relevance - log(3)/6) < 1e-6);
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
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_1);
        ASSERT(abs(found_docs[0].relevance - 0.25068) < 1e-6);
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(abs(found_docs[1].relevance - 0.0675775) < 1e-6);
        ASSERT(found_docs[2].id == doc_id_3);
        ASSERT(abs(found_docs[2].relevance - 0) < 1e-6);
       
    }
}
 

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
     RUN_TEST(TestDocumentsAdding);
     RUN_TEST(TestStopWordsSupporting);
     RUN_TEST(TestMinusWordsSupporting);
     RUN_TEST(TestMatching);
     RUN_TEST(TestRelevanceSorting);
     RUN_TEST(TestRating);
     RUN_TEST(TestPredicate);
     RUN_TEST(TestStatus);
     RUN_TEST(TestRelevanceAccuracy);
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

/*int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}*/
