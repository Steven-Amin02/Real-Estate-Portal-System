#include "Exception.h"

InvalidInputException::InvalidInputException(const char* ptr) {
	this->errorMessage = ptr;
}

InvalidInputException::InvalidInputException() : errorMessage("Invalid input, retry!\n") {}

std::string InvalidInputException::getErrorMessage() {
	return this->errorMessage;
}
