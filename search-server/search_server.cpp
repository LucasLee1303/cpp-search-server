#include "search_server.h"

void SearchServer::AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings) {
        if ((document_id < 0) || (documents_.count(document_id) > 0)) {
            throw std::invalid_argument("Invalid document_id"s);
        }
        const auto words = SplitIntoWordsNoStop(document);

        const double inv_word_count = 1.0 / words.size();
        for (const std::string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        for (const std::string& word : words) {
            get_word_freqs_[document_id][word] += inv_word_count; 
        }

        documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
        document_ids_.emplace(document_id);
    }

std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query, DocumentStatus status) const {
    return FindTopDocuments(raw_query,
        [status](int document_id, DocumentStatus new_status, int rating) {
            return new_status == status;
        });
}

std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query) const {
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
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

std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const std::string& raw_query, int document_id) const {
    const Query query = ParseQuery(raw_query);

    std::vector<std::string> matched_words;
    for (const std::string& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.push_back(word);
        }
    }
    for (const std::string& word : query.minus_words) {
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

 const std::map<std::string, double>& SearchServer::GetWordFrequencies(int document_id) const {
    static const std::map<std::string, double> blank;
    if (get_word_freqs_.count(document_id) == 0) {
        return blank;
    }
    else {
        return get_word_freqs_.at(document_id);
    }
 }

void SearchServer::RemoveDocument(int document_id) {
    for (auto [word, freq] : get_word_freqs_.at(document_id)) {
        word_to_document_freqs_.at(word).erase(document_id);
    }
    get_word_freqs_.erase(document_id);
    documents_.erase(document_id);
    document_ids_.erase(document_id);   
}

bool SearchServer::IsStopWord(const std::string& word) const {
    return stop_words_.count(word) > 0;
}

bool SearchServer::IsValidWord(const std::string& word) {
    return none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
        });
}


std::vector<std::string> SearchServer::SplitIntoWordsNoStop(const std::string& text) const {
    std::vector<std::string> words;
    for (const std::string& word : SplitIntoWords(text)) {
        if (!IsValidWord(word)) {
            throw std::invalid_argument("Word "s + word + " is invalid"s);
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


SearchServer::QueryWord SearchServer::ParseQueryWord(std::string text) const {
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
        throw std::invalid_argument("Query word "s + text + " is invalid");
    }

    return {  text, is_minus, IsStopWord(text) };
}

SearchServer::Query SearchServer::ParseQuery(const std::string& text) const {
    Query result;

    for (const std::string& word : SplitIntoWords(text)) {
        QueryWord query_word = ParseQueryWord(word);
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                result.minus_words.insert(query_word.data);
            }
            else {
                result.plus_words.insert(query_word.data);
            }
        }
    }
    return result;
}
