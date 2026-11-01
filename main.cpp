#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// OOP concepts used: encapsulation, abstraction, classes, objects, constructors.
class Book {
private:
    int id;
    string title;
    string author;
    bool issued;

public:
    Book() : id(0), issued(false) {}
    Book(int bookId, const string& bookTitle, const string& bookAuthor, bool isIssued = false)
        : id(bookId), title(bookTitle), author(bookAuthor), issued(isIssued) {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }
    void setIssued(bool value) { issued = value; }

    string toCSV() const {
        return to_string(id) + "|" + title + "|" + author + "|" + (issued ? "1" : "0");
    }
};

class Member {
private:
    int id;
    string name;

public:
    Member() : id(0) {}
    Member(int memberId, const string& memberName) : id(memberId), name(memberName) {}
    int getId() const { return id; }
    string getName() const { return name; }
    string toCSV() const { return to_string(id) + "|" + name; }
};

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    int nextBookId = 1;
    int nextMemberId = 1;
    const string booksFile = "books.txt";
    const string membersFile = "members.txt";

    static vector<string> split(const string& line, char delimiter) {
        vector<string> parts;
        string part;
        stringstream stream(line);
        while (getline(stream, part, delimiter)) parts.push_back(part);
        return parts;
    }

    void loadBooks() {
        ifstream file(booksFile);
        string line;
        while (getline(file, line)) {
            auto p = split(line, '|');
            if (p.size() != 4) continue;
            try {
                int id = stoi(p[0]);
                books.emplace_back(id, p[1], p[2], p[3] == "1");
                nextBookId = max(nextBookId, id + 1);
            } catch (...) {}
        }
    }

    void loadMembers() {
        ifstream file(membersFile);
        string line;
        while (getline(file, line)) {
            auto p = split(line, '|');
            if (p.size() != 2) continue;
            try {
                int id = stoi(p[0]);
                members.emplace_back(id, p[1]);
                nextMemberId = max(nextMemberId, id + 1);
            } catch (...) {}
        }
    }

    void saveBooks() const {
        ofstream file(booksFile);
        for (const auto& book : books) file << book.toCSV() << '\n';
    }

    void saveMembers() const {
        ofstream file(membersFile);
        for (const auto& member : members) file << member.toCSV() << '\n';
    }

    Book* findBook(int id) {
        for (auto& book : books) if (book.getId() == id) return &book;
        return nullptr;
    }

    bool memberExists(int id) const {
        for (const auto& member : members) if (member.getId() == id) return true;
        return false;
    }

    void pause() const {
        cout << "\nPress Enter to continue...";
        cin.ignore(10000, '\n');
        cin.get();
    }

public:
    Library() { loadBooks(); loadMembers(); }

    void addBook() {
        string title, author;
        cout << "Book title: ";
        getline(cin >> ws, title);
        cout << "Author: ";
        getline(cin, author);
        books.emplace_back(nextBookId++, title, author);
        saveBooks();
        cout << "\nBook added successfully. Book ID: " << books.back().getId() << '\n';
    }

    void showBooks() const {
        cout << "\n" << left << setw(8) << "ID" << setw(32) << "TITLE" << setw(25) << "AUTHOR" << "STATUS\n";
        cout << string(78, '-') << '\n';
        if (books.empty()) { cout << "No books in the library yet.\n"; return; }
        for (const auto& book : books) {
            cout << left << setw(8) << book.getId()
                 << setw(32) << book.getTitle().substr(0, 30)
                 << setw(25) << book.getAuthor().substr(0, 23)
                 << (book.isIssued() ? "Issued" : "Available") << '\n';
        }
    }

    void searchBook() const {
        string query;
        cout << "Enter title or author to search: ";
        getline(cin >> ws, query);
        transform(query.begin(), query.end(), query.begin(), [](unsigned char c){ return static_cast<char>(tolower(c)); });
        bool found = false;
        for (const auto& book : books) {
            string title = book.getTitle(), author = book.getAuthor();
            transform(title.begin(), title.end(), title.begin(), [](unsigned char c){ return static_cast<char>(tolower(c)); });
            transform(author.begin(), author.end(), author.begin(), [](unsigned char c){ return static_cast<char>(tolower(c)); });
            if (title.find(query) != string::npos || author.find(query) != string::npos) {
                cout << "\nID: " << book.getId() << " | " << book.getTitle()
                     << " | " << book.getAuthor() << " | "
                     << (book.isIssued() ? "Issued" : "Available") << '\n';
                found = true;
            }
        }
        if (!found) cout << "No matching book found.\n";
    }

    void registerMember() {
        string name;
        cout << "Member name: ";
        getline(cin >> ws, name);
        members.emplace_back(nextMemberId++, name);
        saveMembers();
        cout << "Member registered successfully. Member ID: " << members.back().getId() << '\n';
    }

    void showMembers() const {
        cout << "\nMEMBERS\n" << string(40, '-') << '\n';
        if (members.empty()) { cout << "No members registered yet.\n"; return; }
        for (const auto& member : members)
            cout << "ID: " << member.getId() << " | Name: " << member.getName() << '\n';
    }

    void issueBook() {
        int bookId, memberId;
        cout << "Book ID to issue: ";
        if (!(cin >> bookId)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid ID.\n"; return; }
        cout << "Member ID: ";
        if (!(cin >> memberId)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid ID.\n"; return; }
        Book* book = findBook(bookId);
        if (!book) { cout << "Book not found.\n"; return; }
        if (!memberExists(memberId)) { cout << "Member not found. Register the member first.\n"; return; }
        if (book->isIssued()) { cout << "This book is already issued.\n"; return; }
        book->setIssued(true);
        saveBooks();
        cout << "Book issued successfully to member " << memberId << ".\n";
    }

    void returnBook() {
        int bookId;
        cout << "Book ID to return: ";
        if (!(cin >> bookId)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid ID.\n"; return; }
        Book* book = findBook(bookId);
        if (!book) { cout << "Book not found.\n"; return; }
        if (!book->isIssued()) { cout << "This book is already available.\n"; return; }
        book->setIssued(false);
        saveBooks();
        cout << "Book returned successfully.\n";
    }

    void run() {
        int choice;
        do {
            cout << "\n====================================\n"
                 << "     LIBRARY MANAGEMENT SYSTEM\n"
                 << "        C++ OOP MINI PROJECT\n"
                 << "====================================\n"
                 << "1. Add a book\n"
                 << "2. View all books\n"
                 << "3. Search books\n"
                 << "4. Register a member\n"
                 << "5. View members\n"
                 << "6. Issue a book\n"
                 << "7. Return a book\n"
                 << "0. Save and exit\n"
                 << "Choose an option: ";
            if (!(cin >> choice)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Please enter a number from the menu.\n";
                continue;
            }
            switch (choice) {
                case 1: addBook(); break;
                case 2: showBooks(); break;
                case 3: searchBook(); break;
                case 4: registerMember(); break;
                case 5: showMembers(); break;
                case 6: issueBook(); break;
                case 7: returnBook(); break;
                case 0: saveBooks(); saveMembers(); cout << "Data saved. Goodbye!\n"; break;
                default: cout << "Invalid choice. Try again.\n";
            }
            if (choice != 0) pause();
        } while (choice != 0);
    }
};

int main() {
    Library library;
    library.run();
    return 0;
}
