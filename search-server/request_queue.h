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
        int Counter;
        int Results;
    };

    std:: deque<QueryResult> Requests;
    const SearchServer& Search_Server;
    int EmptyRequests;
    int CurrentTime;
    const static int MinInDay = 1440;

    void AddRequest(int NullRequests);
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
    const auto result = Search_Server.FindTopDocuments(raw_query, document_predicate);
        AddRequest(result.size());
        return result;
}