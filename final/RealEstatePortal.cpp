#include "RealEstatePortal.h"
#include "Admin.h"
#include "RegularUser.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

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

  User *newUser = new RegularUser(uname, pwd, name);
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
static std::string escapeCommas(const std::string &s)
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
static std::string unescapeCommas(const std::string &s)
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
  mkdir("data", 0777);
  std::ofstream userFile("data/users.txt");
  for (const auto &pair : users)
  {
    User *u = pair.second;
    std::string type = dynamic_cast<Admin *>(u) ? "admin" : "user";
    userFile << type << ',' << u->getUsername() << ',' << u->getPassword() << ',' << u->getName() << '\n';
  }
  userFile.close();
  // Save properties
  std::ofstream propFile("data/properties.txt");
  for (const auto &p : properties)
  {
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
      properties.emplace_back(id, name, location, type, price, features, owner, highlight, approve);
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
  std::getline(std::cin, features);

  std::string owner = currentUser ? currentUser->getUsername() : "unknown";
  properties.emplace_back(nextPropertyId++, name, location, type, price, features, owner, false, false);
  std::cout << "Property listed successfully! Awaiting admin approval.\n";
}

void RealEstatePortal::compareProperties()
{
  std::vector<int> ids;
  int id, count = 0;
  std::cout << "Enter up to 4 property IDs to compare (0 to stop): ";
  while (count < 4)
  {
    std::cin >> id;
    if (id == 0)
      break;
    ids.push_back(id);
    count++;
  }
  std::cout << "\n--- Property Comparison ---\n";
  for (int cid : ids)
  {
    for (const auto &p : properties)
    {
      if (p.getId() == cid && p.isApproved())
        p.display();
    }
  }
}

void RealEstatePortal::adminManageListings()
{
  int choice;
  std::cout << "\n--- Admin Listing Management ---\n";
  std::cout << "Current Properties:\n";
  for (const auto &p : properties)
  {
    std::cout << (p.isHighlighted() ? "* " : "  ")
              << "ID: " << p.getId() << " | Name: " << p.getName()
              << " | Status: " << (p.isApproved() ? "Approved" : "Pending Approval") << std::endl;
  }
  std::cout << "1. Remove Property\n2. Highlight/Unhighlight Property\n3. Approve Property\n4. Back\nChoice: ";
  std::cin >> choice;
  if (choice == 1)
  {
    int id;
    std::cout << "Enter property ID to remove: ";
    std::cin >> id;
    auto it = std::remove_if(properties.begin(), properties.end(), [id](const Property &p)
                             { return p.getId() == id; });
    if (it != properties.end())
    {
      properties.erase(it, properties.end());
      std::cout << "Property removed.\n";
    }
    else
    {
      std::cout << "Property not found.\n";
    }
  }
  else if (choice == 2)
  {
    int id;
    std::cout << "Enter property ID to (un)highlight: ";
    std::cin >> id;
    for (auto &p : properties)
    {
      if (p.getId() == id)
      {
        p.setHighlighted(!p.isHighlighted());
        std::cout << (p.isHighlighted() ? "Property highlighted.\n" : "Property unhighlighted.\n");
        return;
      }
    }
    std::cout << "Property not found.\n";
  }
  else if (choice == 3)
  {
    int id;
    std::cout << "Enter property ID to approve: ";
    std::cin >> id;
    for (auto &p : properties)
    {
      if (p.getId() == id)
      {
        p.setApproved(true);
        std::cout << "Property approved.\n";
        return;
      }
    }
    std::cout << "Property not found.\n";
  }
}

