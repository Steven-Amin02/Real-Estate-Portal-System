#include "RealEstatePortal.h"
#include "Admin.h"
#include "RegularUser.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>  // For mkdir on Windows
#include <iomanip>

// Helper to format price with commas and 1 decimal
std::string formatPrice(double price)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << price;
    std::string s = ss.str();
    size_t dot = s.find('.');
    size_t start = (s[0] == '-') ? 1 : 0;
    for (size_t i = dot - 3; i > start; i -= 3)
        s.insert(i, ",");
    return s;
}

RealEstatePortal::RealEstatePortal() : currentUser(nullptr), nextPropertyId(1)
{
    loadData();
}

RealEstatePortal::~RealEstatePortal()
{
    saveData();
    for (const auto& pair : users)
        delete pair.second;
}

void RealEstatePortal::run()
{
    int choice;
    while (true)
    {
        if (!currentUser)
        {
            std::cout << "\n--- Real Estate Portal ---\n";
            std::cout << "1. Register\n2. Login\n3. Exit\nChoice: ";
            std::cin >> choice;
            if (choice == 1)
                registerUser();
            else if (choice == 2)
                loginUser();
            else if (choice == 3)
                break;
            else
                std::cout << "Invalid choice.\n";
        }
        else
        {
            currentUser->displayMenu();
            std::cout << "Choice: ";
            std::cin >> choice;
            if (dynamic_cast<Admin*>(currentUser))
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
                    std::cout << "Invalid choice.\n";
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
                    logoutUser();
                else
                    std::cout << "Invalid choice.\n";
            }
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void RealEstatePortal::registerUser()
{
    std::string uname, pwd, name;
    std::cout << "Enter username: ";
    std::cin >> uname;

    if (users.find(uname) != users.end())
    {
        std::cout << "Username already exists.\n";
        return;
    }

    std::cout << "Enter password: ";
    std::cin >> pwd;
    std::cout << "Enter your name: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    User* newUser = new RegularUser(uname, pwd, name);
    users[uname] = newUser;
    std::cout << "Registration successful!\n";
}

void RealEstatePortal::loginUser()
{
    std::string uname, pwd;
    std::cout << "Enter username: ";
    std::cin >> uname;
    std::cout << "Enter password: ";
    std::cin >> pwd;

    auto it = users.find(uname);
    if (it != users.end() && it->second->checkPassword(pwd))
    {
        currentUser = it->second;
        std::cout << "Login successful!\n";
        return;
    }
    std::cout << "Invalid username or password.\n";
}

void RealEstatePortal::logoutUser()
{
    currentUser = nullptr;
    std::cout << "Logged out.\n";
}

// Helper functions for escaping/unescaping commas in features
static std::string escapeCommas(const std::string& s)
{
    std::string out = s;
    size_t pos = 0;
    while ((pos = out.find(',', pos)) != std::string::npos)
    {
        out.replace(pos, 1, "&#44;");
        pos += 5;
    }
    return out;
}
static std::string unescapeCommas(const std::string& s)
{
    std::string out = s;
    size_t pos = 0;
    while ((pos = out.find("&#44;", pos)) != std::string::npos)
    {
        out.replace(pos, 5, ",");
        pos += 1;
    }
    return out;
}

void RealEstatePortal::saveData()
{
    // Save users
    _mkdir("data");  // Windows-style directory creation
    std::ofstream userFile("data/users.txt");
    for (const auto& pair : users)
    {
        User* u = pair.second;
        std::string type = dynamic_cast<Admin*>(u) ? "admin" : "user";
        userFile << type << ',' << u->getUsername() << ',' << u->getPassword() << ',' << u->getName() << '\n';
    }
    userFile.close();

    // Save properties
    std::ofstream propFile("data/properties.txt");
    for (const auto& pair : properties)
    {
        const Property& p = pair.second;
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
    for (const auto& pair : users)
    {
        delete pair.second;
    }
    users.clear();
    properties.clear();

    std::ifstream userFile("data/users.txt");
    if (userFile)
    {
        std::string line;
        while (std::getline(userFile, line))
        {
            std::stringstream ss(line);
            std::string type, uname, pwd, name;
            std::getline(ss, type, ',');
            std::getline(ss, uname, ',');
            std::getline(ss, pwd, ',');
            std::getline(ss, name, ',');

            User* user;
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
    std::ifstream propFile("data/properties.txt");
    if (propFile)
    {
        std::string line;
        while (std::getline(propFile, line))
        {
            std::stringstream ss(line);
            int id;
            std::string name, location, type, priceStr, features, owner, highlightStr, approveStr;
            double price;
            std::getline(ss, priceStr, ',');
            id = std::stoi(priceStr);
            std::getline(ss, name, ',');
            name = unescapeCommas(name);
            std::getline(ss, location, ',');
            location = unescapeCommas(location);
            std::getline(ss, type, ',');
            type = unescapeCommas(type);
            std::getline(ss, priceStr, ',');
            price = std::stod(priceStr);
            std::getline(ss, features, ',');
            features = unescapeCommas(features);
            std::getline(ss, owner, ',');
            std::getline(ss, highlightStr, ',');
            std::getline(ss, approveStr, ',');
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
    std::string name, location, type, features;
    double price;
    std::cin.ignore();
    std::cout << "Enter property name: ";
    std::getline(std::cin, name);
    std::cout << "Enter location: ";
    std::getline(std::cin, location);
    std::cout << "Enter type: ";
    std::getline(std::cin, type);
    std::cout << "Enter price: ";
    std::cin >> price;

    std::cout << "Enter features: ";
    std::cin.ignore();
    std::getline(std::cin, features);

    std::string owner = currentUser ? currentUser->getUsername() : "unknown";
    properties[nextPropertyId] = Property(nextPropertyId, name, location, type, price, features, owner, false, false);
    nextPropertyId++;
    std::cout << "Property listed successfully! Awaiting admin approval.\n";
}

void RealEstatePortal::compareProperties()
{
    std::vector<const Property*> propsToCompare;
    int id, count = 0;
    std::cout << "Enter up to 4 property IDs to compare (0 to stop): ";
    while (count < 4)
    {
        std::cin >> id;
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
            std::cout << "Property ID " << id << " not found or not approved. Try another.\n";
        }
    }

    if (propsToCompare.empty())
    {
        std::cout << "No valid properties selected for comparison.\n";
        return;
    }

    // Find the maximum width needed for each field
    size_t maxNameLen = 4, maxLocLen = 8, maxTypeLen = 4, maxFeatLen = 8;
    for (const auto* prop : propsToCompare)
    {
        maxNameLen = std::max(maxNameLen, prop->getName().length());
        maxLocLen = std::max(maxLocLen, prop->getLocation().length());
        maxTypeLen = std::max(maxTypeLen, prop->getType().length());
        maxFeatLen = std::max(maxFeatLen, prop->getFeatures().length());
    }

    // Print header
    std::cout << "\n=== Property Comparison ===\n\n";
    std::cout << std::setw(15) << "Property ID";
    for (const auto* prop : propsToCompare)
    {
        std::cout << " | " << std::setw(5) << prop->getId();
    }
    std::cout << "\n" << std::string(15 + propsToCompare.size() * 8, '-') << "\n";

    // Print details
    std::cout << std::setw(15) << "Name";
    for (const auto* prop : propsToCompare)
    {
        std::cout << " | " << std::setw(5) << prop->getName();
    }
    std::cout << '\n';

    std::cout << std::setw(15) << "Location";
    for (const auto* prop : propsToCompare)
    {
        std::cout << " | " << std::setw(5) << prop->getLocation();
    }
    std::cout << '\n';

    std::cout << std::setw(15) << "Type";
    for (const auto* prop : propsToCompare)
    {
        std::cout << " | " << std::setw(5) << prop->getType();
    }
    std::cout << '\n';

    std::cout << std::setw(15) << "Price";
    for (const auto* prop : propsToCompare)
    {
        std::cout << " | $" << std::setw(4) << formatPrice(prop->getPrice());
    }
    std::cout << '\n';

    std::cout << std::setw(15) << "Features";
    for (const auto* prop : propsToCompare)
    {
        std::cout << " | " << std::setw(5) << prop->getFeatures();
    }
    std::cout << '\n';

    std::cout << std::setw(15) << "Highlighted";
    for (const auto* prop : propsToCompare)
    {
        std::cout << " | " << std::setw(5) << (prop->isHighlighted() ? "Yes" : "No");
    }
    std::cout << "\n\n";
}

void RealEstatePortal::adminManageListings()
{
    int choice;
    std::cout << "\n+==================== Admin Listing Management ====================+\n";
    std::cout << "|                      Current Properties                          |\n";
    std::cout << "+-------+-----------------------------------+------------+---------+\n";
    std::cout << "| ID    |           Property Name           |   Status   | Feature |\n";
    std::cout << "+-------+-----------------------------------+------------+---------+\n";

    // Calculate maximum lengths for formatting
    size_t maxNameLen = 35;  // We'll truncate names longer than this

    for (const auto& pair : properties)
    {
        const Property& p = pair.second;
        std::string name = p.getName();
        if (name.length() > maxNameLen - 3) {
            name = name.substr(0, maxNameLen - 3) + "...";
        }
        else {
            name.append(maxNameLen - name.length(), ' ');
        }

        std::cout << "| " << std::setw(5) << p.getId() << " | "
            << name << " | "
            << std::setw(10) << (p.isApproved() ? "Approved" : "Pending") << " | "
            << std::setw(7) << (p.isHighlighted() ? "*Yes" : "No") << " |\n";
    }

    std::cout << "+-------+-----------------------------------+------------+---------+\n\n";
    std::cout << "Management Options:\n";
    std::cout << "+----------------------------------+\n";
    std::cout << "| 1. Remove Property               |\n";
    std::cout << "| 2. Toggle Property Highlight     |\n";
    std::cout << "| 3. Approve Property             |\n";
    std::cout << "| 4. Back to Main Menu            |\n";
    std::cout << "+----------------------------------+\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    if (choice == 1)
    {
        int id;
        std::cout << "\nEnter property ID to remove: ";
        std::cin >> id;
        if (properties.erase(id) > 0)
        {
            std::cout << "(+) Property successfully removed.\n";
        }
        else
        {
            std::cout << "(-) Property not found.\n";
        }
    }
    else if (choice == 2)
    {
        int id;
        std::cout << "\nEnter property ID to toggle highlight status: ";
        std::cin >> id;
        auto it = properties.find(id);
        if (it != properties.end())
        {
            it->second.setHighlighted(!it->second.isHighlighted());
            std::cout << "(+) Property highlight status "
                << (it->second.isHighlighted() ? "enabled" : "disabled") << ".\n";
        }
        else
        {
            std::cout << "(-) Property not found.\n";
        }
    }
    else if (choice == 3)
    {
        // Check if all properties are already approved
        bool allApproved = true;
        for (const auto& pair : properties) {
            if (!pair.second.isApproved()) {
                allApproved = false;
                break;
            }
        }

        if (allApproved) {
            std::cout << "\n(-) All properties are already approved.\n";
            return;
        }

        int id;
        std::cout << "\nEnter property ID to approve: ";
        std::cin >> id;
        auto it = properties.find(id);
        if (it != properties.end())
        {
            if (it->second.isApproved()) {
                std::cout << "(-) This property is already approved.\n";
            }
            else {
                it->second.setApproved(true);
                std::cout << "(+) Property approved successfully.\n";
            }
        }
        else
        {
            std::cout << "(-) Property not found.\n";
        }
    }
}

void RealEstatePortal::adminManageUsers()
{
    int choice;
    std::cout << "\n--- Admin User Management ---\n";
    std::cout << "Current Users:\n";
    for (const auto& pair : users)
    {
        const User* u = pair.second;
        std::string type = dynamic_cast<const Admin*>(u) ? "Admin" : "User";
        std::cout << "Username: " << u->getUsername() << " | Name: " << u->getName() << " | Type: " << type << std::endl;
    }
    std::cout << "1. Remove User\n2. Back\nChoice: ";
    std::cin >> choice;
    if (choice == 1)
    {
        std::string uname;
        std::cout << "Enter username to remove: ";
        std::cin >> uname;
        auto it = users.find(uname);
        if (it != users.end())
        {
            delete it->second;
            users.erase(it);
            std::cout << "User removed.\n";
        }
        else
        {
            std::cout << "User not found.\n";
        }
    }
}

void RealEstatePortal::searchProperties()
{
    std::string location, type;
    double minPrice = 0, maxPrice = 1e9;

    // Clear input buffer before getting location
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter location to search (or leave empty): ";
    std::getline(std::cin, location);
    std::cout << "Enter type to search (or leave empty): ";
    std::getline(std::cin, type);

    std::cout << "Enter min price (or 0): ";
    std::cin >> minPrice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        minPrice = 0;
    }

    std::cout << "Enter max price (or 0 for no max): ";
    std::cin >> maxPrice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        maxPrice = 0;
    }

    if (maxPrice == 0)
        maxPrice = 1e9;

    std::vector<const Property*> filteredProps;
    for (const auto& pair : properties)
    {
        const Property& p = pair.second;
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

    std::cout << "\n--- Search Results ---\n";
    for (const auto* p : filteredProps)
    {
        p->display();
    }
}

void RealEstatePortal::viewMyListings()
{
    std::cout << "\n--- My Listings ---\n";
    std::string myUser = currentUser ? currentUser->getUsername() : "";

    std::vector<const Property*> myProps;
    for (const auto& pair : properties)
    {
        const Property& p = pair.second;
        if (p.getOwnerUsername() == myUser)
        {
            myProps.push_back(&p);
        }
    }

    for (const auto* p : myProps)
    {
        p->display();
        std::cout << "    Status: " << (p->isApproved() ? "Approved" : "Pending Approval") << std::endl;
    }
}
