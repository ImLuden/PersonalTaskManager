#ifndef TASK_MANAGER_H  // Защита от двойного включения
#define TASK_MANAGER_H

#include <string>       // Подключаем стандартную библиотеку для работы со строками
#include <vector>       // Подключаем стандартную библиотеку для работы с векторами
#include <sqlite3.h>    // Подключаем заголовочный файл SQLite для работы с базой данных

// Определение структуры Task
struct Task {
    int id;                     // Уникальный идентификатор задачи
    std::string title;          // Название задачи
    std::string description;    // Описание задачи
    std::string due_date;       // Срок выполнения задачи
    bool completed;             // Флаг завершенности задачи

    std::string to_string() const; // Метод для преобразования задачи в строку (для отображения в консоли)
};

// Класс для управления задачами
class TaskManager {
public:
    // Конструктор и деструктор
    TaskManager(const std::string& db_file);  // Открытие базы данных и инициализация
    ~TaskManager();                           // Закрытие базы данных при уничтожении объекта

    // Методы управления задачами
    int add_task(const std::string& title, const std::string& description, const std::string& due_date);  
    void complete_task(int task_id); // Завершение задачи
    void delete_task(int task_id);   // Удаление задачи
    std::vector<Task> get_all_tasks(); // Получение списка всех задач

private:
    sqlite3* db;           // Указатель на базу данных SQLite
    std::string db_file;   // Путь к файлу базы данных

    void execute_sql(const std::string& sql); // Вспомогательный метод для выполнения SQL-запросов
    static int callback(void* data, int argc, char** argv, char** col_names); // Статическая функция обратного вызова для обработки SQL-результатов
};

#endif // TASK_MANAGER_H
