#ifndef EXCEPTION
#define EXCEPTION

#include <string>

class InvalidInputException {
private:
	std::string errorMessage;
public:
	InvalidInputException();
	InvalidInputException(const char*);
	std::string getErrorMessage();
};

#endif
