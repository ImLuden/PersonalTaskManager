cmake_minimum_required(VERSION 3.30 FATAL_ERROR)


# Указываем путь к toolchain-файлу vcpkg
set(CMAKE_TOOLCHAIN_FILE "F:/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(PROJECT_NAME PersonalTaskManager)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp  src/task_manager.cpp  src/task_manager.h)

add_subdirectory(external/fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)



# Добавляем SQLite в проект
find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)




