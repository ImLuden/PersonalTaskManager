cmake_minimum_required(VERSION 3.30.2 FATAL_ERROR)

set(PROJECT_NAME PersonalTaskManager)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(external/fmt)
target_link_libraries(${PROJECT_NAME} fmt::fmt) 

add_subdirectory(external/json)
target_link_libraries(${PROJECT_NAME} json) 

# Указываем исходные файлы SQLite
set(SQLITE_SRC ${PROJECT_SOURCE_DIR}/external/sqlite/sqlite3.c)

# Добавляем библиотеку из исходников
add_library(sqlite3 STATIC external/sqlite)

# Добавляем исполняемый файл
add_executable(task_manager src/main.cpp src/sqlite_handler.cpp)

# Линкуем SQLite
target_link_libraries(task_manager PRIVATE sqlite)



