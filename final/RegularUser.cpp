#include "RegularUser.h"
#include <iostream>

using namespace std;

RegularUser::RegularUser(std::string uname, std::string pwd, std::string n)
    : User(uname, pwd, n)
{
}

void RegularUser::displayMenu(int totalUsers, int totalProps, int pendingProps, int approvedProps, int highlightedProps)
{
    cout << "\n--- User Menu ---\n";
    cout << "1. List a Property\n";
    cout << "2. Search Properties\n";
    cout << "3. Compare Properties\n";
    cout << "4. View My Listings\n";
    cout << "5. Manage Profile\n";
    cout << "6. Logout\n";
}
