cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

set(CMAKE_TOOLCHAIN_FILE "F:/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(PROJECT_NAME PersonalTaskManager)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp  src/task_manager.cpp  src/task_manager.h)


find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)




