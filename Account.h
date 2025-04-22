#ifndef ACCOUNT
#define ACCOUNT

#include <unordered_map>
#include <string>
#include "Property.h"

enum class AccountType {
	AdminAccount,
	UserAccount
};

class BaseAccount {
public:
	AccountType type;
	std::string firstName, lastName;
	std::string userHandle, password;
	static void createAccount();
	static void logIn(std::unordered_map<std::string, BaseAccount*>*);
	BaseAccount(AccountType, std::string, std::string, std::string, std::string);
};

class AdminAccount : public BaseAccount {
public:
	// admin-specific code
	bool authorization;
	AdminAccount(std::string, std::string, std::string, std::string);
};

class UserAccount : public BaseAccount {
public:
	void submitPropertyListing();
	void updateAccountInfo();
	UserAccount(std::string, std::string, std::string, std::string);
};

#endif
