#include <iostream>
#include <string>

#include "StrengthCheck.h"
#include "PasswordGenerator.h"

bool PasswordStrengthCheck(StrengthCheck& StrengthChecker);
void PasswordGen(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker);

int main()
{
	std::cout << "Loading...\n";
	PasswordGenerator PassGen;
	StrengthCheck StrengthChecker("words_alpha.txt");

	int option;

	std::cout << "What would you like to do?\n" << "Option 1: Check the strength of an existing password?\n" << "Option 2: Generate a new, secure password?\n" << "Option 3: Close the program.\n" << "Please enter the number of the option you would like: ";
	std::cin >> option;
	while (1 == 1) {
		while (option != 1 && option != 2 && option != 3) {
			std::cout << "Enter an option number: ";
			std::cin >> option;
		} 
	
		if (option == 1) {
			if (!PasswordStrengthCheck(StrengthChecker)) {
				PasswordStrengthCheck(StrengthChecker);
			}
		} if (option == 2) {
			PasswordGen(PassGen, StrengthChecker);
		} if (option == 3) {
			break;
		}
	}

	return 0;
}

bool PasswordStrengthCheck(StrengthCheck& StrengthChecker) {
	std::string password;
	char yn;
	std::cout << "Enter a password to be checked: ";
	std::cin >> password;
	if (StrengthChecker.check(password)) {
		std::cout << "Password is secure!";
		return true;
	}
	else {
		std::cout << "Would you like to check another password? (y/n): ";
		std::cin >> yn;
		while (tolower(yn) != 'y' && tolower(yn) != 'n') {
			std::cout << "Respond with y or n: ";
			std::cin >> yn;
		} if (tolower(yn) == 'y') {
			return false;
		} if (tolower(yn) == 'n') {
			return true;
		}
	}
}

void PasswordGen(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker) {
	std::string password;
	int length;

	std::cout << "Enter desired password length (Min. 12): ";
	std::cin >> length;

	while (length < 12) {
		std::cout << "Passwords must be at least 12 characters long.\n" << "Enter desired password length (Min. 12): ";
		std::cin >> length;
	}

	if (length >= 12) {
		password = PassGen.Generate(length);
		std::cout << "Validating...\n";
		while (!PassGen.isValid(StrengthChecker)) {
			password = PassGen.Generate(length);
		}
		if (PassGen.isValid(StrengthChecker)) {
			std::cout << "Password Confirmed: " << password << std::endl;
		}
	}
}