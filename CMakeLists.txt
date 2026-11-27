cmake_minimum_required(VERSION 3.31)
project(Library_Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
        sqlite3.c
        main.cpp
        DBManager.cpp
        UserService.cpp
        BookService.cpp
        LoanService.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
