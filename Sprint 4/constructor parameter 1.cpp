/*
Задание 1
Разработайте для представленного класса Animal параметризованный конструктор, который позволяет задать тип и кличку животного и имя хозяина.

Добавьте конструктор, принимающий параметры type, name и owner_name. Используйте их для полей type_, name_ и owner_name_.
*/
#include <string>

using namespace std;

enum class AnimalType {
    Cat,
    Dog,
    Mouse,
};

class Animal {
public:
    Animal () = default; //here
    
    Animal (AnimalType type, string name, string owner_name) ///here
    {
        type_ = type;
        name_ = name;
        owner_name_ = owner_name;
    }
    
        AnimalType GetType() const {
        return type_;
    }

    const string& GetName() const {
        return name_;
    }

    const string& GetOwnerName() const {
        return owner_name_;
    }

private:
    AnimalType type_;
    string name_;
    string owner_name_;
};
