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
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>


using namespace std;

/* Подставьте вашу реализацию класса SearchServer сюда */

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
        }
        else {
            word += c;
        }
    }
    words.push_back(word);

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
                }
                else {
                    return lhs.relevance > rhs.relevance;
                }
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(
            raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, [](int document_id, DocumentStatus status, int rating) { return status == DocumentStatus::ACTUAL; });
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
        return { matched_words, documents_.at(document_id).status };
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
                }
                else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
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

/*
   Подставьте сюда вашу реализацию макросов
   ASSERT, ASSERT_EQUAL, ASSERT_EQUAL_HINT, ASSERT_HINT и RUN_TEST
*/

template <typename First, typename Second>
ostream& operator<< (ostream& out, const pair <First, Second>& pair) {
    return out << pair.first << ": " << pair.second;
}

template <typename Element>
void Print(ostream& out, const Element& container) {
    bool is_first = true;
    for (const auto& element : container) {
        if (!is_first)
            out << ", "s;
        is_first = false;
        out << element;
    }
    return;
}

template <typename Element>
ostream& operator<<(ostream& out, const vector<Element>& container) {
    out << "[";
    Print(out, container);
    out << "]";
    return out;
}
template <typename Element>
ostream& operator<<(ostream& out, const set<Element>& container) {
    out << "{";
    Print(out, container);
    out << "}";
    return out;
}

template <typename Key, typename Value>
ostream& operator<<(ostream& out, const map<Key, Value>& container) {
    out << "{";
    Print(out, container);
    out << "}";
    return out;
}

template <typename T, typename U>
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
}



template <typename TestFunc>
void RunTestImpl(TestFunc func_, const string& func) {
    cerr << func;
    func_();
    cerr << " OK"s << endl;
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define RUN_TEST(func) RunTestImpl((func), #func)  

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
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
1. тест на добавление документов можно было бы организовать проще - используя метод, который возвращает количество документов!___Исправила
*/
void TestDocumentsAdding() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };

    //Было
   /* {
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
    */
    //Стало
    {
        SearchServer server;
        ASSERT_EQUAL(server.GetDocumentCount(), 0);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        server.AddDocument(doc_id + 1, "dog near red town", DocumentStatus::ACTUAL, ratings);
        ASSERT_EQUAL(server.GetDocumentCount(), 2);
        const auto found_docs = server.FindTopDocuments("cat"s);
        ASSERT(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        ASSERT(doc0.id == doc_id);
    }

}

/*2. у вас же есть уже реализация на исключение стоп слов!___Убрала тест__Исправила*/
/*
void TestStopWordsSupporting() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const string stop_words = "in the"s;
    const vector<int> ratings = { 1, 2, 3 };

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
*/
void TestMinusWordsSupporting() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };

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
    const vector<int> ratings = { 1, 2, 3 };


    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto [matched_words, doc_status] = server.MatchDocument("dog in the big city"s, doc_id);
        //3. v, ds - стоит делать более информативные наименования переменных, не ленитесь с этим. через пару недель будет трудно вспомнить, что подразумевали под данными переменными
        ASSERT(matched_words.size() == 3);
        //4. проверка на матчинг - это не только количество слов, стоит сравнить и наборы слов___Исправила
        
       
    }
    //сделала так
   {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto [matched_words, doc_status] = server.MatchDocument("dog in the big city"s, doc_id);
        const vector<string> matched_result = {"city"s};
        ASSERT_EQUAL(matched_result, matched_words);
    }



   {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto [matched_words, doc_status] = server.MatchDocument("dog in the big -city"s, doc_id);
        ASSERT(matched_words.empty());
    }

}

