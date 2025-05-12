#pragma once
#include "User.h"

class RegularUser : public User
{
public:
  RegularUser(std::string uname, std::string pwd, std::string n);
  void displayMenu() override;
};