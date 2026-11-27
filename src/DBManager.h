#pragma once
#include <string>
#include "sqlite3.h"
using namespace std;

class DBManager {
public:
    explicit DBManager(const string& db_file);
    ~DBManager();

    bool execute(const string& sql);
    sqlite3_stmt* prepare(const string& sql);
    void initSchema();

private:
    sqlite3* db_ = nullptr;
};
