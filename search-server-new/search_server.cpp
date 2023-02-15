#include "search_server.h"

void SearchServer::AddDocument(int document_id, std::string_view document, DocumentStatus status, const std::vector<int>& ratings) {
    if ((document_id < 0) || (documents_.count(document_id) > 0)) {
        throw std::invalid_argument("Invalid document_id"s);
    }
    auto words = SplitIntoWordsNoStop(document);
    const std::string doc_string{ document };
    documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status, doc_string });

    words = SplitIntoWordsNoStop(documents_.at(document_id).txt);

    const double inv_word_count = 1.0 / words.size();

    for (auto word : words) {
        word_to_document_freqs_[word][document_id] += inv_word_count;
        get_word_freqs_[document_id][word] += inv_word_count;
    }

    document_ids_.emplace(document_id);
}

std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query, DocumentStatus status) const {
    return FindTopDocuments(std::execution::seq, raw_query, status);
}

std::vector<Document> SearchServer::FindTopDocuments(std::string_view raw_query) const {
    return FindTopDocuments(std::execution::seq, raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
    return static_cast<int>(documents_.size());
}

const std::set<int>::const_iterator SearchServer::begin() const {
    return document_ids_.begin();
}

const std::set<int>::const_iterator SearchServer::end() const {
    return document_ids_.end();
}

tuple_match_doc_t SearchServer::MatchDocument(std::string_view raw_query, int document_id) const {

    if (!IsValidWord(raw_query)) {
        throw std::invalid_argument("Invalid query");
    }

    if (documents_.count(document_id) == 0) {
        throw std::out_of_range("Invalid document_id"s);
    }

    const auto query = ParseQuery(raw_query);
    const auto status = documents_.at(document_id).status;

    std::vector<std::string_view> matched_words;

    for (const std::string_view word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.clear();
            return { {}, status };
        }
    }

    for (const std::string_view word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            auto it = word_to_document_freqs_.find(word);
            if (it->first == word)
                matched_words.push_back(it->first);
        }
    }
    return { matched_words, status };
}

tuple_match_doc_t SearchServer::MatchDocument(const std::execution::sequenced_policy& policy, std::string_view raw_query, int document_id) const {
    return MatchDocument(raw_query, document_id);
}

tuple_match_doc_t SearchServer::MatchDocument(const std::execution::parallel_policy& policy, std::string_view raw_query, int document_id) const {
    if (!IsValidWord(raw_query)) {
        throw std::invalid_argument("Invalid query");
    }

    if (documents_.count(document_id) == 0) {
        throw std::out_of_range("Invalid document_id"s);
    }

    const auto& query = ParseQuery(raw_query);
    const auto status = documents_.at(document_id).status;


    if (std::any_of(query.minus_words.begin(), query.minus_words.end(),
        [this, document_id](const std::string_view word) {
            const auto it = word_to_document_freqs_.find(word);
    return it != word_to_document_freqs_.end() && it->second.count(document_id); }))
    {
        return { {}, status };
    }

    std::vector<std::string_view> matched_words;
    matched_words.reserve(query.plus_words.size());

    std::copy_if(query.plus_words.begin(), query.plus_words.end(),
        std::back_inserter(matched_words),
        [this, document_id](const std::string_view word) {
            const auto it = word_to_document_freqs_.find(word);
    return it != word_to_document_freqs_.end() && it->second.count(document_id); }
    );


    std::sort(policy, matched_words.begin(), matched_words.end());
    const auto itr = std::unique(matched_words.begin(), matched_words.end());
    matched_words.erase(itr, matched_words.end());
    return { matched_words, status };
}

const std::map<std::string_view, double>& SearchServer::GetWordFrequencies(int document_id) const {
    static const std::map<std::string_view, double> blank;
    if (get_word_freqs_.count(document_id) == 0) {
        return blank;
    }
    else {
        return get_word_freqs_.at(document_id);
    }
}

void SearchServer::RemoveDocument(int document_id) {
    if (!document_ids_.count(document_id)) return;

    const std::map<std::string_view, double>& word_freqs = get_word_freqs_.at(document_id);
    for (auto [key_string, value_double] : word_freqs) {
        word_to_document_freqs_[key_string].erase(document_id);
    }

    get_word_freqs_.erase(document_id);
    documents_.erase(document_id);
    document_ids_.erase(document_id);
}

bool SearchServer::IsStopWord(std::string_view word) const {
    return stop_words_.count(word) > 0;
}

bool SearchServer::IsValidWord(std::string_view word) {

    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
        });
}

std::vector<std::string_view> SearchServer::SplitIntoWordsNoStop(std::string_view text) const
{
    std::vector<std::string_view> words;

    for (auto word : SplitIntoWords(text)) {
        if (!IsValidWord(word)) {
            throw std::invalid_argument("Word is invalid"s);
        }
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}


int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) {
    if (ratings.empty()) {
        return 0;
    }
    int rating_sum = 0;
    for (const auto rating : ratings) {
        rating_sum += rating;
    }
    return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string_view text) const {
    QueryWord result;

    if (text.empty()) {
        throw std::invalid_argument("Query word is empty"s);
    }

    bool is_minus = false;
    if (text[0] == '-') {
        is_minus = true;
        text = text.substr(1);
    }
    if (text.empty() || text[0] == '-' || !IsValidWord(text)) {
        throw std::invalid_argument("Query word is invalid");
    }

    return { text, is_minus, IsStopWord(text) };
}

std::vector<std::string_view> SearchServer::SortUnique(std::vector<std::string_view>& container) const {

    std::sort(container.begin(), container.end());
    auto w_end = std::unique(container.begin(), container.end());
    container.resize(std::distance(container.begin(), w_end));


    return container;
}

SearchServer::Query SearchServer::ParseQuery(std::string_view text, bool par) const
{
    Query result;

    for (auto word : SplitIntoWords(text)) {
        const auto query_word = ParseQueryWord(word);
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                result.minus_words.push_back(query_word.data);
            }
            else {
                result.plus_words.push_back(query_word.data);
            }
        }
    }
    if (par) {
        result.minus_words = SortUnique(result.minus_words);
        result.plus_words = SortUnique(result.plus_words);
    }   

    return result;
}

double SearchServer::ComputeWordInverseDocumentFreq(std::string_view word) const {
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}
