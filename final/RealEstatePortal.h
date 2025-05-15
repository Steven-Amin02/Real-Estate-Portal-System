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
    std::unordered_map<int, Property> properties;
    User *currentUser;
    int nextPropertyId;

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
    void searchProperties();
    void compareProperties();
    void viewMyListings();
    void adminManageListings();
    void adminManageUsers();
    void manageProfile();
    void editProperty();
};
