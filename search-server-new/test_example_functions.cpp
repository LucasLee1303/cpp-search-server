#include "test_example_functions.h"

void AddDocument(SearchServer& search_server, int document_id, const std::string& document, DocumentStatus status,
    const std::vector<int>& ratings) 
{
    try 
    {
        search_server.AddDocument(document_id, document, status, ratings);
    }
    catch (const std::invalid_argument& e) 
    {
        std::cout << "\n Error adding a document "s << document_id << ": "s << e.what() << std::endl;
    }
}

void FindTopDocuments(const SearchServer& search_server, const std::string& raw_query) 
{
    std::cout << "\n Search results for: "s << raw_query << std::endl;
    try 
    {
        for (const Document& document : search_server.FindTopDocuments(raw_query)) 
        {
            PrintDocument(document);
        }
    }
    catch (const std::exception& e) 
    {
        std::cout << "\n Search error: "s << e.what() << std::endl;
    }
}

void MatchDocuments(const SearchServer& search_server, const std::string& query) 
{
    try 
    {
        std::cout << "\n Matching documents on request: "s << query << std::endl;

        for (const int document_id : search_server) 
        {
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    }
    catch (const std::exception& e) 
    {
        std::cout << "\n Error in matching documents to a request "s << query << ": "s << e.what() << std::endl;
    }
}