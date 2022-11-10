/*
Задание
Это задание — итоговый проект четвертого спринта. Вы будете сдавать его на проверку через репозиторий на GitHub.
Доработайте класс SearchServer. Примените механизм исключений и реализуйте в SearchServer обработку проблем.
Конструкторы класса SearchServer должны выбрасывать исключение invalid_argument, если любое из переданных стоп-слов содержит недопустимые символы, 
то есть символы с кодами от 0 до 31. Такого требования не было в предыдущих заданиях, так как известные вам на тот момент способы обработки ошибок 
не позволяли эффективно решить эту задачу в конструкторе.
Метод AddDocument больше не должен использовать возврат значения типа bool для сообщения об успехе или ошибке. Вместо этого он должен выбрасывать 
исключение invalid_argument в следующих ситуациях:
Попытка добавить документ с отрицательным id;
Попытка добавить документ c id ранее добавленного документа;
Наличие недопустимых символов (с кодами от 0 до 31) в тексте добавляемого документа.
Методы FindDocument вместо возврата optional<vector<Document>> должны возвращать vector<Document> и выбрасывать исключение invalid_argument в следующих ситуациях:
В словах поискового запроса есть недопустимые символы с кодами от 0 до 31;
Наличие более чем одного минуса перед словами, которых не должно быть в искомых документах, например, пушистый --кот. В середине слов минусы разрешаются, 
например: иван-чай.
Отсутствие текста после символа «минус» в поисковом запросе: пушистый -.
Метод MatchDocument должен возвращать tuple<vector<string>, DocumentStatus>, выбрасывая исключение invalid_argument в тех же ситуациях, что и метод FindDocument.
Метод GetDocumentId должен выбрасывать исключение out_of_range, если индекс переданного документа выходит за пределы допустимого диапазона (0; количество документов).
Обновлённая сигнатура класса SearchServer приведена ниже:
class SearchServer {
public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words) {
        ...
    }
    explicit SearchServer(const string& stop_words_text) {
    }
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        ...
    }
    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        ...
    }
    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        ...
    }
    vector<Document> FindTopDocuments(const string& raw_query) const {
        ...
    }
    int GetDocumentCount() const {
        ...
    }
    int GetDocumentId(int index) const {
        ...
    }
    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        ...
    }
    ...
    
Выбор стратегии обработки ошибок влияет на архитектуру класса в целом. Поэтому выбирайте стратегию на этапе проектирования класса. 
Обработка ошибок с использованием кодов ошибок или optional пронизывает все методы класса — от публичных до приватных.
При переходе на механизм обработки исключений внесите изменения в сигнатуру методов, которые ранее возвращали код ошибки. 
Чтобы упростить себе задачу, сначала переведите на механизм исключений публичные методы, потом приватные методы, от которых зависят только что переведённые публичные. 
Когда внесёте изменения в очередной метод, проверьте, что программа компилируется и корректно работает.
При реализации обработки ошибок в методе GetDocumentId вам может пригодиться то, что метод at в классе vector выбрасывает исключение out_of_range, 
если передать в качестве индекса недопустимое значение.
*/
/* Решение из урока №3 (на основе optional) */
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <optional>

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

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id)
        , relevance(relevance)
        , rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) 
{
    set<string> non_empty_strings;
    for (const string& string : strings) 
    {
        if (!string.empty()) non_empty_strings.insert(string);
    }
    return non_empty_strings;
}

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    // Defines an invalid document id
    // You can refer to this constant as SearchServer::INVALID_DOCUMENT_ID
   // inline static constexpr int INVALID_DOCUMENT_ID = -1;

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) 
    {
        if (!all_of(stop_words.begin(), stop_words.end(), IsValidWord)) throw invalid_argument("the word contains a special symbol"s);
    }

    explicit SearchServer(const string& stop_words_text)
        : SearchServer(
            SplitIntoWords(stop_words_text))  // Invoke delegating constructor from string container
    {}

    void AddDocument(int document_id, const string& document, DocumentStatus status,
        const vector<int>& ratings)
    {
        if (document_id < 0) throw invalid_argument ("document with negative id"s);

        if (documents_.find(document_id) != documents_.end()) throw invalid_argument("document with the id of the previously added document"s);

        const vector<string> words = SplitIntoWordsNoStop(document);

        for (const string& word : words) if (!IsValidWord(word)) throw invalid_argument ("presence of invalid symbols");

        const double inv_word_count = 1.0 / words.size();

        for (const string& word : words) 
        {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });

        documents_sequence_number.push_back(document_id);
    }

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const
    {
        Query query = ParseQuery(raw_query);

        auto matched_documents = FindAllDocuments(query, document_predicate);

        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) 
            {
                if (abs(lhs.relevance - rhs.relevance) < 1e-6) return lhs.rating > rhs.rating;
                else 
                    return lhs.relevance > rhs.relevance;
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) 
        {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const
    {
        return FindTopDocuments(
            raw_query, [&status](int document_id, DocumentStatus new_status, int rating) {
                return new_status == status;
            });
    }


    vector<Document> FindTopDocuments(const string& raw_query) const
    {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    int GetDocumentCount() const 
    {
        return documents_.size();
    }

    int GetDocumentId(int index) const 
    {
        return documents_sequence_number.at(index);
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const
    {
        Query query = ParseQuery(raw_query);

        vector<string> matched_words;

        for (const string& word : query.plus_words) 
        {
            if (word_to_document_freqs_.count(word) == 0) continue;

            if (word_to_document_freqs_.at(word).count(document_id)) 
            {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) 
        {
            if (word_to_document_freqs_.count(word) == 0) continue;

            if (word_to_document_freqs_.at(word).count(document_id)) 
            {
                matched_words.clear();
                break;
            }
        }
        return {  matched_words, documents_.at(document_id).status };
    }


private:
    struct DocumentData 
    {
        int rating;
        DocumentStatus status;
    };

    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    vector<int> documents_sequence_number;


    bool IsStopWord(const string& word) const 
    {
        return stop_words_.count(word) > 0;
    }


    static bool IsValidWord(const string& word) 
    {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) 
            {
            return c >= '\0' && c < ' ';
            });
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const 
    {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) 
        {
            if (!IsStopWord(word)) 
            {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) 
    {
        if (ratings.empty()) return 0;

        int rating_sum = 0;
        for (const int rating : ratings) 
        {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord 
    {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const 
    {

        //string word = text;
        if (text.empty()) throw invalid_argument("Query word is empty"s);
        
        bool is_minus = false;
        if (text[0] == '-') 
        {
            is_minus = true;
            text = text.substr(1);
        }
        if (text.empty() || text[0] == '-' || !IsValidWord(text))
            throw invalid_argument("Query word "s + text + " is invalid");
        
        return { text, is_minus, IsStopWord(text) };
    }

    struct Query 
    {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const 
    {
        Query result;
        for (const string& word : SplitIntoWords(text)) 
        {
            QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) 
            {
                if (query_word.is_minus) 
                {
                    result.minus_words.insert(query_word.data);
                }
                else 
                {
                    result.plus_words.insert(query_word.data);
                }
            }
        }
        return result;
    }

    // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const 
    {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
        DocumentPredicate document_predicate) const 
    {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) 
        {
            if (word_to_document_freqs_.count(word) == 0) continue;

            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) 
            {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) 
                {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) 
        {
            if (word_to_document_freqs_.count(word) == 0) continue;

            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) 
            {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) 
        {
            matched_documents.push_back(
                { document_id, relevance, documents_.at(document_id).rating });
        }
        return matched_documents;
    }
};

// ==================== для примера =========================

void PrintDocument(const Document& document) 
{
    cout << "{ "s
        << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating << " }"s << endl;
}

void PrintMatchDocumentResult(int document_id, const vector<string>& words, DocumentStatus status) 
{
    cout << "{ "s
        << "document_id = "s << document_id << ", "s
        << "status = "s << static_cast<int>(status) << ", "s 
        << "words ="s;

    for (const string& word : words) 
    {
        cout << ' ' << word;
    }
    cout << "}"s << endl;
}

void AddDocument(SearchServer& search_server, int document_id, const string& document, DocumentStatus status,
    const vector<int>& ratings) 
{
    try 
    {
        search_server.AddDocument(document_id, document, status, ratings);
    }
    catch (const invalid_argument& e) 
    {
        cout << "\n Error adding a document "s << document_id << ": "s << e.what() << endl;
    }
}

void FindTopDocuments(const SearchServer& search_server, const string& raw_query) 
{
    cout << "\n Search results for: "s << raw_query << endl;
    try 
    {
        for (const Document& document : search_server.FindTopDocuments(raw_query)) 
        {
            PrintDocument(document);
        }
    }
    catch (const exception& e) 
    {
        cout << "\n Search error: "s << e.what() << endl;
    }
}

void MatchDocuments(const SearchServer& search_server, const string& query) 
{
    try 
    {
        cout << "\n Matching documents on request: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();

        for (int index = 0; index < document_count; ++index) 
        {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    }
    catch (const exception& e) 
    {
        cout << "\n Error in matching documents to a request "s << query << ": "s << e.what() << endl;
    }
}

int main() 
{
    SearchServer search_server("and in on"s);
    
    AddDocument(search_server, 1, "fluffy cat fluffy tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
    AddDocument(search_server, 1, "a fluffy dog and a fashionable collar"s, DocumentStatus::ACTUAL, { 1, 2 });
    AddDocument(search_server, -1, "a fluffy dog and a fashionable collar"s, DocumentStatus::ACTUAL, { 1, 2 });
    AddDocument(search_server, 3, "big dog ta\x12il"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
    AddDocument(search_server, 4, "big dog tail"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

    FindTopDocuments(search_server, "fluffy -dog"s);
    FindTopDocuments(search_server, "fluffy --cat"s);
    FindTopDocuments(search_server, "fluffy -"s);

    MatchDocuments(search_server, "fluffy dog"s);
    MatchDocuments(search_server, "fashionable -cat"s);
    MatchDocuments(search_server, "fashionable --dog"s);
    MatchDocuments(search_server, "fluffy - tail"s);
}
