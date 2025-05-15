#include "RealEstatePortal.h"
#include "Admin.h"
#include "RegularUser.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <iomanip>
using namespace std;

RealEstatePortal::RealEstatePortal() : currentUser(nullptr), nextPropertyId(1)
{
    loadData();
}

RealEstatePortal::~RealEstatePortal()
{
    saveData();
    for (const auto &pair : users)
        delete pair.second;
}

void RealEstatePortal::run()
{
    int choice;
    while (true)
    {
        if (!currentUser)
        {
            cout << "\n--- Real Estate Portal ---\n";
            cout << "1. Register\n2. Login\n3. Exit\nChoice: ";
            cin >> choice;
            if (choice == 1)
                registerUser();
            else if (choice == 2)
                loginUser();
            else if (choice == 3)
                break;
            else
                cout << "Invalid choice.\n";
        }
        else
        {
            // Calculate statistics
            int totalUsers = users.size();
            int totalProps = properties.size();
            int pendingProps = 0;
            int approvedProps = 0;
            int highlightedProps = 0;

            for (const auto &pair : properties)
            {
                if (!pair.second.isApproved())
                    pendingProps++;
                if (pair.second.isApproved())
                    approvedProps++;
                if (pair.second.isHighlighted())
                    highlightedProps++;
            }

            currentUser->displayMenu(totalUsers, totalProps, pendingProps, approvedProps, highlightedProps);
            cout << "Choice: ";
            cin >> choice;

            if (dynamic_cast<Admin *>(currentUser))
            {
                if (choice == 1)
                    adminManageListings();
                else if (choice == 2)
                    adminManageUsers();
                else if (choice == 3)
                    searchProperties();
                else if (choice == 4)
                    logoutUser();
                else
                    cout << "Invalid choice.\n";
            }
            else
            {
                if (choice == 1)
                    addProperty();
                else if (choice == 2)
                    searchProperties();
                else if (choice == 3)
                    compareProperties();
                else if (choice == 4)
                    viewMyListings();
                else if (choice == 5)
                    manageProfile();
                else if (choice == 6)
                    logoutUser();
                else
                    cout << "Invalid choice.\n";
            }
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void RealEstatePortal::registerUser()
{
    string uname, pwd, name;
    cout << "Enter username: ";
    cin >> uname;

    if (users.find(uname) != users.end())
    {
        cout << "Username already exists.\n";
        return;
    }

    cout << "Enter password: ";
    cin >> pwd;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);

    User *newUser = new RegularUser(uname, pwd, name);
    users[uname] = newUser;
    cout << "Registration successful!\n";
}

void RealEstatePortal::loginUser()
{
    string uname, pwd;
    cout << "Enter username: ";
    cin >> uname;
    cout << "Enter password: ";
    cin >> pwd;

    auto it = users.find(uname);
    if (it != users.end() && it->second->checkPassword(pwd))
    {
        currentUser = it->second;
        cout << "Login successful!\n";
        return;
    }
    cout << "Invalid username or password.\n";
}

void RealEstatePortal::logoutUser()
{
    currentUser = nullptr;
    cout << "Logged out.\n";
}

// Helper functions for escaping/unescaping commas in features
static string escapeCommas(const string &s)
{
    string out = s;
    size_t pos = 0;
    while ((pos = out.find(',', pos)) != string::npos)
    {
        out.replace(pos, 1, "&#44;");
        pos += 5;
    }
    return out;
}
static string unescapeCommas(const string &s)
{
    string out = s;
    size_t pos = 0;
    while ((pos = out.find("&#44;", pos)) != string::npos)
    {
        out.replace(pos, 5, ",");
        pos += 1;
    }
    return out;
}

void RealEstatePortal::saveData()
{
    // Save users
    ofstream userFile("data/users.txt");
    for (const auto &pair : users)
    {
        User *u = pair.second;
        string type = dynamic_cast<Admin *>(u) ? "admin" : "user";
        userFile << type << ',' << u->getUsername() << ',' << u->getPassword() << ',' << u->getName() << '\n';
    }
    userFile.close();

    // Save properties
    ofstream propFile("data/properties.txt");
    for (const auto &pair : properties)
    {
        const Property &p = pair.second;
        propFile << p.getId() << ','
                 << escapeCommas(p.getName()) << ','
                 << escapeCommas(p.getLocation()) << ','
                 << escapeCommas(p.getType()) << ','
                 << p.getPrice() << ','
                 << escapeCommas(p.getFeatures()) << ','
                 << p.getOwnerUsername() << ','
                 << p.isHighlighted() << ',' << p.isApproved() << '\n';
    }
    propFile.close();
}

void RealEstatePortal::loadData()
{
    // Clear existing users
    for (const auto &pair : users)
    {
        delete pair.second;
    }
    users.clear();
    properties.clear();

    ifstream userFile("data/users.txt");
    if (userFile)
    {
        string line;
        while (getline(userFile, line))
        {
            stringstream ss(line);
            string type, uname, pwd, name;
            getline(ss, type, ',');
            getline(ss, uname, ',');
            getline(ss, pwd, ',');
            getline(ss, name, ',');

            User *user;
            if (type == "admin")
            {
                user = new Admin(uname, pwd, name);
            }
            else
            {
                user = new RegularUser(uname, pwd, name);
            }
            users[uname] = user;
        }
        userFile.close();
    }
    ifstream propFile("data/properties.txt");
    if (propFile)
    {
        string line;
        while (getline(propFile, line))
        {
            stringstream ss(line);
            int id;
            string name, location, type, priceStr, features, owner, highlightStr, approveStr;
            double price;
            getline(ss, priceStr, ',');
            id = stoi(priceStr);
            getline(ss, name, ',');
            name = unescapeCommas(name);
            getline(ss, location, ',');
            location = unescapeCommas(location);
            getline(ss, type, ',');
            type = unescapeCommas(type);
            getline(ss, priceStr, ',');
            price = stod(priceStr);
            getline(ss, features, ',');
            features = unescapeCommas(features);
            getline(ss, owner, ',');
            getline(ss, highlightStr, ',');
            getline(ss, approveStr, ',');
            bool highlight = (highlightStr == "1" || highlightStr == "true");
            bool approve = (approveStr == "1" || approveStr == "true");
            properties[id] = Property(id, name, location, type, price, features, owner, highlight, approve);
            if (id >= nextPropertyId)
                nextPropertyId = id + 1;
        }
        propFile.close();
    }
}

void RealEstatePortal::addProperty()
{
    string name, location, type, features;
    double price;
    cin.ignore();
    cout << "Enter property name: ";
    getline(cin, name);
    cout << "Enter location: ";
    getline(cin, location);
    cout << "Enter type: ";
    getline(cin, type);
    cout << "Enter price: ";
    cin >> price;

    cout << "Enter features: ";
    cin.ignore();
    getline(cin, features);

    string owner = currentUser ? currentUser->getUsername() : "unknown";
    properties[nextPropertyId] = Property(nextPropertyId, name, location, type, price, features, owner, false, false);
    nextPropertyId++;
    cout << "Property listed successfully! Awaiting admin approval.\n";
}

void RealEstatePortal::compareProperties()
{
    vector<const Property *> propsToCompare;
    int id, count = 0;
    cout << "Enter up to 4 property IDs to compare (0 to stop): ";
    while (count < 4)
    {
        cin >> id;
        if (id == 0)
            break;
        auto it = properties.find(id);
        if (it != properties.end() && it->second.isApproved())
        {
            propsToCompare.push_back(&it->second);
            count++;
        }
        else
        {
            cout << "Property ID " << id << " not found or not approved. Try another.\n";
        }
    }

    if (propsToCompare.empty())
    {
        cout << "No valid properties selected for comparison.\n";
        return;
    }

    // Find the maximum width needed for each field
    size_t maxNameLen = 4, maxLocLen = 8, maxTypeLen = 4, maxFeatLen = 8;
    for (const auto *prop : propsToCompare)
    {
        maxNameLen = max(maxNameLen, prop->getName().length());
        maxLocLen = max(maxLocLen, prop->getLocation().length());
        maxTypeLen = max(maxTypeLen, prop->getType().length());
        maxFeatLen = max(maxFeatLen, prop->getFeatures().length());
    }

    // Print header
    cout << "\n=== Property Comparison ===\n\n";
    cout << setw(15) << "Property ID";
    for (const auto *prop : propsToCompare)
    {
        cout << " | " << setw(5) << prop->getId();
    }
    cout << "\n"
         << string(15 + propsToCompare.size() * 8, '-') << "\n";

    // Print details
    cout << setw(15) << "Name";
    for (const auto *prop : propsToCompare)
    {
        cout << " | " << setw(5) << prop->getName();
    }
    cout << '\n';

    cout << setw(15) << "Location";
    for (const auto *prop : propsToCompare)
    {
        cout << " | " << setw(5) << prop->getLocation();
    }
    cout << '\n';

    cout << setw(15) << "Type";
    for (const auto *prop : propsToCompare)
    {
        cout << " | " << setw(5) << prop->getType();
    }
    cout << '\n';

    cout << setw(15) << "Price";
    for (const auto *prop : propsToCompare)
    {
        cout << " | $" << fixed << setprecision(1) << prop->getPrice();
    }
    cout << '\n';

    cout << setw(15) << "Features";
    for (const auto *prop : propsToCompare)
    {
        cout << " | " << setw(5) << prop->getFeatures();
    }
    cout << '\n';

    cout << setw(15) << "Highlighted";
    for (const auto *prop : propsToCompare)
    {
        cout << " | " << setw(5) << (prop->isHighlighted() ? "Yes" : "No");
    }
    cout << "\n\n";
}

void RealEstatePortal::adminManageListings()
{
    int choice;
    cout << "\n+==================== Admin Listing Management ====================+\n";
    cout << "|                      Current Properties                          |\n";
    cout << "+-------+-----------------------------------+------------+---------+\n";
    cout << "| ID    |           Property Name           |   Status   | Feature |\n";
    cout << "+-------+-----------------------------------+------------+---------+\n";

    // Calculate maximum lengths for formatting
    size_t maxNameLen = 35; // We'll truncate names longer than this

    for (const auto &pair : properties)
    {
        const Property &p = pair.second;
        string name = p.getName();
        if (name.length() > maxNameLen - 3)
        {
            name = name.substr(0, maxNameLen - 3) + "...";
        }
        else
        {
            name.append(maxNameLen - name.length(), ' ');
        }

        cout << "| " << setw(5) << p.getId() << " | "
             << name << " | "
             << setw(10) << (p.isApproved() ? "Approved" : "Pending") << " | "
             << setw(7) << (p.isHighlighted() ? "*Yes" : "No") << " |\n";
    }

    cout << "+-------+-----------------------------------+------------+---------+\n\n";
    cout << "Management Options:\n";
    cout << "+----------------------------------+\n";
    cout << "| 1. Remove Property               |\n";
    cout << "| 2. Toggle Property Highlight     |\n";
    cout << "| 3. Approve Property             |\n";
    cout << "| 4. Back to Main Menu            |\n";
    cout << "+----------------------------------+\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        int id;
        cout << "\nEnter property ID to remove: ";
        cin >> id;
        if (properties.erase(id) > 0)
        {
            cout << "(+) Property successfully removed.\n";
        }
        else
        {
            cout << "(-) Property not found.\n";
        }
    }
    else if (choice == 2)
    {
        int id;
        cout << "\nEnter property ID to toggle highlight status: ";
        cin >> id;
        auto it = properties.find(id);
        if (it != properties.end())
        {
            it->second.setHighlighted(!it->second.isHighlighted());
            cout << "(+) Property highlight status "
                 << (it->second.isHighlighted() ? "enabled" : "disabled") << ".\n";
        }
        else
        {
            cout << "(-) Property not found.\n";
        }
    }
    else if (choice == 3)
    {
        // Check if all properties are already approved
        bool allApproved = true;
        for (const auto &pair : properties)
        {
            if (!pair.second.isApproved())
            {
                allApproved = false;
                break;
            }
        }

        if (allApproved)
        {
            cout << "\n(-) All properties are already approved.\n";
            return;
        }

        int id;
        cout << "\nEnter property ID to approve: ";
        cin >> id;
        auto it = properties.find(id);
        if (it != properties.end())
        {
            if (it->second.isApproved())
            {
                cout << "(-) This property is already approved.\n";
            }
            else
            {
                it->second.setApproved(true);
                cout << "(+) Property approved successfully.\n";
            }
        }
        else
        {
            cout << "(-) Property not found.\n";
        }
    }
}

void RealEstatePortal::adminManageUsers()
{
    int choice;
    cout << "\n--- Admin User Management ---\n";
    cout << "Current Users:\n";
    for (const auto &pair : users)
    {
        const User *u = pair.second;
        string type = dynamic_cast<const Admin *>(u) ? "Admin" : "User";
        cout << "Username: " << u->getUsername() << " | Name: " << u->getName() << " | Type: " << type << endl;
    }
    cout << "1. Remove User\n2. Back\nChoice: ";
    cin >> choice;
    if (choice == 1)
    {
        string uname;
        cout << "Enter username to remove: ";
        cin >> uname;
        auto it = users.find(uname);
        if (it != users.end())
        {
            delete it->second;
            users.erase(it);
            cout << "User removed.\n";
        }
        else
        {
            cout << "User not found.\n";
        }
    }
}

void RealEstatePortal::searchProperties()
{
    string location, type;
    double minPrice = 0, maxPrice = 1e9;

    // Clear input buffer before getting location
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter location to search (or leave empty): ";
    getline(cin, location);
    cout << "Enter type to search (or leave empty): ";
    getline(cin, type);

    cout << "Enter min price (or 0): ";
    cin >> minPrice;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        minPrice = 0;
    }

    cout << "Enter max price (or 0 for no max): ";
    cin >> maxPrice;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        maxPrice = 0;
    }

    if (maxPrice == 0)
        maxPrice = 1e9;

    vector<const Property *> filteredProps;
    for (const auto &pair : properties)
    {
        const Property &p = pair.second;
        if (!p.isApproved())
            continue;
        if ((!location.empty() && p.getLocation() != location) ||
            (!type.empty() && p.getType() != type) ||
            p.getPrice() < minPrice || p.getPrice() > maxPrice)
        {
            continue;
        }
        filteredProps.push_back(&p);
    }

    cout << "\n--- Search Results ---\n";
    for (const auto *p : filteredProps)
    {
        cout << "ID: " << p->getId() << " | Name: " << p->getName()
             << " | Location: " << p->getLocation()
             << " | Type: " << p->getType()
             << " | Price: $" << fixed << setprecision(1) << p->getPrice()
             << " | Features: " << p->getFeatures()
             << " | Owner: " << p->getOwnerUsername()
             << " | Highlighted: " << (p->isHighlighted() ? "Yes" : "No") << endl;
    }
}

