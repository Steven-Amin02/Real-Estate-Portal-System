#pragma once
#include "User.h"

class Admin : public User
{
public:
	Admin(std::string uname, std::string pwd, std::string n);
	void displayMenu(int totalUsers, int totalProps, int pendingProps, int approvedProps, int highlightedProps) override;
};
