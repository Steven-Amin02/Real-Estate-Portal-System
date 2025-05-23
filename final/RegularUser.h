#pragma once
#include "User.h"

class RegularUser : public User
{
public:
	RegularUser(std::string uname, std::string pwd, std::string n);
	void displayMenu(int totalUsers = 0, int totalProps = 0, int pendingProps = 0, int approvedProps = 0, int highlightedProps = 0) override;
};
