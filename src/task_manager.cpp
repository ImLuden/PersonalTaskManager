#include "task_manager.h"   // Подключаем заголовочный файл с объявлением классов и методов
#include <iostream>         // Для вывода информации об ошибках
#include <sstream>          // Для форматирования строки
#include <stdexcept>        // Для обработки исключений (ошибок)


// Конструктор - открываем базу данных SQLite
TaskManager::TaskManager(const std::string& db_file) : db_file(db_file) 
{
    //std::cout << "Попытка открыть базу данных: " << db_file << std::endl;

    if (sqlite3_open(db_file.c_str(), &db) != SQLITE_OK) 
    {
        throw std::runtime_error("Database opening error ");
    }
    std::cout << "База данных успешно открыта" << std::endl;

    // Создаем таблицу задач, если она еще не существует
    std::string sql = "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "description TEXT,"
        "due_date TEXT,"
        "completed INTEGER DEFAULT 0);";
    execute_sql(sql);
}

// Деструктор - закрываем базу данных
TaskManager::~TaskManager() {
    sqlite3_close(db);
}

// Метод для выполнения SQL-запросов
void TaskManager::execute_sql(const std::string& sql) {
    char* err_msg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::string error = "SQL error: ";
        error += err_msg;
        sqlite3_free(err_msg);
        throw std::runtime_error(error);
    }
}

// Добавление новой задачи
//void TaskManager::add_task(const std::string& title, const std::string& description, const std::string& due_date) {
//    std::string sql = "INSERT INTO tasks (title, description, due_date) VALUES ('" +
//        title + "', '" + description + "', '" + due_date + "');";
//    execute_sql(sql);
//}

int TaskManager::add_task(const std::string& title, const std::string& description, const std::string& due_date) 
{
    std::string sql = "INSERT INTO tasks (title, description, due_date) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    // Подготавливаем SQL-запрос
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Ошибка при подготовке SQL-запроса INSERT: " + std::string(sqlite3_errmsg(db)));
    }

    // Связываем параметры с запросом
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, due_date.c_str(), -1, SQLITE_STATIC);

    // Выполняем запрос
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Ошибка при выполнении SQL-запроса INSERT: " + std::string(sqlite3_errmsg(db)));
    }

    // Получаем ID последней вставленной задачи
    int task_id = sqlite3_last_insert_rowid(db);
    std::cout << "Новая задача добавлена с ID: " << task_id << std::endl;

    // Освобождаем ресурсы
    sqlite3_finalize(stmt);

    return task_id;  // Возвращаем ID новой задачи
}

// Завершение задачи
void TaskManager::complete_task(int task_id) {
    std::string sql = "UPDATE tasks SET completed = 1 WHERE id = " + std::to_string(task_id) + ";";
    execute_sql(sql);
}

// Удаление задачи
//void TaskManager::delete_task(int task_id) {
//    std::string sql = "DELETE FROM tasks WHERE id = " + std::to_string(task_id) + ";";
//    execute_sql(sql);
//}

// Метод для удаления задачи по её ID
void TaskManager::delete_task(int task_id) {
    std::string sql = "DELETE FROM tasks WHERE id = ?;";
    sqlite3_stmt* stmt;

    // Подготавливаем SQL-запрос
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Ошибка при подготовке SQL-запроса DELETE: " + std::string(sqlite3_errmsg(db)));
    }

    // Связываем task_id с первым параметром запроса
    sqlite3_bind_int(stmt, 1, task_id);

    // Выполняем запрос
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Ошибка при выполнении SQL-запроса DELETE: " + std::string(sqlite3_errmsg(db)));
    }

    std::cout << "Задача с ID " << task_id << " успешно удалена." << std::endl;

    // Освобождаем ресурсы
    sqlite3_finalize(stmt);
}

// Получение всех задач
std::vector<Task> TaskManager::get_all_tasks() {
    std::string sql = "SELECT * FROM tasks;";
    std::vector<Task> tasks;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQL Task preparation error");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(stmt, 0);
        task.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        task.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        task.due_date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        task.completed = sqlite3_column_int(stmt, 4);
        tasks.push_back(task);
    }

    sqlite3_finalize(stmt);
    return tasks;
}

// Метод Task для представления задачи в виде строки
std::string Task::to_string() const {
    std::ostringstream oss;
    oss << "[" << (completed ? "X" : " ") << "] " << title
        << " (Due: " << due_date << ") - " << description;
    return oss.str();
}