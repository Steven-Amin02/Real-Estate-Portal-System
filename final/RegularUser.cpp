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
    cout << "1. Add a Property\n";
    cout << "2. Search Properties\n";
    cout << "3. Compare Properties\n";
    cout << "4. View My Listings\n";
    cout << "5. Edit Property\n";
    cout << "6. Manage Profile\n";
    cout << "7. Logout\n";
}
