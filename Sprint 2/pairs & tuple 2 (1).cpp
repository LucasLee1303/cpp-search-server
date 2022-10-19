/*
Задание 1
Перепишите компаратор из предыдущего урока, используя пары.

Сравнить lhs и rhs по убыванию сначала одной характеристики, а затем другой — то же самое, что сравнить по убыванию пары из этих характеристик. 
Или по возрастанию, если поменять местами lhs и rhs.
*/
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    double relevance;
    int rating;
};

void SortDocuments(vector<Document>& matched_documents) {
    sort(matched_documents.begin(), matched_documents.end(),
         [](const Document& lhs, const Document& rhs) {
             
             return pair (lhs.rating, lhs.relevance) > pair (rhs.rating, rhs.relevance); 
                // lhs.rating > rhs.rating ||(lhs.rating == rhs.rating && lhs.relevance > rhs.relevance)
         });
}

int main() {
    vector<Document> documents = {{100, 0.5, 4}, {101, 1.2, 4}, {102, 0.3, 5}};
    SortDocuments(documents);
    for (const Document& document : documents) {
        cout << document.id << ' ' << document.relevance << ' ' << document.rating << endl;
    }

    return 0;
}
