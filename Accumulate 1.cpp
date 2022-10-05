/*Урок 5 задание 1. Вычислите среднее арифметическое вектора чисел. Считайте, что среднее арифметическое пустого массива равно нулю.*/

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <cmath>

using namespace std;


double Average(const vector<int>& xs) {
    // посчитайте среднее арифметическое: поделите сумму на размер
    // суммирование начните с 0.0 - тогда результат выражения будет вещественным числом
    
    if (xs.empty())
    {
        return 0;
    }
    else
    {
        return accumulate(xs.begin(), xs.end(), 0.0) / xs.size();
    }

}

int main() {
    // не меняйте код main
    cout << Average({ 1, 2, 2 }) << endl;
    return 0;
}
