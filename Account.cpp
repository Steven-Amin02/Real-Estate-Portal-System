#include <stdexcept>
#include <iostream>
#include "Account.h"
#include "Exception.h"
#include "Property.h"

BaseAccount::BaseAccount(AccountType type, std::string firstName, std::string lastName, std::string userHandle, std::string password)
: type(type), firstName(firstName), lastName(lastName), userHandle(userHandle), password(password) {}

AdminAccount::AdminAccount(std::string firstName, std::string lastName, std::string userHandle, std::string password)
	: BaseAccount(AccountType::AdminAccount, firstName, lastName, userHandle, password), authorization(false) {
}

UserAccount::UserAccount(std::string firstName, std::string lastName, std::string userHandle, std::string password)
: BaseAccount(AccountType::UserAccount, firstName, lastName, userHandle, password) {}

void BaseAccount::createAccount() {
	int accountTypeChoice;
	std::string firstName, lastName;
	std::string userHandle, password;
	std::cout << "Press:\n(1) to create an admin account.\n(2) to create a user account.\n";
	try {
		std::cin >> accountTypeChoice;
		if (accountTypeChoice < 1 || accountTypeChoice > 2)
			throw InvalidInputException();
	}
	catch (InvalidInputException e) {
		std::cout << e.getErrorMessage();
		BaseAccount::createAccount();
	}
	std::cout << "Enter First Name: ";
	std::cin >> firstName;
	std::cout << "Enter Last Name: ";
	std::cin >> lastName;
	std::cout << "Enter User Handle: @";
	std::cin >> userHandle;
	std::cout << "Enter Password: ";
	std::cin >> password;
	BaseAccount* newAcc;
	if (accountTypeChoice == 1) {
		newAcc = new AdminAccount(firstName, lastName, userHandle, password);
	} else {
		newAcc = new UserAccount(firstName, lastName, userHandle, password);
	}
	// go back to main menu
}

void BaseAccount::logIn(std::unordered_map<std::string, BaseAccount*>* accountsStore) {
	std::string userHandle, password;
	std::cout << "Enter User Handle: @";
	std::cin >> userHandle;
	std::cout << "Enter Password: ";
	std::cin >> password;
	try {
		BaseAccount* acc(accountsStore->at(userHandle));
		if (acc->password == password) {
			if (acc->type == AccountType::AdminAccount) {
				if (((AdminAccount*)(acc))->authorization == true)
					// display admin menu
					;
				else
					std::cout << "Your account awaits authorization!\n";
			} else {
				// display user menu
			}
		} else {
			std::cout << "Incorrect credential combiination, retry!\n";
			BaseAccount::logIn(accountsStore);
		}
	} catch (std::out_of_range e) {
		std::cout << "Account does not exist, retry!\n";
		BaseAccount::logIn(accountsStore);
	}
}

void UserAccount::updateAccountInfo() {
	int userSelector;
	std::string userInput;
	std::cout << "Press:\n(1) to update first name.\n(2) to update last name.\n(3) to update password.\n";
	try {
		std::cin >> userSelector;
		if (userSelector < 1 || userSelector > 3)
			throw InvalidInputException();
	}
	catch (InvalidInputException e) {
		std::cout << e.getErrorMessage();
		this->updateAccountInfo();
	}
	std::cout << "Input: ";
	std::cin >> userInput;
	switch (userSelector) {
	case 1:
		this->firstName = userInput;
		break;
	case 2:
		this->lastName = userInput;
		break;
	case 3:
		this->password = userInput;
		break;
	}
}

void UserAccount::submitPropertyListing() {
	int propertyTypeChoice;
	std::string name, features, location;
	double price, size;
	std::cout << "Enter Property Name: ";
	std::cin >> name;
	std::cout << "Enter Property Size: ";
	std::cin >> size;
	std::cout << "Enter Property Features: ";
	std::cin >> features;
	std::cout << "Enter Property Location: ";
	std::cin >> location;
	std::cout << "Enter Property Price: ";
	std::cin >> price;
	std::cout << "Pick Property Type:\n";
	std::cout << "Press:\n(1) for Apartment.\n(2) for Villa.\n(3) for Studio.\n(4) for Townhouse.\n(5) for Land.\n(6) for Unknown.\n";
	try {
		std::cin >> propertyTypeChoice;
		if (propertyTypeChoice < 1 || propertyTypeChoice > 6)
			throw InvalidInputException();
	}
	catch (InvalidInputException e) {
		std::cout << e.getErrorMessage();
		this->submitPropertyListing();
	}
	Property newProperty(name, location, price, size, (PropertyType) (propertyTypeChoice - 1), features, this->userHandle);
	// store property in appropriate data structure here
}
