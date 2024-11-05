
#include "task_manager.h"
#include<iostream>
#include<fmt/core.h>
#include <codecvt> // codecvt_utf8
#include <locale>  // wstring_convert
#include <sqlite3.h>
#include <nlohmann/json.hpp>
#include <windows.h> // для кодировки в консоли


//using json = nlohmann::json;
//// encoding function (json test)
//std::string to_utf8(std::wstring& wide_string)
//{
//    static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
//    return utf8_conv.to_bytes(wide_string);
//}
//
//int main()
//{
//
//
//    json j;
//    std::wstring ws = L"車B1234 こんにちは";
//
//    j["original"] = ws;
//    j["encoded"] = to_utf8(ws);
//
//    std::cout << j << std::endl;
//    std::cout << "Hello, World" << std::endl;
//    fmt::print("Fmt test");
//    return 0;
//}



int main() {
    SetConsoleCP(CP_UTF8);  // Для ввода
    SetConsoleOutputCP(CP_UTF8);  // Для вывода
    TaskManager manager("F:/PersonalTaskManager/data/tasks.db");

    //manager.add_task("Пункт 1", "Сделать нормальный ввод в консоль", "2024-10-15");
    //manager.add_task("New Task", "Console input", "2024-10-15");
    manager.delete_task(5);

    std::cout << "Список задач:\n";
    for (const auto& task : manager.get_all_tasks()) 
    {
        std::cout << task.to_string() << std::endl;
    }

    return 0;
}