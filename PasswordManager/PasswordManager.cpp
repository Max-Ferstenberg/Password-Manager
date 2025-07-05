#include <iostream>
#include <string>

#include "StrengthCheck.h"
#include "PasswordGenerator.h"
#include "FileIO.h"

void menu(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations);
void PasswordStrengthCheck(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations);
void PasswordGen(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations);
void PasswordSearch(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations);

int main()
{
	std::cout << "Loading...\n";
	PasswordGenerator PassGen;
	FileIO FileOperations;
	StrengthCheck StrengthChecker("words_alpha.txt");

	menu(PassGen, StrengthChecker, FileOperations);

	return 0;
}

void menu(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations) {
	int option;

	std::cout << "What would you like to do?\n" << "Option 1: Check the strength of an existing password?\n" << "Option 2: Generate a new, secure password?\n" << "Option 3: Retrieve credentials for an existing account?\n" << "Please enter the number of the option you would like: ";
	std::cin >> option;
	while (1 == 1) {
		while (option != 1 && option != 2 && option != 3) {
			std::cout << "Enter a valid option number: ";
			std::cin >> option;
		}

		if (option == 1) {
			PasswordStrengthCheck(PassGen, StrengthChecker, FileOperations);
		} if (option == 2) {
			PasswordGen(PassGen, StrengthChecker, FileOperations);
		} if (option == 3) {
			PasswordSearch(PassGen, StrengthChecker, FileOperations);
		}
	}
}

void PasswordStrengthCheck(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations) {
	std::string password;
	char yn;
	std::cout << "Enter a password to be checked: ";
	std::cin >> password;
	if (StrengthChecker.check(password)) {
		std::cout << "Password is secure!";
	}
	else {
		std::cout << "Would you like to check another password? (y/n): ";
		std::cin >> yn;
		while (tolower(yn) != 'y' && tolower(yn) != 'n') {
			std::cout << "Respond with y or n: ";
			std::cin >> yn;
		} if (tolower(yn) == 'y') {
			PasswordStrengthCheck(PassGen, StrengthChecker, FileOperations);
		} if (tolower(yn) == 'n') {
			menu(PassGen, StrengthChecker, FileOperations);
		}
	}
}

void PasswordGen(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations) {
	std::string password;
	int length;
	char yn;
	std::string accountName;
	std::string usernameString;

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

	std::cout << "Would you like to save this password? (y/n): ";
	std::cin >> yn;
	while (tolower(yn) != 'y' && tolower(yn) != 'n') {
		std::cout << "Respond with y or n: ";
		std::cin >> yn;
	} if (tolower(yn) == 'y') {

		const char* caccountName = accountName.c_str();
		const char* cusernameString = usernameString.c_str();
		const char* cpassword = password.c_str();

		std::cout << "Enter the name of the service or account this password is for: ";
		std::cin >> accountName;
		std::cout << "Enter the corresponding username for that account: ";
		std::cin >> usernameString;

		FileOperations.Write(caccountName, cpassword, cusernameString);

		std::cout << "Would you like to generate another password? (y/n): ";
		std::cin >> yn;
		while (tolower(yn) != 'y' && tolower(yn) != 'n') {
			std::cout << "Respond with y or n: ";
			std::cin >> yn;
		} if (tolower(yn) == 'y') {
			PasswordGen(PassGen, StrengthChecker, FileOperations);
		} if (tolower(yn) == 'n') {
			menu(PassGen, StrengthChecker, FileOperations);
		}

	} if (tolower(yn) == 'n') {
		menu(PassGen, StrengthChecker, FileOperations);
	}
}

void PasswordSearch(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations) {
	std::string accountName = {};
	char yn;

	std::cout << "Enter the name of the account or service you wish to retrieve credentials for (case sensitive): ";
	std::cin >> accountName;

	FileOperations.Search(accountName);

	std::cout << "Would you like to search for another account? (y/n): ";
	std::cin >> yn;
	while (tolower(yn) != 'y' && tolower(yn) != 'n') {
		std::cout << "Respond with y or n: ";
		std::cin >> yn;
	} if (tolower(yn) == 'y') {
		PasswordSearch(PassGen, StrengthChecker, FileOperations);
	} if (tolower(yn) == 'n') {
		menu(PassGen, StrengthChecker, FileOperations);
	}

}