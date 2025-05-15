#include "Admin.h"
#include <iostream>
#include <iomanip>

using namespace std;

Admin::Admin(std::string uname, std::string pwd, std::string n)
    : User(uname, pwd, n)
{
}

void Admin::displayMenu(int totalUsers, int totalProps, int pendingProps, int approvedProps, int highlightedProps)
{
    cout << "\n=== Admin Dashboard ===\n";
    cout << "Welcome, " << getName() << "!\n\n";

    // Table formatting
    const int labelWidth = 24;
    const int valueWidth = 5;
    cout << "Portal Statistics:\n";
    cout << "+" << string(labelWidth + valueWidth + 3, '-') << "+\n";
    cout << "| " << left << setw(labelWidth) << "Total Users:" << " " << right << setw(valueWidth) << totalUsers << " |\n";
    cout << "| " << left << setw(labelWidth) << "Total Properties:" << " " << right << setw(valueWidth) << totalProps << " |\n";
    cout << "| " << left << setw(labelWidth) << "Pending Properties:" << " " << right << setw(valueWidth) << pendingProps << " |\n";
    cout << "| " << left << setw(labelWidth) << "Approved Properties:" << " " << right << setw(valueWidth) << approvedProps << " |\n";
    cout << "| " << left << setw(labelWidth) << "Highlighted Properties:" << " " << right << setw(valueWidth) << highlightedProps << " |\n";
    cout << "+" << string(labelWidth + valueWidth + 3, '-') << "+\n\n";

    cout << "Menu Options:\n";
    cout << "1. Manage Listings\n";
    cout << "2. Add a Property\n";
    cout << "3. Manage Users\n";
    cout << "4. View All Properties\n";
    cout << "5. Edit Property\n";
    cout << "6. Logout\n";
}
