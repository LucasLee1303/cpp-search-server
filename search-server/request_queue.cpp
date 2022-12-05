#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server)
    : Search_Server(search_server), EmptyRequests(0), CurrentTime(0) {}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
     const auto result = Search_Server.FindTopDocuments(raw_query, status);
        AddRequest(result.size());
        return result;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    const auto result = Search_Server.FindTopDocuments(raw_query);
        AddRequest(result.size());
        return result;
}

int RequestQueue::GetNoResultRequests() const {
    return EmptyRequests;
}

void RequestQueue::AddRequest(int NullRequests) {
    ++CurrentTime;

        for (; !Requests.empty() && MinInDay <= CurrentTime - Requests.front().Counter; )
        {
            if (0 == Requests.front().Results) --EmptyRequests;

            Requests.pop_front();
        }
        Requests.push_back({ CurrentTime, NullRequests });

        if (0 == NullRequests) ++EmptyRequests;
}