void TestRelevanceSorting() {

    {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };

        const int doc_id_2 = 43;
        const string content_2 = "fox fox the city city city"s;
        const vector<int> ratings_2 = { 4, 5, 6 };

        const int doc_id_3 = 44;
        const string content_3 = "dog dog dog in the city"s;
        const vector<int> ratings_3 = { 2, 4, 2 };

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
    /*5. достаточно одного блока для проверки!___Убрала блок___Исправила*/
    /* {
        const int doc_id_1 = 52;
        const string content_1 = "cat fox dog in the city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };

        const int doc_id_2 = 53;
        const string content_2 = "fox dog the city city city"s;
        const vector<int> ratings_2 = { 4, 5, 6 };

        const int doc_id_3 = 54;
        const string content_3 = "dog in the city city city"s;
        const vector<int> ratings_3 = { 2, 4, 2 };

        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_1);
        //6. к тому же у вас в этом блоке в каком порядке добавлены документы в сервер, в таком и выдаются - не видна сортировка!
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(found_docs[2].id == doc_id_3);
    }*/


}
/*7. не хватает глагола в наименовании теста. Что за действие вы проверяете?!___Исправила*/
//Было void TestRating() 
//Стало
void TestRatingCalculation()
{

    {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };

        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].rating == round ((1 + 2 + 3 + 0.0) / 3)); /* 8. отлично, но по стилю совет - всегда ставьте вокруг бинарных операций по одному пробелу!___Добавила пробелы*/
    }
}
/*9. не хватает глагола. Лучше "тест поиска документов по предикату"!!!___Исправила*/
// Было void TestPredicate() 
// Стало
void TestSearchDocumentsByPredicate ()
{

    const int doc_id = 42;
    const string content = "cat in the city city city"s;
    const vector<int> ratings = { 1, 2, 3 };
    const DocumentStatus document_status = DocumentStatus::ACTUAL;

   /* {
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
    } убрала 2 блока*/
    /*10. первый два блока относятся скорее к поиску по статусу!!!___Исправила*/
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, []([[maybe_unused]] int document_id, DocumentStatus status, int rating)
            { 
            document_id += 1; /*11. [[maybe_unused]]___Исправила*/
            if (status == DocumentStatus::ACTUAL) { document_id += 1; };
            return rating > 1;
            });
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].id == doc_id);
    }

    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, []([[maybe_unused]] int document_id, DocumentStatus status, int rating) {
            document_id += 1; /*12.[[maybe_unused]]___Исправила */
            if (status == DocumentStatus::ACTUAL) { document_id += 1; };
            return rating > 3;
            });
        ASSERT(found_docs.empty());
    }

}
/*13. "тест поиска документов по статусу"!___Исправила*/
// Было void TestStatus() 
// Стало
void TestSearchDocumentsByStatus ()
{

    const int doc_id = 42;
    const string content = "cat in the city city city"s;
    const vector<int> ratings = { 1, 2, 3 };
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
        const vector<int> ratings_1 = { 1, 2, 3 };

        const int doc_id_2 = 43;
        const string content_2 = "fox fox the city city city"s;
        const vector<int> ratings_2 = { 4, 5, 6 };

        const int doc_id_3 = 44;
        const string content_3 = "dog dog dog in the city"s;
        const vector<int> ratings_3 = { 2, 4, 2 };

        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_3);
        ASSERT(abs(found_docs[0].relevance - log(3) / 2) < 1e-6);
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(abs(found_docs[1].relevance - log(3) / 3) < 1e-6);
        ASSERT(found_docs[2].id == doc_id_1);
        ASSERT(abs(found_docs[2].relevance - log(3) / 6) < 1e-6);

    }

    {
        const int doc_id_1 = 52;
        const string content_1 = "cat fox dog in the city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };

        const int doc_id_2 = 53;
        const string content_2 = "fox dog the city city city"s;
        const vector<int> ratings_2 = { 4, 5, 6 };

        const int doc_id_3 = 54;
        const string content_3 = "dog in the city city city"s;
        const vector<int> ratings_3 = { 2, 4, 2 };

        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(found_docs.size() == 3);

        ASSERT(found_docs[0].id == doc_id_1);
        ///14. в первом блоке формулу использовали. а здесь также стоит сделать с формулой___ Исправила
        ASSERT(abs(found_docs[0].relevance - (log((3 * 1.0) / 1) + log((3 * 1.0) / 2)) / 6) < 1e-6);     // работает
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(abs(found_docs[1].relevance - log((3 * 1.0) / 2) * (1.0 / 4.0) / 1.5) < 1e-6); //работает
        ASSERT(found_docs[2].id == doc_id_3);
        ASSERT(abs(found_docs[2].relevance - log (1) / 6) < 1e-6);//работает

    }

}


// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestDocumentsAdding);
    //RUN_TEST(TestStopWordsSupporting);
    RUN_TEST(TestMinusWordsSupporting);
    RUN_TEST(TestMatching);
    RUN_TEST(TestRelevanceSorting);
    RUN_TEST(TestRatingCalculation);
    RUN_TEST(TestSearchDocumentsByPredicate);
    RUN_TEST(TestSearchDocumentsByStatus);
    RUN_TEST(TestRelevanceAccuracy);
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;

}