void RealEstatePortal::adminManageUsers()
{
  int choice;
  std::cout << "\n--- Admin User Management ---\n";
  std::cout << "Current Users:\n";
  for (const auto &pair : users)
  {
    const User *u = pair.second;
    std::string type = dynamic_cast<const Admin *>(u) ? "Admin" : "User";
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

std::vector<const Property *> RealEstatePortal::getSortedProperties(
    const std::vector<const Property *> &props,
    const std::string &sortBy) const
{

  // Create a priority queue based on the sorting criteria
  auto compare = [sortBy](const Property *a, const Property *b)
  {
    if (sortBy == "price_asc")
    {
      return a->getPrice() > b->getPrice();
    }
    else if (sortBy == "price_desc")
    {
      return a->getPrice() < b->getPrice();
    }
    else if (sortBy == "name")
    {
      return a->getName() > b->getName();
    }
    else if (sortBy == "location")
    {
      return a->getLocation() > b->getLocation();
    }
    else
    { // default: highlight
      if (a->isHighlighted() != b->isHighlighted())
      {
        return !a->isHighlighted();
      }
      return a->getPrice() < b->getPrice(); // secondary sort by price
    }
  };

  std::priority_queue<const Property *, std::vector<const Property *>, decltype(compare)> pq(compare);

  // Add all properties to the priority queue
  for (const auto *prop : props)
  {
    pq.push(prop);
  }

  // Convert priority queue to vector
  std::vector<const Property *> sorted;
  while (!pq.empty())
  {
    sorted.push_back(pq.top());
    pq.pop();
  }

  return sorted;
}

void RealEstatePortal::searchProperties(const std::string &sortBy)
{
  std::string location, type;
  double minPrice = 0, maxPrice = 1e9;
  std::cin.ignore();
  std::cout << "Enter location to search (or leave empty): ";
  std::getline(std::cin, location);
  std::cout << "Enter type to search (or leave empty): ";
  std::getline(std::cin, type);

  std::cout << "Enter min price (or 0): ";
  std::cin >> minPrice;
  std::cout << "Enter max price (or 0 for no max): ";
  std::cin >> maxPrice;

  if (maxPrice == 0)
    maxPrice = 1e9;

  std::vector<const Property *> filteredProps;
  for (const auto &p : properties)
  {
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

  std::cout << "\nSort by:\n";
  std::cout << "1. Highlighted (default)\n";
  std::cout << "2. Price (low to high)\n";
  std::cout << "3. Price (high to low)\n";
  std::cout << "4. Name\n";
  std::cout << "5. Location\n";
  std::cout << "Choice: ";

  int sortChoice;
  std::cin >> sortChoice;

  std::string sortCriteria;
  switch (sortChoice)
  {
  case 2:
    sortCriteria = "price_asc";
    break;
  case 3:
    sortCriteria = "price_desc";
    break;
  case 4:
    sortCriteria = "name";
    break;
  case 5:
    sortCriteria = "location";
    break;
  default:
    sortCriteria = "highlight";
    break;
  }

  auto sortedProps = getSortedProperties(filteredProps, sortCriteria);

  std::cout << "\n--- Search Results ---\n";
  for (const auto *p : sortedProps)
  {
    p->display();
  }
}

void RealEstatePortal::viewMyListings(const std::string &sortBy)
{
  std::cout << "\n--- My Listings ---\n";
  std::string myUser = currentUser ? currentUser->getUsername() : "";

  std::vector<const Property *> myProps;
  for (const auto &p : properties)
  {
    if (p.getOwnerUsername() == myUser)
    {
      myProps.push_back(&p);
    }
  }

  std::cout << "Sort by:\n";
  std::cout << "1. Highlighted (default)\n";
  std::cout << "2. Price (low to high)\n";
  std::cout << "3. Price (high to low)\n";
  std::cout << "4. Name\n";
  std::cout << "5. Location\n";
  std::cout << "Choice: ";

  int sortChoice;
  std::cin >> sortChoice;

  std::string sortCriteria;
  switch (sortChoice)
  {
  case 2:
    sortCriteria = "price_asc";
    break;
  case 3:
    sortCriteria = "price_desc";
    break;
  case 4:
    sortCriteria = "name";
    break;
  case 5:
    sortCriteria = "location";
    break;
  default:
    sortCriteria = "highlight";
    break;
  }

  auto sortedProps = getSortedProperties(myProps, sortCriteria);

  for (const auto *p : sortedProps)
  {
    p->display();
    std::cout << "    Status: " << (p->isApproved() ? "Approved" : "Pending Approval") << std::endl;
  }
}