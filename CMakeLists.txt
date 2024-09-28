cmake_minimum_required(VERSION 3.30.2 FATAL_ERROR)

set(PROJECT_NAME PersonalTaskManager)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(external/fmt)
target_link_libraries(${PROJECT_NAME} fmt::fmt) 




