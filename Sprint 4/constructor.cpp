/*
Задание
Разработайте для класса Rational конструктор по умолчанию, который задаёт значение дроби, равное 0/1.

Решить задачу можно двумя способами:
написать конструктор по умолчанию и задать значения числителя и знаменателя;
задать числителю и знаменателю значения напрямую при их объявлении.
*/
#include <iostream>
using namespace std;

class Rational {
public:
    
    Rational ()
    {
        numerator_ = 0;
        denominator_ = 1;
        
    }
    
    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }
    
    void SetNumerator(int numerator) {
        numerator_ = numerator;
    }

    void SetDenominator(int denominator) {
        if (denominator != 0) {
            denominator_ = denominator;
        }
    }

private:
    int numerator_;
    int denominator_;
};

int main () {
    Rational Rational;
    cout << Rational.Numerator() << "/"s << Rational.Denominator() << endl;
}
