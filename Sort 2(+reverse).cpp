/*Задание 1
Считайте количество людей, имя и возраст каждого. Выведите все имена с новой строки по старшинству — от старых к молодым.
Вектор пар сортируется лексикографически. Например, у вас есть вектор:
vector<pair<int, double>> values = {{5, -1.1}, {5, -1.0}, {1, 0.0}}; 
После сортировки значения будут в таком порядке:
{1, 0.0}
{5, -1.1} // 1 < 5
{5, -1.0}, // 5 == 5, -1.1 < -1.0 */

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    int count;
        
    vector<pair<int, string>> Vname_age;
 
        
    cin >> count;
    for (int i = 0; i < count; ++i) 
    {
        string name;
        int age;
        cin >> name >> age;
        // сохраните в вектор пар
        Vname_age.push_back (make_pair(age, name));
    }
    
    sort (Vname_age.begin(), Vname_age.end());
    
    reverse(Vname_age.begin(), Vname_age.end());
    
    for (const auto& element: Vname_age) 
    {
        cout << element.second << endl;
    }
   
 
   

    // выведите только имена в порядке убывания возраста
    // Jack
    // John
    // ...
}
