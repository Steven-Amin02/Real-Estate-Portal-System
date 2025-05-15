#pragma once
#include <string>

class User
{
protected:
	std::string username, password, name;

public:
	User(std::string uname, std::string pwd, std::string n);
	virtual ~User() {}
	std::string getUsername() const;
	std::string getPassword() const;
	std::string getName() const;
	bool checkPassword(const std::string& pwd) const;
	virtual void displayMenu() = 0;
};
