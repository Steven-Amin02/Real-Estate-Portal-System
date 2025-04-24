#include <stdexcept>
#include <iostream>
#include "Account.h"
#include "Exception.h"
#include "Property.h"
#include "SelctorInputValidator.h"

BaseAccount::BaseAccount(AccountType type, std::string firstName, std::string lastName, std::string userHandle, std::string password)
	: type(type), firstName(firstName), lastName(lastName), userHandle(userHandle), password(password) {
}

AdminAccount::AdminAccount(std::string firstName, std::string lastName, std::string userHandle, std::string password)
	: BaseAccount(AccountType::AdminAccount, firstName, lastName, userHandle, password), authorization(false) {
}

UserAccount::UserAccount(std::string firstName, std::string lastName, std::string userHandle, std::string password)
	: BaseAccount(AccountType::UserAccount, firstName, lastName, userHandle, password) {
}

void BaseAccount::createAccount() {
	int accountTypeChoice;
	std::string firstName, lastName;
	std::string userHandle, password;
	std::cout << "Press:\n(1) to create an admin account.\n(2) to create a user account.\n";
	InputValidator::inputSelector(accountTypeChoice, 2);
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
	}
	else {
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
			}
			else {
				// display user menu
			}
		}
		else {
			std::cout << "Incorrect credential combiination, retry!\n";
			BaseAccount::logIn(accountsStore);
		}
	}
	catch (std::out_of_range e) {
		std::cout << "Account does not exist, retry!\n";
		BaseAccount::logIn(accountsStore);
	}
}

void UserAccount::updateAccountInfo() {
	int userSelector;
	std::string userInput;
	std::cout << "Press:\n(1) to update first name.\n(2) to update last name.\n(3) to update password.\n";
	InputValidator::inputSelector(userSelector);
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

void UserAccount::submitPropertyListing(std::unordered_map<PropertyType, std::unordered_map<PropertyLocation, PropertyListing[]>*>* litsingsStore) {
	int propertyTypeChoice, governorateChoice, cityChoice, streetChoice;
	std::string name, features;
	double price, size;
	std::cout << "Enter Property Name: ";
	std::cin >> name;
	std::cout << "Enter Property Size: ";
	std::cin >> size;
	std::cout << "Pick Property Type:\n";
	std::cout << "Press:\n(1) for Apartment.\n(2) for Villa.\n(3) for Studio.\n(4) for Townhouse.\n(5) for Land.\n(6) for Unknown.\n";
	InputValidator::inputSelector(propertyTypeChoice, 6);
	std::cout << "Enter Property Features: ";
	std::cin >> features;
	std::cout << "Pick Property Location: ";
	std::cout << "Pick Governorate:\nPress:\n(1) for Cairo.\n(2) for Alexandria.\n(3) for Giza.\n";
	InputValidator::inputSelector(governorateChoice);
	std::cout << "Pick City:\nPress:\n";
	const char* cities(PropertyLocation::getCities(3 * (governorateChoice - 1)));
	for (int i(0); i < 3; ++i)
		std::cout << '(' << (i + 1) << ") for " << cities[i] << '.' << '\n';
	InputValidator::inputSelector(cityChoice);
	const char* streets(PropertyLocation::getStreets(3 * (cityChoice - 1)));
	for (int i(0); i < 3; ++i)
		std::cout << '(' << (i + 1) << ") for " << streets[i] << '.' << '\n';
	InputValidator::inputSelector(streetChoice);
	PropertyLocation location(governorateChoice - 1, &(cities[cityChoice - 1]), &(streets[streetChoice - 1]));
	std::cout << "Enter Property Price: ";
	std::cin >> price;
	try {
		std::unordered_map<PropertyLocation, PropertyListing[]>* propertyByType = litsingsStore->at((PropertyType)(propertyTypeChoice - 1));
		try {
			propertyByType->at(location)[5];
		}
		catch (std::out_of_range e) {
			// propertyByType->insert();
		}
	}
	catch (std::out_of_range e) {
		// litsingsStore->insert();
	}
	PropertyListing newProperty(name, location, price, size, (PropertyType)(propertyTypeChoice - 1), features, this->userHandle);
	// store property in appropriate data structure here
}
