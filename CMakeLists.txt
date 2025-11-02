cmake_minimum_required(VERSION 3.10)

project(cpp_todo_list_utility)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/file_editor.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)