#pragma once
#include <string>
#include "DBManager.h"
using namespace std;

class BookService {
public:
    explicit BookService(DBManager& db);
    void listAllBooks();
    bool addBook(const string& title, const string& author);
    bool borrowBook(int bookId, int userId);
    bool returnBook(int bookId, int userId);

private:
    DBManager& db_;
};