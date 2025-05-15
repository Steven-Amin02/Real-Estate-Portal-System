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
#include <map>
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
                    addProperty();
                else if (choice == 3)
                    adminManageUsers();
                else if (choice == 4)
                    searchProperties();
                else if (choice == 5)
                    editProperty();
                else if (choice == 6)
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
                    editProperty();
                else if (choice == 6)
                    manageProfile();
                else if (choice == 7)
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
                 << p.getSize() << ','
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
            string name, location, type, priceStr, sizeStr, features, owner, highlightStr, approveStr;
            double price, size;
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
            getline(ss, sizeStr, ',');
            size = stod(sizeStr);
            getline(ss, features, ',');
            features = unescapeCommas(features);
            getline(ss, owner, ',');
            getline(ss, highlightStr, ',');
            getline(ss, approveStr, ',');
            bool highlight = (highlightStr == "1" || highlightStr == "true");
            bool approve = (approveStr == "1" || approveStr == "true");
            properties[id] = Property(id, name, location, type, price, size, features, owner, highlight, approve);
            if (id >= nextPropertyId)
                nextPropertyId = id + 1;
        }
        propFile.close();
    }
}

void RealEstatePortal::addProperty()
{
    string name, location, type, features;
    double price, size;
    cin.ignore();
    cout << "Enter property name: ";
    getline(cin, name);
    cout << "Enter location: ";
    getline(cin, location);
    cout << "Enter type: ";
    getline(cin, type);
    cout << "Enter size (in square feet): ";
    cin >> size;
    cout << "Enter price: ";
    cin >> price;

    cout << "Enter features: ";
    cin.ignore();
    getline(cin, features);

    std::string owner = currentUser ? currentUser->getUsername() : "unknown";

    // Set isApproved to true if the current user is an admin
    bool isAdmin = (dynamic_cast<Admin *>(currentUser) != nullptr);
    bool isApproved = isAdmin ? true : false;

    properties[nextPropertyId] = Property(nextPropertyId, name, location, type, price, size, features, owner, false, isApproved);
    nextPropertyId++;

    if (isAdmin)
    {
        std::cout << "Property added and automatically approved.\n";
    }
    else
    {
        std::cout << "Property listed successfully! Awaiting admin approval.\n";
    }
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
    for (const auto &pair : properties)
    {
        const Property &p = pair.second;
        p.display();
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
    double minSize = 0, maxSize = 1e9;
    int sortChoice;

    // Clear input buffer before getting location
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter location to search (or leave empty): ";
    getline(cin, location);
    cout << "Enter type to search (or leave empty): ";
    getline(cin, type);

    cout << "Enter min size in sq ft (or 0): ";
    cin >> minSize;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        minSize = 0;
    }

    cout << "Enter max size in sq ft (or 0 for no max): ";
    cin >> maxSize;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        maxSize = 0;
    }

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
    if (maxSize == 0)
        maxSize = 1e9;

    cout << "\nSort by:\n";
    cout << "1. Price (Low to High)\n";
    cout << "2. Price (High to Low)\n";
    cout << "3. Size (Small to Large)\n";
    cout << "4. Size (Large to Small)\n";
    cout << "5. Featured First (Default)\n";
    cout << "Enter choice (1-5): ";
    cin >> sortChoice;
    if (sortChoice < 1 || sortChoice > 5)
    {
        sortChoice = 5; // Default to featured first
    }

    // First filter properties based on search criteria
    vector<const Property *> filteredProps;
    for (const auto &pair : properties)
    {
        const Property &p = pair.second;
        if (!p.isApproved())
            continue;
        if ((!location.empty() && p.getLocation() != location) ||
            (!type.empty() && p.getType() != type) ||
            p.getPrice() < minPrice || p.getPrice() > maxPrice ||
            p.getSize() < minSize || p.getSize() > maxSize)
        {
            continue;
        }
        filteredProps.push_back(&p);
    }

    // Sort based on user choice using multimap
    multimap<double, const Property *> sortedProps; // For price/size sorting
    multimap<bool, const Property *> featuredProps; // For featured sorting

    if (sortChoice == 1) // Price Low to High
    {
        for (const auto *p : filteredProps)
        {
            sortedProps.insert({p->getPrice(), p});
        }
    }
    else if (sortChoice == 2) // Price High to Low
    {
        for (const auto *p : filteredProps)
        {
            sortedProps.insert({-p->getPrice(), p}); // Negative price for reverse sorting
        }
    }
    else if (sortChoice == 3) // Size Small to Large
    {
        for (const auto *p : filteredProps)
        {
            sortedProps.insert({p->getSize(), p});
        }
    }
    else if (sortChoice == 4) // Size Large to Small
    {
        for (const auto *p : filteredProps)
        {
            sortedProps.insert({-p->getSize(), p}); // Negative size for reverse sorting
        }
    }
    else // Featured First
    {
        for (const auto *p : filteredProps)
        {
            featuredProps.insert({!p->isHighlighted(), p}); // false comes before true
        }
    }

    cout << "\n--- Search Results ---\n";
    if (sortChoice >= 1 && sortChoice <= 4)
    {
        for (const auto &pair : sortedProps)
        {
            const Property *p = pair.second;
            p->display();
        }
    }
    else // Featured First
    {
        for (const auto &pair : featuredProps)
        {
            const Property *p = pair.second;
            p->display();
        }
    }

    cout << "\nFound " << filteredProps.size() << " properties matching your criteria.\n";
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

