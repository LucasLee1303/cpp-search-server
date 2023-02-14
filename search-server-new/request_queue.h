#pragma once

#include <deque>

#include "search_server.h"
#include "document.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    struct QueryResult {
        int counter;
        int results;
    };

    std:: deque<QueryResult> requests;
    const SearchServer& search_server;
    int empty_requests;
    int current_time;
    const static int min_in_day = 1440;

    void AddRequest(int null_requests);
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    const auto result = search_server.FindTopDocuments(raw_query, document_predicate);
    AddRequest(result.size());
        return result;
}