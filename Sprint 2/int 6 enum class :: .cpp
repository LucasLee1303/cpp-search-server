/*
Задание
Научитесь работать с перечислимым типом, объявленным внутри класса. Для этого перенесите объявление DocumentStatus в публичную секцию класса SearchServer, 
а в функции main выведите численное значение статуса BANNED.

Используйте двойное двоеточие дважды: SearchServer::DocumentStatus::BANNED. Не забудьте о static_cast.
*/
#include <iostream>

using namespace std;

class SearchServer {
public:
    // перенесите сюда DocumentStatus
    enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};
};

int main() {
    // выведите численное значение статуса BANNED
    //SearchServer::DocumentStatus::BANNED;
    
    cout << static_cast <int> (SearchServer::DocumentStatus::BANNED);
    
    return 0;
}