void RealEstatePortal::viewMyListings()
{
    cout << "\n--- My Listings ---\n";
    string myUser = currentUser ? currentUser->getUsername() : "";

    vector<const Property *> myProps;
    for (const auto &pair : properties)
    {
        const Property &p = pair.second;
        if (p.getOwnerUsername() == myUser)
        {
            myProps.push_back(&p);
        }
    }

    for (const auto *p : myProps)
    {
        p->display();
        cout << "    Status: " << (p->isApproved() ? "Approved" : "Pending Approval") << endl;
    }
}

void RealEstatePortal::manageProfile()
{
    int choice;
    string newName, newPassword, currentPassword;

    while (true)
    {
        cout << "\n=== Profile Management ===\n";
        cout << "Current Profile:\n";
        cout << "Username: " << currentUser->getUsername() << "\n";
        cout << "Name: " << currentUser->getName() << "\n";
        cout << "\nOptions:\n";
        cout << "1. Change Name\n";
        cout << "2. Change Password\n";
        cout << "3. Back to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            cout << "Enter new name: ";
            getline(cin, newName);
            if (!newName.empty())
            {
                currentUser->setName(newName);
                cout << "Name updated successfully!\n";
            }
            else
            {
                cout << "Name cannot be empty.\n";
            }
        }
        else if (choice == 2)
        {
            cout << "Enter current password: ";
            getline(cin, currentPassword);

            if (!currentUser->checkPassword(currentPassword))
            {
                cout << "Current password is incorrect.\n";
                continue;
            }

            cout << "Enter new password: ";
            getline(cin, newPassword);

            if (newPassword.empty())
            {
                cout << "Password cannot be empty.\n";
                continue;
            }

            currentUser->setPassword(newPassword);
            cout << "Password updated successfully!\n";
        }
        else if (choice == 3)
        {
            break;
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }
}
