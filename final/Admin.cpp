#include "Admin.h"
#include <iostream>

Admin::Admin(std::string uname, std::string pwd, std::string n)
    : User(uname, pwd, n) {}

void Admin::displayMenu()
{
  std::cout << "\n--- Admin Dashboard ---\n";
  std::cout << "1. Manage Listings\n";
  std::cout << "2. Manage Users\n";
  std::cout << "3. View All Properties\n";
  std::cout << "4. Logout\n";
}