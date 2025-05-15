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
	bool checkPassword(const std::string &pwd) const;
	virtual void displayMenu(int totalUsers = 0, int totalProps = 0, int pendingProps = 0, int approvedProps = 0, int highlightedProps = 0) = 0;
	virtual void setName(const std::string &newName);
	virtual void setPassword(const std::string &newPassword);
};
