#include "remove_duplicates.h"


void RemoveDuplicates(SearchServer& search_server) {
	std::set <int> duplicate;
	std::set<std::set<std::string>> docs;
	
	for (const int document_id : search_server) {
		auto& doc = search_server.GetWordFrequencies(document_id);
		std::set<std::string> doc_words;
		for (auto& [word, freq] : doc) {
			 doc_words.insert(word);
		}
		if (docs.count(doc_words)) {
			duplicate.insert(document_id);
		}
		else {
			docs.insert(doc_words);
		}
	}

	for (auto document_id : duplicate) {
		std::cout << "Found duplicate document id "s << document_id << std::endl;
		search_server.RemoveDocument(document_id);
	}
	}