/*
Задание 2. Трекер задач
Для работы над текущими задачами тимлид Матвей нанял нескольких разработчиков. Чтобы лучше следить за тем, как продвигаются работы, Матвею нужен трекер задач. 
Каждая задача привязана к определённому разработчику и проходит четыре статуса: NEW → IN_PROGRESS → TESTING → DONE. Матвей хочет быстро узнавать статистику задач 
своих коллег: сколько задач находится в каждом из статусов.
Реализуйте:
Класс TeamTasks, трекер задач. Он может хранить только статистику — сколько задач какого разработчика находится в каком статусе.
Метод GetPersonTasksInfo класса TeamTasks, принимающий имя разработчика и возвращающий статистику его задач.
Метод AddNewTask класса TeamTasks. Позволяет добавить одну задачу в статусе NEW указанному разработчику.
Метод PerformPersonTasks класса TeamTasks. Он принимает имя разработчика и число N и вызывается, когда разработчик выполнил N задач. Считается, что разработчик 
выполняет вначале задачи с самым низким статусом, и эти задачи переходят в следующий статус.
Точные сигнатуры методов описаны в заготовке кода.
Метод PerformPersonTasks принимает параметры: person — имя разработчика, task_count — количество выполненных задач. Его алгоритм таков:
Рассмотреть все невыполненные задачи разработчика person.
Упорядочить их по статусам: сначала все задачи в статусе NEW, затем все задачи в статусе IN_PROGRESS и, наконец, задачи в статусе TESTING.
Рассмотреть первые task_count задач и перевести каждую из них в следующий статус в соответствии с естественным порядком: NEW → IN_PROGRESS → TESTING → DONE.
Этот метод возвращает пару из двух элементов:
Словарь со статусами обновившихся задач: количество обновившихся задач по каждому статусу.
Информацию о невыполненных задачах — тех, статус которых не DONE и не изменился. Также в виде словаря, хранящего количество по статусу.
Для словаря, хранящего количество задач по статусам, предусмотрен псевдоним TasksInfo.
Ограничения
В словари, возвращаемые методом PerformPersonTasks, не добавляйте лишние значения, то есть статусы, имеющие ноль задач.
В методе PerformPersonTasks не обновляйте статус одной и той же задачи дважды.
Также в методе PerformPersonTasks второй элемент возвращаемого кортежа не должен содержать задач со статусом DONE.
Если разработчика с именем person нет, метод PerformPersonTasks возвращает кортеж из двух пустых TasksInfo.
Как будет тестироваться программа
Будет проверена корректность класса TeamTasks и его методов.
Гарантируется, что параметр task_count метода PerformPersonTasks — положительное число. Если task_count превышает текущее количество невыполненных задач 
разработчика, достаточно считать, что task_count равен количеству невыполненных задач. Дважды обновлять статус какой-либо задачи в этом случае не нужно.
Гарантируется, что метод GetPersonTasksInfo не будет вызван для разработчика, не имеющего задач.
Пример работы метода PerformPersonTasks
Допустим, у конкретного разработчика десять задач со следующими статусами:
NEW — 3 задачи,
IN_PROGRESS — 2 задачи,
TESTING — 4 задачи,
DONE — 1 задача.
Поступает команда PerformPersonTasks с параметром task_count, равным 4. Это означает, что нужно обновить статус c NEW до IN_PROGRESS для трёх задач и с 
IN_PROGRESS до TESTING для одной задачи. Новые статусы задач:
NEW — нет задач,
IN_PROGRESS — 4 задачи: 3 обновлённых, 1 старая,
TESTING — 5 задач: 1 обновлённая, 4 старых,
DONE — 1 старая.
В этом случае нужно вернуть кортеж из двух словарей:
Обновлённые задачи: IN_PROGRESS — 3, TESTING — 1.
Необновлённые задачи, исключая задачи со статусом DONE: IN_PROGRESS — 1, TESTING — 4.
Статусы, которым соответствует ноль задач, в словари не добавляем.
Пример ввода:
// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
        ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
        ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
        ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}
int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
  
    TasksInfo updated_tasks, untouched_tasks;
  
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
  
    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
} 
Пример вывода:
Ilia's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Ivan's tasks: 3 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 0 new tasks, 2 tasks in progress, 0 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 0 new tasks, 1 tasks in progress, 1 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 0 new tasks, 1 tasks in progress, 0 tasks are being tested, 0 tasks are done 

Обновление словаря одновременно с итерированием по нему может привести к непредсказуемым последствиям. Сначала соберите информацию об обновлениях 
в отдельном временном словаре, а потом примените их к основному словарю.
*/
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
 
// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};
 
// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;
 
 
class TeamTasks {
    map<string, TasksInfo> db;
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const std::string& person) const {
        return db.at(person);
    }
 
    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const std::string& person) {
        // Если у человека еще нет ни одной задачи
        if (db.count(person) == 0) {
            // Создаем единственную задачу
            db.insert({ person, {{TaskStatus::NEW, 1}, {TaskStatus::IN_PROGRESS, 0 }, {TaskStatus::TESTING, 0 }, {TaskStatus::DONE, 0}} });
        } else {
            // Добавляем одну новую задачу
            db.at(person).at(TaskStatus::NEW)++;
        }
    }
 
    // Обновить статусы по данному количеству задач конкретного разработчика,
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        TasksInfo updated;
        TasksInfo other;
 
        // Для всех задач от NEW до TESTING
        for (auto status = static_cast<int>(TaskStatus::NEW); status < static_cast<int>(TaskStatus::DONE); status++) {
            TaskStatus now = static_cast<TaskStatus>(status);      // Текущий статус
            TaskStatus next = static_cast<TaskStatus>(status + 1); // Следующий статус
 
            // Если нет задачь с текущим статусом, переходим к следующему статусу
            if (!db.count(person)) continue;
 
            // Пока нужно обновить статус и есть задачи с текущим статусом
            while (task_count && db.at(person).at(now)) {
                // Если еще нет обновленных задач с таким статусом
                if (!updated.count(next)) updated.insert({ next, 0 });
 
                db.at(person).at(now)--;  // Уменьшаем кол-во задач с текущим статусом
                //db.at(person).at(next)++; // Увеличиваем кол-во задач с следующим статусом
                updated.at(next)++;
                task_count--;
            }
        }
        for (auto taskInfo : db.at(person)) {
            if (taskInfo.first != TaskStatus::DONE) {
                other.insert({ taskInfo.first, taskInfo.second });
            }
        }
        // Добавляем обновившиеся задачи
        for (auto update : updated) {
            db.at(person).at(update.first) += update.second;
        }
        return std::make_tuple(updated, other);
    }
};
 
 
 
 
 
// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    std::cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
        ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
        ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
        ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
}
 
int main() {
    TeamTasks tasks;
 
    /*for (int i = 0; i < 5; i++) tasks.AddNewTask("Alice");
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    tasks.PerformPersonTasks("Alice", 5);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    tasks.PerformPersonTasks("Alice", 5);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    tasks.PerformPersonTasks("Alice", 1);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    for (int i = 0; i < 5; i++) tasks.AddNewTask("Alice");
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    tasks.PerformPersonTasks("Alice", 2);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
    tasks.PerformPersonTasks("Alice", 4);
    PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));
 
    system("pause");
    return 0;*/
 
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    std::cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    std::cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));
 
    TasksInfo updated_tasks, untouched_tasks;
 
    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 2);
    std::cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
 
    tie(updated_tasks, untouched_tasks) =
        tasks.PerformPersonTasks("Ivan", 2);
    std::cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    std::cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);
 
    system("pause");
    return 0;
}
