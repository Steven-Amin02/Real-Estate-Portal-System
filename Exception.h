#pragma once
#ifndef EXCEPTION
#define EXCEPTION
#include <string>
using namespace std;
class InvalidInputException {
private:
	string errorMessage;
public:
	InvalidInputException();
	InvalidInputException(const char*);
	string getErrorMessage();
};

#endif
