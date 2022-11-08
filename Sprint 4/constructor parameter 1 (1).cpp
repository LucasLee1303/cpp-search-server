/*
Задание 2
Многие стандартные классы языка C++ имеют несколько конструкторов. Вот, например, несколько способов создания экземпляров классов string и vector:
#include <string>
#include <vector>
using namespace std;
int main() {
    string empty; // пустая строка
    string greeting("hello, c++"); // строка "hello, c++"
    string aaaaa(5, 'A'); // строка "AAAAA"
    string cpp(greeting, 7, 3); // строка "c++"
    vector<string> empty_vec;                 // {}
    vector<string> four_empty_strings(4);     // {"", "", "", ""|
    vector<string> three_greetings(3, "Hi!"s); // {"Hi!", "Hi!", "Hi!"} 
}
```
В дополнение к конструктору по умолчанию для класса `Rational` разработайте параметризованные конструкторы:
- Конструктор с одним параметром, создающий дробь из целого числа. Числитель этой дроби равен переданному целому числу, а знаменатель — единице.
- Конструктор, который принимает два параметра типа `int`, задающих значение числителя и знаменателя, 
а затем приводит дробь к несократимому виду с положительным знаменателем. 
Например, дробь `6/15` должна быть преобразована в `2/5`, а дробь `8/(-6)` — в `(-4)/3`. Гарантируется, что знаменатель не равен нулю.
Эти конструкторы сделают возможными такие действия над обыкновенными дробями:
```cpp
Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();
    // Создаём и возвращаем дробь с заданным числителем и знаменателем
    return Rational{numerator, denominator};
}
int main() {
    Rational zero;     // Дробь 0/1 = 0
    const Rational seven(7); // Дробь 7/1 = 7
    const Rational one_third(1, 3); // Дробь 1/3
    vector<Rational> numbers;
    numbers.push_back(Rational{7, 8});
    // Следующие 2 строки эквивалентны - добавляют в numbers дробь 3/1
    numbers.push_back(Rational{3});
    numbers.push_back(3);
    Rational sum = Add(Rational{1,6}, one_third);
    // Выведет 1/2
    cout << sum.Numerator() << "/" << sum.Denominator();
} 

Перед приведением дроби к несократимому виду измените знак числителя и знаменателя, если знаменатель изначально был отрицательным. 
Затем и числитель, и знаменатель разделите на их наибольший общий делитель. Для нахождения наибольшего общего делителя в C++ есть удобная функция gcd 
из заголовочного файла <numeric>. Вы можете познакомиться с ней в документации по С++ или разработать такую функцию самостоятельно.
*/

#include <numeric>
#include <vector>
#include <iostream>

using namespace std;

class Rational {
public:
    
    Rational() = default;
    
    Rational (int numerator)
    {
        numerator_ = numerator;
        denominator_ = 1;
    }
    
     Rational (int numerator, int denominator)
    {
        numerator_ = numerator;
        denominator_ = denominator;
         
        if (denominator_ < 0) {
            numerator_ *= -1;
            denominator_ *= -1;
        }
        int GPD = gcd(numerator_, denominator_);
        numerator_ /= GPD;
        denominator_ /= GPD;
    }
    
    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
};

Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();
    // Создаём и возвращаем дробь с заданным числителем и знаменателем
    return Rational{numerator, denominator};
}
int main() {
    Rational zero;     // Дробь 0/1 = 0
    const Rational seven(7); // Дробь 7/1 = 7
    const Rational one_third(1, 3); // Дробь 1/3
    vector<Rational> numbers;
    numbers.push_back(Rational{7, 8});
    // Следующие 2 строки эквивалентны - добавляют в numbers дробь 3/1
    numbers.push_back(Rational{3});
    numbers.push_back(3);
    Rational sum = Add(Rational{1,6}, one_third);
    // Выведет 1/2
    cout << sum.Numerator() << "/" << sum.Denominator();
} 