#pragma once
#include "User.h"

class Admin : public User
{
public:
	Admin(std::string uname, std::string pwd, std::string n);
	void displayMenu() override;
};
