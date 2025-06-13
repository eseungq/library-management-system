#pragma once
#include "DBManager.h"
using namespace std;

class LoanService {
public:
    explicit LoanService(DBManager& db);
    void listUserLoans(int userId);
    void listAllLoans();

private:
    DBManager& db_;
};