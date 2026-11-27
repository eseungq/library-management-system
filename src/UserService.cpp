#include "UserService.h"
#include <iostream>
#include <iomanip>
#include "sqlite3.h"

UserService::UserService(DBManager& db) : db_(db) {}

bool UserService::registerUser(const string& username,
                               const string& password,
                               bool isAdmin)
{
    string sql = "INSERT INTO users (username, password, is_admin) VALUES ('"
        + username + "', '" + password + "', " + to_string(isAdmin) + ");";
    return db_.execute(sql);
}

bool UserService::login(const string& username,
                        const string& password,
                        int& outUserId,
                        bool& outIsAdmin)
{
    string sql = "SELECT id, is_admin FROM users WHERE username = '"
        + username + "' AND password = '" + password + "';";
    sqlite3_stmt* stmt = db_.prepare(sql);
    if (!stmt) return false;

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        outUserId  = sqlite3_column_int(stmt, 0);
        outIsAdmin = sqlite3_column_int(stmt, 1) == 1;
        success    = true;
    }
    sqlite3_finalize(stmt);
    return success;
}

void UserService::listAllUsers() {
    string sql = "SELECT id, username, is_admin FROM users;";
    sqlite3_stmt* stmt = db_.prepare(sql);
    if (!stmt) return;

    cout << left;
    cout << setw(10) << "ID"
        << setw(15) << "Username"
        << setw(15) << "Role" << endl;

    // 데이터
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id            = sqlite3_column_int(stmt, 0);
        const char* name  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        bool admin        = sqlite3_column_int(stmt, 2) == 1;

        cout << left;
        cout << setw(10) << id
            << setw(15) << name
            << setw(15) << (admin ? "Admin" : "User") << endl;
    }
    sqlite3_finalize(stmt);
}