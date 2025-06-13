#include "LoanService.h"
#include <iostream>
#include <iomanip>
#include "sqlite3.h"

LoanService::LoanService(DBManager& db) : db_(db) {}

// 대출 기록 출력
void LoanService::listUserLoans(int userId) {
    string sql = "SELECT l.id, b.title, l.loan_date, l.return_date "
                      "FROM loans l "
                      "JOIN books b ON l.book_id = b.id "
                      "WHERE l.user_id = " + to_string(userId) + ";";
    sqlite3_stmt* stmt = db_.prepare(sql);
    if (!stmt) return;

    // 틀 출력
    cout << left;
    cout << setw(10) << "LoanID"
        << setw(22) << "Title"
        << setw(15) << "LoanDate"
        << setw(15) << "ReturnDate" << endl;

    // 리스트 출력
    while (sqlite3_step(stmt) == SQLITE_ROW) {              // 한줄씩 가지고 오기
        int id             = sqlite3_column_int(stmt, 0);   // n번째 열을 데이터로 가지고 옴
        const char* title  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* loanDate   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* returnDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        cout << left;
        cout << setw(10) << id
            << setw(22) << title
            << setw(15) << loanDate
            << setw(15) << (returnDate ? returnDate : "-") << endl;
    }
    sqlite3_finalize(stmt);
}

void LoanService::listAllLoans() {
    string sql = "SELECT l.id, u.username, b.title, l.loan_date, l.return_date "
                    "FROM loans l "
                    "JOIN users u ON l.user_id = u.id "
                    "JOIN books b ON l.book_id = b.id;";
    sqlite3_stmt* stmt = db_.prepare(sql);
    if (!stmt) return;

    // 헤더
    cout << left;
    cout << setw(10) << "LoanID"
        << setw(15) << "User"
        << setw(22) << "Title"
        << setw(15) << "LoanDate"
        << setw(15) << "ReturnDate" << endl;

    // 데이터
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id             = sqlite3_column_int(stmt, 0);
        const char* user   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* title  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* loanDate   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* returnDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        cout << left;
        cout << setw(10) << id
            << setw(15) << user
            << setw(22) << title
            << setw(15) << loanDate
            << setw(15) << (returnDate ? returnDate : "-") << endl;
    }
    sqlite3_finalize(stmt);
}