#include "BookService.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include "sqlite3.h"

BookService::BookService(DBManager& db) : db_(db) {}

void BookService::listAllBooks() {
    string sql = "SELECT id, title, author, is_borrowed FROM books;";
    sqlite3_stmt* stmt = db_.prepare(sql);
    if (!stmt) return;

    cout << left;
    cout << setw(4) << "ID"
         << setw(22) << "Title"
         << setw(16) << "Author"
         << setw(10) << "Status" << endl;

    // 데이터
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id             = sqlite3_column_int(stmt, 0);
        const char* title  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        bool borrowed      = sqlite3_column_int(stmt, 3) == 1;

        cout << left;
        cout << setw(4) << id
             << setw(22) << title
             << setw(16) << author
             << setw(10) << (borrowed ? "Borrowed" : "Available") << endl;
    }
    sqlite3_finalize(stmt);
}

bool BookService::addBook(const string& title, const string& author) {
    string sql = "INSERT INTO books (title, author) VALUES ('"
        + title + "', '" + author + "');";
    return db_.execute(sql);
}

bool BookService::borrowBook(int bookId, int userId) {
    // 도서 상태 확인
    string checkSql = "SELECT is_borrowed FROM books WHERE id = "
        + to_string(bookId) + ";";
    sqlite3_stmt* checkStmt = db_.prepare(checkSql);
    if (!checkStmt) return false;

    bool success = false;
    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
        bool borrowed = sqlite3_column_int(checkStmt, 0) == 1;
        if (!borrowed) {
            // 책 상태 업데이트
            string updateSql = "UPDATE books SET is_borrowed = 1 WHERE id = "
                + to_string(bookId) + ";";

            // 대출 기록 삽입
            time_t t = time(nullptr);
            char buf[20];
            strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
            string loanSql = string("INSERT INTO loans (user_id, book_id, loan_date) VALUES (")
                + to_string(userId) + ", "
                + to_string(bookId) + ", '"
                + buf + "');";

            if (db_.execute(updateSql) && db_.execute(loanSql)) {
                success = true;
            }
        } else {
            cout << "Book is already borrowed.\n";
        }
    }
    sqlite3_finalize(checkStmt);
    return success;
}

bool BookService::returnBook(int bookId, int userId) {
    // 활성화된 대출 기록 찾기
    string loanCheck = string("SELECT id FROM loans WHERE book_id = ")
        + to_string(bookId)
        + " AND user_id = "
        + to_string(userId)
        + " AND return_date IS NULL;";
    sqlite3_stmt* loanStmt = db_.prepare(loanCheck);
    if (!loanStmt) return false;

    bool success = false;
    if (sqlite3_step(loanStmt) == SQLITE_ROW) {
        int loanId = sqlite3_column_int(loanStmt, 0);

        // 반납일 업데이트 및 책 상태 변경
        time_t t = time(nullptr);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));

        string returnSql = string("UPDATE loans SET return_date = '")
            + buf
            + "' WHERE id = "
            + to_string(loanId)
            + ";";
        string updateSql = string("UPDATE books SET is_borrowed = 0 WHERE id = ")
            + to_string(bookId)
            + ";";

        if (db_.execute(returnSql) && db_.execute(updateSql)) {
            success = true;
        }
    } else {
        cout << "No active loan found for this book and user.\n";
    }
    sqlite3_finalize(loanStmt);
    return success;
}