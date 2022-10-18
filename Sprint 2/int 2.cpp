/*
Задание 2
Теперь сделайте то же самое другим способом. Сделайте функцию ComputeAverageRating приватным статическим методом класса SearchServer. 
Перед возвращаемым значением функции добавьте ключевое слово static и перенесите саму функцию внутрь класса SearchServer. В отличие от обычных методов, 
static-методы не привязаны к объектам класса и не могут обращаться к полям. Здесь приватный статический метод — просто функция. Вызвать её можно только из методов класса. Не забудьте круглые скобки.
Формат ввода
и в на
3
белый кот и модный ошейник
2 8 -3
пушистый кот пушистый хвост
3 7 2 7
ухоженный пёс выразительные глаза
4 5 -12 2 1
пушистый ухоженный кот 
Формат вывода
{ document_id = 1, relevance = 0.650672, rating = 5 }
{ document_id = 2, relevance = 0.274653, rating = -1 }
{ document_id = 0, relevance = 0.101366, rating = 2 } 

Метод должен начинаться так:
static int ComputeAverageRating(const vector<int>& ratings) { 
Вызывать его из других методов этого класса можно как обычную функцию.
*/
#include <algorithm>
#include <cmath>
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
	int id;
	double relevance;
	int rating;
};



class SearchServer 
{
public:
	void SetStopWords(const string& text) {
		for (const string& word : SplitIntoWords(text)) {
			stop_words_.insert(word);
		}
	}

	void AddDocument(int document_id, const string& document, const vector<int>& ratings) {
		++document_count_;
		const vector<string> words = SplitIntoWordsNoStop(document);
		const double inv_word_count = 1.0 / words.size();
		for (const string& word : words) {
			word_to_document_freqs_[word][document_id] += inv_word_count;
		}
		document_ratings_.emplace(document_id, ComputeAverageRating(ratings));
	}

	vector<Document> FindTopDocuments(const string& raw_query) const {
		const Query query = ParseQuery(raw_query);
		auto matched_documents = FindAllDocuments(query);

		sort(matched_documents.begin(), matched_documents.end(),
			[](const Document & lhs, const Document & rhs) {
				return lhs.relevance > rhs.relevance;
			});
		if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
			matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
		}
		return matched_documents;
	}

private:
	int document_count_ = 0;
	set<string> stop_words_;
	map<string, map<int, double>> word_to_document_freqs_;
	map <int, int> document_ratings_;

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
		return { text, is_minus, IsStopWord(text) };
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
    

	// Existence required
	double ComputeWordInverseDocumentFreq(const string& word) const {
		return log(document_count_ * 1.0 / word_to_document_freqs_.at(word).size());
	}

	vector<Document> FindAllDocuments(const Query & query) const {
		map<int, double> document_to_relevance;
		for (const string& word : query.plus_words) {
			if (word_to_document_freqs_.count(word) == 0) {
				continue;
			}
			const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
			for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
				document_to_relevance[document_id] += term_freq * inverse_document_freq;
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
			matched_documents.push_back({ document_id, relevance, document_ratings_.at(document_id) });
		}
		return matched_documents;
	}
    
    static int ComputeAverageRating(const vector<int>& ratings) 
{
	if (ratings.empty())
	{
		return 0;
	}
	int rating_sum = 0;
	for (const int rating : ratings)
	{
		rating_sum += rating;
	}
	int rating_size = ratings.size();
	return rating_sum / rating_size;
}
};

SearchServer CreateSearchServer() {
	SearchServer search_server;
	search_server.SetStopWords(ReadLine());

	const int document_count = ReadLineWithNumber();
	for (int document_id = 0; document_id < document_count; ++document_id) 
	{
		const string document = ReadLine();
		int ratings_size;
		cin >> ratings_size;

		vector <int> ratings(ratings_size, 0);
		for (int& rating : ratings)
		{
			cin >> rating;
		}
		search_server.AddDocument(document_id, document, ratings);
		ReadLine();
	}

	return search_server;
}

int main() {
	const SearchServer search_server = CreateSearchServer();

	const string query = ReadLine();
	for (const Document& document : search_server.FindTopDocuments(query)) {
		cout << "{ document_id = "s << document.id << ", "s
			<< "relevance = "s << document.relevance << ", " << "rating = " << document.rating << " }"s << endl;
	}
}
