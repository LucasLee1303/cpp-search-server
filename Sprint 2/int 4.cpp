/*
Напишите функцию PrintDocuments, принимающую вектор документов и два числа: skip_start и skip_finish. Пусть функция сортирует документы по убыванию рейтинга и 
выводит документы в таком порядке, пропустив skip_start документов с наибольшим рейтингом и skip_finish с наименьшим. Считайте, что рейтинги не повторяются.
Числа на входе функции могут быть сколь угодно большими: ваша программа должна обрабатывать их корректно и не падать. Если требуется пропустить все документы или 
даже больше, не выводите ничего. Вектор документов может быть пустым, skip_start и skip_finish не обязательно меньше размера вектора. Формат вывода документов — 
в примере:
Пример
PrintDocuments(
    {
        {100, 5},
        {101, 7},
        {102, -4},
        {103, 9},
        {104, 1}
    },
    1,
    2
); 
Вывод
{ id = 101, rating = 7 }
{ id = 100, rating = 5 } 

Объявите индекс в цикле типа size_t и не вычитайте из documents.size(). Cтоит проверить, что не происходит переполнения, 
когда числа на входе PrintDocuments оказываются предельно большими.
*/
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    int rating;
};

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish)
{
	const size_t doc_size = documents.size();
	if (documents.empty())
	{
		return;
	}
	if (skip_start + skip_finish >= doc_size)
	{
		return;
	}

	sort(documents.begin(), documents.end(), [&](Document a, Document b) {return a.rating > b.rating; });
    
    for (size_t i = skip_start; i + skip_finish < documents.size(); ++i) 
    {
        cout << "{ id = "s << documents[i].id
             << ", rating = "s << documents[i].rating
             << " }"s << endl;
    }
        
/*auto l = documents.begin() + skip_start, r = documents.end() - skip_finish;
		while (l < r) {
			cout << "{ id = " << l->id << ", rating = " << l->rating << " }" << endl;
			++l;
		} or*/


int main() {
    PrintDocuments({{100, 5}, {101, 7}, {102, -4}, {103, 9}, {104, 1}}, 1, 2);
}
