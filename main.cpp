#include <iostream>
#include <string>
#include <windows.h>
#include "sqlite3.h"
using namespace std;

// 사용자 정보 저장 구조체
struct User {
    int id;
    string username;
    bool is_admin;
};

User* currentUser = nullptr;   // 로그인한 사용자
sqlite3* db = nullptr;         // SQLite DB 핸들

// DB 열기
bool openDatabase() {
    if (sqlite3_open("library.db", &db) != SQLITE_OK) {
        cerr << "데이터베이스 열기 실패: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}

// DB 닫기
void closeDatabase() {
    if (db)
        sqlite3_close(db);
}

// 회원가입
bool signup() {
    string username, password;
    int is_admin;

    cout << "회원가입 - 아이디 입력: ";
    cin >> username;

    // 아이디 중복 검사
    string check_sql = "SELECT COUNT(*) FROM users WHERE username = ?";
    sqlite3_stmt* check_stmt;
    sqlite3_prepare_v2(db, check_sql.c_str(), -1, &check_stmt, nullptr);
    sqlite3_bind_text(check_stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(check_stmt);
    if (rc == SQLITE_ROW && sqlite3_column_int(check_stmt, 0) > 0) {
        cout << "이미 존재하는 아이디입니다.\n";
        sqlite3_finalize(check_stmt);
        return false;
    }
    sqlite3_finalize(check_stmt);

    cout << "비밀번호 입력: ";
    cin >> password;
    cout << "관리자로 등록하시겠습니까? (0: 일반, 1: 관리자): ";
    cin >> is_admin;

    string insert_sql = "INSERT INTO users (username, password, is_admin) VALUES (?, ?, ?)";
    sqlite3_stmt* insert_stmt;
    sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &insert_stmt, nullptr);
    sqlite3_bind_text(insert_stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insert_stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(insert_stmt, 3, is_admin);

    rc = sqlite3_step(insert_stmt);
    if (rc != SQLITE_DONE) {
        cerr << "회원가입 실패: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(insert_stmt);
        return false;
    }

    sqlite3_finalize(insert_stmt);
    cout << "회원가입 성공! 로그인 해주세요.\n";
    return true;
}

// 로그인
bool login() {
    string username, password;
    cout << "아이디 입력: ";
    cin >> username;
    cout << "비밀번호 입력: ";
    cin >> password;

    string sql = "SELECT id, is_admin FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "쿼리 준비 실패\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        int is_admin = sqlite3_column_int(stmt, 1);
        currentUser = new User{id, username, static_cast<bool>(is_admin)};
        cout << "로그인 성공: " << username << "\n";
        sqlite3_finalize(stmt);
        return true;
    } else {
        cout << "로그인 실패: 아이디 또는 비밀번호 오류\n";
        sqlite3_finalize(stmt);
        return false;
    }
}

// 로그아웃
void logout() {
    if (currentUser) {
        cout << "로그아웃: " << currentUser->username << "\n";
        delete currentUser;
        currentUser = nullptr;
    } else {
        cout << "현재 로그인된 사용자가 없습니다.\n";
    }
}

// 메인 실행
int main() {
    SetConsoleOutputCP(CP_UTF8);
    // SQLite 열기
    if (!openDatabase())
        return 1;

    while (true) {
        if (!currentUser) {
            cout << "\n--- 메인 메뉴 ---\n";
            cout << "1. 로그인\n2. 회원가입\n0. 종료\n선택: ";
            int sel;
            cin >> sel;
            // 로그인
            if (sel == 1)
                login();
            // 회원가입
            else if (sel == 2)
                signup();
            // 종료
            else
                break;
            }
        else {
            cout << "\n[현재 사용자: " << currentUser->username << " | ";
            cout << (currentUser->is_admin ? "관리자" : "일반 사용자") << "]\n";
            cout << "1. 로그아웃\n0. 종료\n선택: ";
            int sel;
            cin >> sel;
            if (sel == 1) logout();
            else break;
        }
    }
    // SQLite 닫기
    closeDatabase();
    return 0;
}
