#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>
#include "User.h"
#include "Property.h"

class RealEstatePortal
{
private:
  std::unordered_map<std::string, User *> users;
  std::vector<Property> properties;
  User *currentUser;
  int nextPropertyId;

  // Helper function for property sorting
  std::vector<const Property *> getSortedProperties(const std::vector<const Property *> &props,
                                                    const std::string &sortBy) const;

public:
  RealEstatePortal();
  ~RealEstatePortal();
  void run();
  void registerUser();
  void loginUser();
  void logoutUser();
  void saveData();
  void loadData();
  void addProperty();
  void searchProperties(const std::string &sortBy = "highlight");
  void compareProperties();
  void viewMyListings(const std::string &sortBy = "highlight");
  void adminManageListings();
  void adminManageUsers();
};