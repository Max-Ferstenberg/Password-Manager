#include <iostream>
#include <string>

#include "StrengthCheck.h"

int main()
{
	std::string password;
	std::cout << "Enter a password: ";
	std::cin >> password;

	StrengthCheck StrengthChecker("words_alpha.txt", password);
	StrengthChecker.check(password);

	return 0;
}