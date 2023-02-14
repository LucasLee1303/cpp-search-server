#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server(search_server), empty_requests(0), current_time(0) {}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
     const auto result = search_server.FindTopDocuments(raw_query, status);
     AddRequest(result.size());
        return result;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    const auto result = search_server.FindTopDocuments(raw_query);
    AddRequest(result.size());
        return result;
}

int RequestQueue::GetNoResultRequests() const {
    return empty_requests;
}

void RequestQueue::AddRequest(int null_requests) {
    ++current_time;

        for (; !requests.empty() && min_in_day <= current_time - requests.front().counter; )
        {
            if (0 == requests.front().results) --empty_requests;

            requests.pop_front();
        }
        requests.push_back({ current_time, null_requests });

        if (0 == null_requests) ++empty_requests;
}