void RealEstatePortal::editProperty()
{
    if (!currentUser)
    {
        cout << "Please login first.\n";
        return;
    }

    bool isAdmin = dynamic_cast<Admin *>(currentUser) != nullptr;
    string myUser = currentUser->getUsername();

    // Display available properties
    cout << "\n=== Available Properties for Editing ===\n";
    cout << "+-------+-----------------------------------+------------+---------+\n";

    bool hasProperties = false;

    for (const auto &pair : properties)
    {
        const Property &p = pair.second;
        // Skip if not admin and not owner
        if (!isAdmin && p.getOwnerUsername() != myUser)
            continue;

        hasProperties = true;
        p.display();
    }

    cout << "+-------+-----------------------------------+------------+---------+\n\n";

    if (!hasProperties)
    {
        if (isAdmin)
            cout << "No properties available in the system.\n";
        else
            cout << "You don't have any properties to edit.\n";
        return;
    }

    int id;
    cout << "Enter property ID to edit: ";
    cin >> id;

    auto it = properties.find(id);
    if (it == properties.end())
    {
        cout << "Property not found.\n";
        return;
    }

    Property &property = it->second;

    // Check permissions (double check in case user entered an ID manually)
    if (!isAdmin && property.getOwnerUsername() != myUser)
    {
        cout << "You can only edit your own properties.\n";
        return;
    }

    // Display current property details
    cout << "\nCurrent Property Details:\n";
    property.display();
    cout << "Status: " << (property.isApproved() ? "Approved" : "Pending Approval") << "\n\n";

    // Get new details
    string name, location, type, features;
    double price;
    cin.ignore();

    cout << "Enter new name (or press Enter to keep current): ";
    getline(cin, name);
    if (!name.empty())
    {
        property = Property(property.getId(), name, property.getLocation(),
                            property.getType(), property.getPrice(),
                            property.getSize(), property.getFeatures(),
                            property.getOwnerUsername(), property.isHighlighted(),
                            property.isApproved());
    }

    cout << "Enter new location (or press Enter to keep current): ";
    getline(cin, location);
    if (!location.empty())
    {
        property = Property(property.getId(), property.getName(), location,
                            property.getType(), property.getPrice(),
                            property.getSize(), property.getFeatures(),
                            property.getOwnerUsername(), property.isHighlighted(),
                            property.isApproved());
    }

    cout << "Enter new type (or press Enter to keep current): ";
    getline(cin, type);
    if (!type.empty())
    {
        property = Property(property.getId(), property.getName(), property.getLocation(),
                            type, property.getPrice(),
                            property.getSize(), property.getFeatures(),
                            property.getOwnerUsername(), property.isHighlighted(),
                            property.isApproved());
    }

    cout << "Enter new price (or 0 to keep current): ";
    cin >> price;
    if (price > 0)
    {
        property = Property(property.getId(), property.getName(), property.getLocation(),
                            type, price,
                            property.getSize(), property.getFeatures(),
                            property.getOwnerUsername(), property.isHighlighted(),
                            property.isApproved());
    }

    cin.ignore();
    cout << "Enter new features (or press Enter to keep current): ";
    getline(cin, features);
    if (!features.empty())
    {
        property = Property(property.getId(), property.getName(), property.getLocation(),
                            type, property.getPrice(),
                            property.getSize(), features,
                            property.getOwnerUsername(), property.isHighlighted(),
                            property.isApproved());
    }

    // If admin, allow changing approval status
    if (isAdmin)
    {
        char choice;
        cout << "Change approval status? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            property.setApproved(!property.isApproved());
        }
    }
    else
    {
        // For regular users, property needs to be re-approved after edit
        property.setApproved(false);
        cout << "Property updated and sent for admin approval.\n";
    }

    cout << "\nProperty updated successfully!\n";
    cout << "Updated Property Details:\n";
    property.display();
    cout << "Status: " << (property.isApproved() ? "Approved" : "Pending Approval") << "\n";
}
