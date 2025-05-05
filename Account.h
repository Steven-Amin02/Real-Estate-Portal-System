#pragma once
#ifndef ACCOUNT
#define ACCOUNT

#include <unordered_map>
#include <string>
#include "Property.h"

class Admin;

using namespace std;

enum class AccountType {
    AdminAccount,
    UserAccount
};

class BaseAccount {
public:
    AccountType type;
    string firstName, lastName;
    string userHandle, password;
    static void createAccount();
    static void logIn(unordered_map<string, BaseAccount*>*);
    BaseAccount(AccountType, string, string, string, string);
};

class AdminAccount : public BaseAccount {
public:
    bool authorization;
    AdminAccount(string, string, string, string);
};

class UserAccount : public BaseAccount {
public:
    void submitPropertyListing(Admin admin);
    void updateAccountInfo();
    UserAccount(string, string, string, string);
};

#endif
