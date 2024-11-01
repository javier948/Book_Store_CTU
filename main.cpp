#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "httplib.h"

// Book class
class Book {
public:
    std::string title, author, genre;
    int edition, quantity;

    Book(std::string t, std::string a, int e, std::string g, int q)
        : title(t), author(a), edition(e), genre(g), quantity(q) {}

    void displayBook() {
        std::cout << "Title: " << title << "\nAuthor: " << author
            << "\nEdition: " << edition << "\nGenre: " << genre
            << "\nQuantity: " << quantity << std::endl;
    }
};

// User class (Base Class for Customer and Employee)
class User {
protected:
    std::string username;
    std::string password;
    std::string role; // "customer" or "employee"

public:
    User(std::string u, std::string p, std::string r) : username(u), password(p), role(r) {}

    virtual bool login(std::string u, std::string p) = 0; // Pure virtual function

    std::string getRole() { return role; }

    std::string getUsername() { return username; } // Getter for username
};

// Customer class (Inherits from User)
class Customer : public User {
public:
    Customer(std::string u, std::string p) : User(u, p, "customer") {}

    bool login(std::string u, std::string p) override {
        return username == u && password == p;
    }

    void placeOrder(std::vector<Book>& inventory, const std::string& searchTitle) {
        for (Book& b : inventory) {
            if (b.title == searchTitle && b.quantity > 0) {
                std::cout << "Book found! Processing your order...\n";
                b.quantity--;
                std::cout << "Order placed for: " << b.title << std::endl;
                return;
            }
        }
        std::cout << "Book not available in inventory." << std::endl;
    }
};

// Employee class (Inherits from User)
class Employee : public User {
public:
    Employee(std::string u, std::string p) : User(u, p, "employee") {}

    bool login(std::string u, std::string p) override {
        return username == u && password == p;
    }

    void addBook(std::vector<Book>& inventory, const std::string& title, const std::string& author, int edition, const std::string& genre, int quantity) {
        Book newBook(title, author, edition, genre, quantity);
        inventory.push_back(newBook);
        std::cout << "Book added successfully!" << std::endl;
    }

    void viewSalesRecords() {
        // Simulating sales records for simplicity
        std::cout << "Sales Records:\n";
        std::cout << "Customer: John Doe, Book: C++ Programming, Date: 2024-10-16\n";
        std::cout << "Customer: Jane Smith, Book: Data Structures, Date: 2024-10-15\n";
    }
};

// Inventory Management System
class InventorySystem {
public:
    std::vector<Book> inventory;

    void displayInventory() {
        std::cout << "\n--- Inventory ---\n";
        for (auto& b : inventory) {
            b.displayBook();
        }
        std::cout << std::endl;
    }
};

// User Authentication
class AuthenticationSystem {
public:
    std::unordered_map<std::string, User*> users;

    void addUser(User* user) {
        users[user->getRole() + user->getUsername()] = user;
    }

    User* authenticate(std::string username, std::string password) {
        for (auto& pair : users) {
            if (pair.second->login(username, password)) {
                return pair.second;
            }
        }
        return nullptr;
    }
};

// Main function
int main() {
    InventorySystem inventorySystem;
    AuthenticationSystem authSystem;

    // Sample inventory
    inventorySystem.inventory.push_back(Book("C++ Programming", "Bjarne Stroustrup", 4, "Programming", 10));
    inventorySystem.inventory.push_back(Book("Data Structures", "Mark Allen Weiss", 3, "Computer Science", 5));

    // Add users
    Customer customer1("john_doe", "password123");
    Employee employee1("admin_user", "adminpass");

    authSystem.addUser(&customer1);
    authSystem.addUser(&employee1);

    httplib::Server svr;

    svr.Post("/login", [&](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_param_value("username");
        auto password = req.get_param_value("password");

        User* loggedInUser = authSystem.authenticate(username, password);

        if (loggedInUser) {
            if (loggedInUser->getRole() == "customer") {
                res.set_content("Welcome, customer!", "text/plain");
            }
            else if (loggedInUser->getRole() == "employee") {
                res.set_content("Welcome, employee!", "text/plain");
            }
        }
        else {
            res.set_content("Authentication failed!", "text/plain");
        }
        });

    svr.Post("/place_order", [&](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_param_value("username");
        auto password = req.get_param_value("password");
        auto title = req.get_param_value("title");

        User* loggedInUser = authSystem.authenticate(username, password);

        if (loggedInUser && loggedInUser->getRole() == "customer") {
            static_cast<Customer*>(loggedInUser)->placeOrder(inventorySystem.inventory, title);
            res.set_content("Order placed for: " + title, "text/plain");
        }
        else {
            res.set_content("Authentication failed or not a customer!", "text/plain");
        }
        });

    svr.Post("/add_book", [&](const httplib::Request& req, httplib::Response& res) {
        auto username = req.get_param_value("username");
        auto password = req.get_param_value("password");
        auto title = req.get_param_value("title");
        auto author = req.get_param_value("author");
        auto edition = std::stoi(req.get_param_value("edition"));
        auto genre = req.get_param_value("genre");
        auto quantity = std::stoi(req.get_param_value("quantity"));

        User* loggedInUser = authSystem.authenticate(username, password);

        if (loggedInUser && loggedInUser->getRole() == "employee") {
            static_cast<Employee*>(loggedInUser)->addBook(inventorySystem.inventory, title, author, edition, genre, quantity);
            res.set_content("Book added successfully!", "text/plain");
        }
        else {
            res.set_content("Authentication failed or not an employee!", "text/plain");
        }
        });

    svr.listen("localhost", 8080);

    return 0;
}

