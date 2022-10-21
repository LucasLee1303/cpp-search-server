/*
Задание
Реализуйте шаблонную функцию ComputeTfIdfs, которая вычисляет TF-IDF заданного слова для каждого документа из набора.
Первый параметр documents — контейнер документов. циклом for (const auto& document : documents) можно перебрать все документы в нём, а в каждом документе — 
все слова. Тип слова, документа и набора документов может быть произвольным — ваша функция должна быть готова к любым, если есть возможность итерирования. 
Гарантируется, что и набор документов, и каждый отдельный документ имеют методы begin, end и size. Например, документом может быть строка, а словом — символ.
Второй параметр term — слово, у которого нужно вычислить TF-IDF. Его тип совпадает с типом слов, которые получаются при итерировании по документу.
Функция должна вернуть вектор вещественных чисел, который совпадает по длине с количеством документов. В первом элементе должен лежать TF-IDF слова term для 
первого документа, в последнем элементе — TF-IDF слова term для последнего документа.
Напоминаем, что TF-IDF — это произведение TF и IDF. TF слова term в документе — доля слов документа, совпадающих с term. IDF вычисляется для слова независимо 
от конкретного документа и равен логарифму (функция log из <cmath>) от documents.size() / document_freq, где знаменатель — это количество документов, содержащих term.
Пример из тренажёра должен вывести 0.081093 0.101366 0.

Ваша функция должна иметь два шаблонных параметра: Documents для набора документов и Term для слова. Указывайте их через запятую. 
Тип документа при этом никак не называйте: достаточно написать auto.
Используйте алгоритм count для подсчёта количества вхождений слова в документ.
Не забывайте перед делением приводить целые числа к double оператором static_cast.
*/

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <map>

using namespace std;

template <typename Documents, typename Term> vector <double> ComputeTfIdfs(const  Documents& documents, const  Term& term_query) {
    vector<double> tf, tf_idf;
    unsigned query_in_doc_c{}, query_count{};
    for (auto document : documents ) {
        query_count = count(document.begin(), document.end(), term_query);
        query_in_doc_c += bool(query_count);
        tf.push_back(query_count  / static_cast<double>(document.size()));
    }
    double IDF = log(documents.size() / static_cast<double>(query_in_doc_c ? query_in_doc_c : 1));
    for (auto tf_i : tf)
        tf_idf.push_back(IDF * tf_i);
    return tf_idf;}
/*
template <typename Documents, typename Term>
vector<double> ComputeTfIdfs(const Documents& documents, const Term& term) {
    vector<double> tf_idfs;
 
    int document_freq = 0;
    for (const auto& document : documents) {
        const int freq = count(document.begin(), document.end(), term);
        if (freq > 0) {
            ++document_freq;
        }
        tf_idfs.push_back(static_cast<double>(freq) / document.size());
    }
 
    const double idf = log(static_cast<double>(documents.size()) / document_freq);
    for (double& tf_idf : tf_idfs) {
        tf_idf *= idf;
    }
 
    return tf_idfs;
}
*/
int main() {
    const vector<vector<string>> documents = {
        {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
        {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
        {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
    };
    
       const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);
    for (const double tf_idf : tf_idfs) {
        cout << tf_idf << " "s;
    }
    cout << endl;
    return 0;
}
