#include <iostream>
#include <string>
#include "DBManager.h"
#include "UserService.h"
#include "BookService.h"
#include "LoanService.h"
using namespace std;

int main() {
    DBManager db("library.db");
    db.initSchema();                    // 상태값 초기화
    UserService userSvc(db);
    BookService bookSvc(db);
    LoanService loanSvc(db);

    bool isAdmin = false;
    int userId = -1;

    while (true) {
        cout << "[1] Login  [2] Register  [0] Exit > " << endl;
        string cmd;
        cin >> cmd;
        if (cmd == "1") {               // Login
            string username, password;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            if (userSvc.login(username, password, userId, isAdmin)) {
                cout << "Login successful" << endl;
                break;
            } else {
                cout << "Login failed. Please try again." << endl;
            }
        } else if (cmd == "2") {        // Register
            string username, password;
            char adminChoice = 'n';

            cout << "New username: " ;
            cin >> username;
            cout << "New password: ";
            cin >> password;

            cout << "Register as admin? (Y/N): ";
            cin >> adminChoice;
            bool makeAdmin = (adminChoice == 'Y' || adminChoice == 'y');

            if (userSvc.registerUser(username, password, makeAdmin)) {
                cout << "Registration complete";
                if (makeAdmin) cout << " (admin)";
                cout << endl;
                cout << "Please login." << endl;
            } else {
                cout << "Registration failed. Username may already exist." << endl;
            }
        } else if (cmd == "0") {        // Exit
            cout << "Exiting program" << endl;
            return 0;
        } else {
            cout << "Invalid selection." << endl;
        }
    }

    // Main menu loop
    while (true) {
        if (!isAdmin) {                 // Not Admin
            cout << "[1] List Books  [2] Borrow Book  [3] My Loans  [4] Return Book  [5] Logout > " << endl;
            string sel;
            cin >> sel;
            if (sel == "1") {           // List Books
                bookSvc.listAllBooks();
            } else if (sel == "2") {    // Borrow Book
                int bookId;
                cout << "Enter book ID to borrow: ";
                cin >> bookId;
                bookSvc.borrowBook(bookId, userId);
            } else if (sel == "3") {    // My Loans
                loanSvc.listUserLoans(userId);
            } else if (sel == "4") {    // Return Book
                int bookId;
                cout << "Enter book ID to return: ";
                cin >> bookId;
                bookSvc.returnBook(bookId, userId);
            } else if (sel == "5") {    // Logout
                break;
            } else {
                cout << "Invalid selection." << endl;
            }
        } else {                        // Admin
            cout << "[1] Add Book  [2] List Users  [3] All Loans  [4] Logout > " << endl;
            string sel;
            cin >> sel;
            if (sel == "1") {           // Add Book
                string title, author;
                cout << "Book title: " ;
                // clear leftover whitespace using std::ws
                cin >> ws;
                getline(cin, title);
                cout << "Author: " ;
                getline(cin, author);
                bookSvc.addBook(title, author);
            } else if (sel == "2") {    // List Users
                userSvc.listAllUsers();
            } else if (sel == "3") {    // All Loans
                loanSvc.listAllLoans();
            } else if (sel == "4") {    // Logout
                break;
            } else {
                cout << "Invalid selection." << endl;
            }
        }
    }

    cout << "Program terminated." << endl;
    return 0;
}