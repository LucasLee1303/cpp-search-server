/*
Задание
Реализуйте для класса Rational операции ввода и вывода в формате числитель/знаменатель. Гарантируется, что данные во входном потоке будут корректными.

Не забудьте в операциях ввода и вывода возвращать ссылку на поток, переданный в качестве первого аргумента. Это позволит объединить несколько операций вывода в цепочки:
cout << rational1 << " "s << rational2; 
*/

#include <numeric>
#include <iostream>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

///here; *output and input
ostream& operator<<(ostream& out, Rational& value) {
    out << value.Numerator() << "/"s << value.Denominator();
    return out;
}
 
istream& operator>>(istream& in, Rational& value) {
    int numerator, denominator;
    char slash;
    in >> numerator >> slash >> denominator;
    value = Rational{ numerator, denominator };
    return in;
}
