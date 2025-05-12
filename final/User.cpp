#include "User.h"

User::User(std::string uname, std::string pwd, std::string n)
    : username(uname), password(pwd), name(n) {}

std::string User::getUsername() const
{
  return username;
}

std::string User::getPassword() const
{
  return password;
}

std::string User::getName() const
{
  return name;
}

bool User::checkPassword(const std::string &pwd) const
{
  return password == pwd;
}