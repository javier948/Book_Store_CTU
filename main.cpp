#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "crow_all.h"


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

    void placeOrder(std::vector<Book> &inventory) {
        std::string searchTitle;
        std::cout << "Enter the book title to search: ";
        std::cin.ignore();
        std::getline(std::cin, searchTitle);

        for (Book &b : inventory) {
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

    void addBook(std::vector<Book> &inventory) {
        std::string title, author, genre;
        int edition, quantity;

        std::cout << "Enter book details:\n";
        std::cout << "Title: "; std::cin >> title;
        std::cout << "Author: "; std::cin >> author;
        std::cout << "Edition: "; std::cin >> edition;
        std::cout << "Genre: "; std::cin >> genre;
        std::cout << "Quantity: "; std::cin >> quantity;

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
        for (auto &b : inventory) {
            b.displayBook();
        }
        std::cout << std::endl;
    }
};

// User Authentication
class AuthenticationSystem {
public:
    std::unordered_map<std::string, User*> users;

    void addUser(User *user) {
        users[user->getRole() + user->getUsername()] = user;
    }

    User* authenticate(std::string username, std::string password) {
        for (auto &pair : users) {
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

    std::string username, password;
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    User* loggedInUser = authSystem.authenticate(username, password);

    if (loggedInUser) {
        if (loggedInUser->getRole() == "customer") {
            std::cout << "Welcome, customer!\n";
            static_cast<Customer*>(loggedInUser)->placeOrder(inventorySystem.inventory);
        } else if (loggedInUser->getRole() == "employee") {
            std::cout << "Welcome, employee!\n";
            static_cast<Employee*>(loggedInUser)->addBook(inventorySystem.inventory);
            static_cast<Employee*>(loggedInUser)->viewSalesRecords();
        }
    } else {
        std::cout << "Authentication failed!" << std::endl;
    }

    // Display current inventory
    inventorySystem.displayInventory();

    return 0;
}              
