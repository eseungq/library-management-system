#include <iostream>
#include <windows.h>
#include "sqlite3.h"
using namespace std;

int main() {
    // 한글 입력 UTF 설정
    SetConsoleOutputCP(CP_UTF8);
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db); // test.db라는 SQLite 파일 열기

    // SQLite 열기 실패
    if (rc != SQLITE_OK) {
        cerr << "SQLite 열기 실패: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    // SQLite 열기 성공
    cout << "SQLite 열기 성공!" << endl;

    // 간단한 쿼리 실행: 테이블 생성
    const char* sql = "CREATE TABLE IF NOT EXISTS test_table ("
    "id INTEGER PRIMARY KEY,"
    " name TEXT);";
    char* errMsg = nullptr;

    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "테이블 생성 실패: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    else {
        cout << "테이블 생성 성공!" << endl;
    }

    sqlite3_close(db);
    return 0;
}
