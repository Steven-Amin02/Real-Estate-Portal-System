
#pragma once
#include <iostream>
#include<vector>
#include<map>
#include<unordered_map>
#include<set>
#include <queue>
#include "property.h"
#include "Account.h"
using namespace std;


struct DashboardStats {
	size_t totalUsers;
	size_t totalProperties;
	size_t pendingUserApprovals;
	size_t pendingPropertyApprovals;
};
class Admin {
	//baseaccount pointer for efficient storage pointer only store the memory address 
	unordered_map<string, BaseAccount*> Users;
    unordered_map<int, PropertyListing> Property_Listing;
    set<int> highlightedProperty;
	queue<PropertyListing> pendingApprovals;

public:
	Admin(string username, string password, string email, string phone);

	void ManageListings();
	void UserManagement();

	void viewPendingUserApprovals() ;


	DashboardStats getDashboardStats();
	void accessDashboard();

	void addPropertyToApprovalQueue(PropertyListing property);

private:

	void approveNextProperty();
	void rejectNextProperty();

	void addPropertyListing(PropertyListing property);
	void removePropertyListings(int propertyid);
	void editPropertyListing(int propertyid, PropertyListing updatepropertylisting);
	void highlightPropertyListing(int propertyid);

};
