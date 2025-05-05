#include <stdexcept>
#include <iostream>
#include "Exception.h"
#include "Property.h"
#include "Account.h"
#include "Admin.h"
#include "SelctorInputValidator.h"


BaseAccount::BaseAccount(AccountType type, string firstName, string lastName, string userHandle, string password)
	: type(type), firstName(firstName), lastName(lastName), userHandle(userHandle), password(password) {
}

AdminAccount::AdminAccount(string firstName, string lastName, string userHandle, string password)
	: BaseAccount(AccountType::AdminAccount, firstName, lastName, userHandle, password), authorization(false) {
}

UserAccount::UserAccount(string firstName, string lastName, string userHandle, string password)
	: BaseAccount(AccountType::UserAccount, firstName, lastName, userHandle, password) {
}


void BaseAccount::createAccount() {
	int accountTypeChoice;
	string firstName, lastName;
	string userHandle, password;

	cout << "Press:\n(1) to create an admin account.\n(2) to create a user account.\n";
	InputValidator::inputSelector(accountTypeChoice, 2);

	cout << "Enter First Name: ";
	cin >> firstName;
	cout << "Enter Last Name: ";
	cin >> lastName;
	cout << "Enter User Handle: @";
	cin >> userHandle;
	cout << "Enter Password: ";
	cin >> password;

	BaseAccount* newAcc;
	if (accountTypeChoice == 1) {
		newAcc = new AdminAccount(firstName, lastName, userHandle, password);
	}
	else {
		newAcc = new UserAccount(firstName, lastName, userHandle, password);
	}

	

	cout << "Account created successfully!\n";
}


void BaseAccount::logIn(unordered_map<string, BaseAccount*>* accountsStore) {
	string userHandle, password;
	cout << "Enter User Handle: @";
	cin >> userHandle;
	cout << "Enter Password: ";
	cin >> password;
	try {
		BaseAccount* acc(accountsStore->at(userHandle));
		if (acc->password == password) {
			if (acc->type == AccountType::AdminAccount) {
				if (((AdminAccount*)(acc))->authorization == true)
					// display admin menu
					;
				else
					cout << "Your account awaits authorization!\n";
			}
			else {
				// display user menu
			}
		}
		else {
			cout << "Incorrect credential combiination, retry!\n";
			BaseAccount::logIn(accountsStore);
		}
	}
	catch (out_of_range e) {
		cout << "Account does not exist, retry!\n";
		BaseAccount::logIn(accountsStore);
	}
}

void UserAccount::updateAccountInfo() {
	int userSelector;
	string userInput;
	cout << "Press:\n(1) to update first name.\n(2) to update last name.\n(3) to update password.\n";
	InputValidator::inputSelector(userSelector);
	cout << "Input: ";
	cin >> userInput;
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

void UserAccount::submitPropertyListing(Admin admin) {
	int propertyTypeChoice, governorateChoice, cityChoice, streetChoice;
	string name, features;
	double price, size;

	cout << "Enter Property Name: ";
	cin.ignore();
	getline(cin, name);

	cout << "Enter Property Size: ";
	cin >> size;

	cout << "Pick Property Type:\n";
	cout << "Press:\n(1) for Apartment.\n(2) for Villa.\n(3) for Studio.\n(4) for Townhouse.\n(5) for Land.\n(6) for Unknown.\n";
	InputValidator::inputSelector(propertyTypeChoice, 6);

	cout << "Enter Property Features: ";
	cin.ignore();
	getline(cin, features);

	cout << "Pick Property Location: ";
	cout << "Pick Governorate:\nPress:\n(1) for Cairo.\n(2) for Alexandria.\n(3) for Giza.\n";
	InputValidator::inputSelector(governorateChoice);

	cout << "Pick City:\nPress:\n";
	const char* cities(PropertyLocation::getCities(3 * (governorateChoice - 1)));
	for (int i(0); i < 3; ++i)
		cout << '(' << (i + 1) << ") for " << cities[i] << '.' << '\n';
	InputValidator::inputSelector(cityChoice);

	cout << "Pick Street:\nPress:\n";
	const char* streets(PropertyLocation::getStreets(3 * (cityChoice - 1)));
	for (int i(0); i < 3; ++i)
		cout << '(' << (i + 1) << ") for " << streets[i] << '.' << '\n';
	InputValidator::inputSelector(streetChoice);

	PropertyLocation location(governorateChoice - 1, &(cities[cityChoice - 1]), &(streets[streetChoice - 1]));

	cout << "Enter Property Price: ";
	cin >> price;


	PropertyListing newProperty(name, location, price, size, (PropertyType)(propertyTypeChoice - 1), features, this->userHandle);

	// Send the property to the admin's approval queue
	admin.addPropertyToApprovalQueue(newProperty);
	cout << "Property submitted for admin approval.\n";
}
