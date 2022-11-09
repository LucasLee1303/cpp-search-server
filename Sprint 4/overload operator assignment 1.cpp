/*
Задание 1
Реализуйте для класса Rational операции присваивания +=, -=, *= и /=. 
После выполнения этих операций результирующая дробь должна приводиться к несократимому виду с положительным знаменателем. 
Входные данные программы исключают деление на ноль.

Объявите присваивающие выражения внутри класса Rational, как в примере с классом Vector2D. 
Убедитесь, что они модифицируют текущий объект и возвращают ссылку на него командой return *this;. 
Перед выходом из операции не забудьте нормализовать дробь. Для этого выделите код нормализации дроби в приватный метод Normalize(). 
Вызывайте его в этих операциях перед return *this;.
*/
#include <iostream>
#include <numeric>
#include <string>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int value)
        : numerator_(value)
        , denominator_(1)
    {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator)
    {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }
    
    ///here
    Rational& operator+=(Rational right) {
        numerator_ = numerator_ * right.denominator_ + denominator_ * right.numerator_;
        denominator_ = denominator_ * right.denominator_;
        Normalize();
        return *this;
    }
    
    Rational& operator-=(Rational right) {
        numerator_ = numerator_ * right.denominator_ - denominator_ * right.numerator_;
        denominator_ = denominator_ * right.denominator_;
        Normalize();
        return *this;
    }
    
     Rational& operator*=(Rational right) {
        numerator_ *= right.numerator_;
        denominator_ *= right.denominator_;
        Normalize();
        return *this;
    }
 
    Rational& operator/=(Rational right) {
        numerator_ *= right.denominator_;
        denominator_ *= right.numerator_;
        Normalize();
        return *this;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int n = gcd(numerator_, denominator_);
        numerator_ /= n;
        denominator_ /= n;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& output, Rational rational) {
    return output << rational.Numerator() << '/' << rational.Denominator();
}

istream& operator>>(istream& input, Rational& rational) {
    int numerator;
    int denominator;
    char slash;
    if ((input >> numerator) && (input >> slash) && (slash == '/') && (input >> denominator)) {
        rational = Rational{numerator, denominator};
    }
    return input;
}

Rational operator+(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() + right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();

    return {numerator, denominator};
}

Rational operator-(Rational left, Rational right) {
    const int numerator = left.Numerator() * right.Denominator() - right.Numerator() * left.Denominator();
    const int denominator = left.Denominator() * right.Denominator();

    return {numerator, denominator};
}

Rational operator+(Rational value) {
    return value;
}

Rational operator-(Rational value) {
    return {-value.Numerator(), value.Denominator()};
}