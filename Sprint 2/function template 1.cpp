/*
Задание
Вы уже знакомы с понятием частоты слова в наборе. Примените простую функцию и вычислите частоту строк из данного вектора.

Посчитайте, сколько раз встречается каждый элемент вектора. Для этого проитерируйтесь по ним и прибавьте единицу к соответствующему 
значению в словаре, применив оператор ++.
*/
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

map<string, int> ComputeTermFreqs(const vector<string>& terms) {
    // реализуйте функцию
 
     map< string, int > temp; 
    for(auto i:terms) 
    temp[i]++;
    return temp;
}
/*
map<string, int> ComputeTermFreqs(const vector<string>& terms) {
    map<string, int> term_freqs;
    for (const string& term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}
*/

int main() {
    const vector<string> terms = {"first"s, "time"s, "first"s, "class"s};
    for (const auto& [term, freq] : ComputeTermFreqs(terms)) {
        cout << term << " x "s << freq << endl;
    }
    // вывод:
    // class x 1
    // first x 2
    // time x 1
}
