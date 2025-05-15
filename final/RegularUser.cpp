#include "RegularUser.h"
#include <iostream>

RegularUser::RegularUser(std::string uname, std::string pwd, std::string n)
    : User(uname, pwd, n) {
}

void RegularUser::displayMenu()
{
    std::cout << "\n--- User Menu ---\n";
    std::cout << "1. List a Property\n";
    std::cout << "2. Search Properties\n";
    std::cout << "3. Compare Properties\n";
    std::cout << "4. View My Listings\n";
    std::cout << "5. Logout\n";
}
