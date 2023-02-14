#include "process_queries.h"

std::vector<std::vector<Document>> ProcessQueries(
	const SearchServer& search_server, 
	const std::vector<std::string>& queries) {
	
    std::vector<std::vector<Document>> docs_lists(queries.size());
	std::transform(std::execution::par, queries.begin(), queries.end(),docs_lists.begin(), 
		       [&search_server](auto& query) {return search_server.FindTopDocuments(query); });
	return docs_lists;
}

std::vector<Document> ProcessQueriesJoined(
    const SearchServer& search_server,
    const std::vector<std::string>& queries) {
	std::vector<Document> documents;

	for (const auto& document : ProcessQueries(search_server, queries)) {
		documents.insert(documents.end(), document.begin(), document.end());
	}

	return documents;
}
