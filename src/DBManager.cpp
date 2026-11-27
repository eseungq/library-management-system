#include "DBManager.h"
#include <iostream>

DBManager::DBManager(const string& db_file) {
    if (sqlite3_open(db_file.c_str(), &db_) != SQLITE_OK) {
        cerr << "Failed to open database: " << sqlite3_errmsg(db_) << endl;
        db_ = nullptr;
    }
}

DBManager::~DBManager() {
    if (db_) sqlite3_close(db_);
}

bool DBManager::execute(const string& sql) {
    char* err = nullptr;
    // C 문자열에 맞게 변환후 명령에 집어넣기
    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
        cerr << "SQL error: " << err << endl;
        sqlite3_free(err);
        return false;
    }
    return true;
}

sqlite3_stmt* DBManager::prepare(const string& sql) {
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare SQL error: " << sqlite3_errmsg(db_) << endl;
        return nullptr;
    }
    return stmt;
}

void DBManager::initSchema() {
    execute(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            is_admin INTEGER NOT NULL
        );
    )");
    execute(R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author TEXT,
            is_borrowed INTEGER NOT NULL DEFAULT 0
        );
    )");
    execute(R"(
        CREATE TABLE IF NOT EXISTS loans (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            book_id INTEGER NOT NULL,
            loan_date TEXT NOT NULL,
            return_date TEXT,
            FOREIGN KEY(user_id) REFERENCES users(id),
            FOREIGN KEY(book_id) REFERENCES books(id)
        );
    )");
}