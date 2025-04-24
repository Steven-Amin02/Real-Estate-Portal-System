#include <iostream>
#include "SelctorInputValidator.h"
#include "Exception.h"

void InputValidator::inputSelector(int& selector, int max) {
	try {
		std::cin >> selector;
		if (selector < 1 || selector > max)
			throw InvalidInputException();
	} catch (InvalidInputException e) {
		std::cout << e.getErrorMessage();
		InputValidator::inputSelector(selector, max);
	}
}
