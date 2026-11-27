cmake_minimum_required(VERSION 3.20)
project(Library_Project C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/DBManager.cpp
        src/UserService.cpp
        src/BookService.cpp
        src/LoanService.cpp
        third_party/sqlite/sqlite3.c
)

target_include_directories(${PROJECT_NAME} PRIVATE src third_party/sqlite)
