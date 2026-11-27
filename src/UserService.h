#pragma once
#include <string>
#include "DBManager.h"
using namespace std;

struct User {
    int id;
    string username;
    bool isAdmin;
};

class UserService {
public:
    explicit UserService(DBManager& db);
    bool registerUser(const string& username, const string& password, bool isAdmin);
    bool login(const string& username, const string& password, int& outUserId, bool& outIsAdmin);
    void listAllUsers();

private:
    DBManager& db_;
};