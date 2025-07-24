#include <iostream>
#include <string>

#include "StrengthCheck.h"
#include "PasswordGenerator.h"
#include "FileIO.h"
#include "PBKDF2.h"
#include "Encryption.h"

#include <pwdbased.h>
#include <sha.h>
#include <base64.h>


//for testing purposes, master password is: V;GAdXn0[Swq

void menu(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor);
void PasswordStrengthCheck(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor);
void PasswordGen(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor);
void PasswordSearch(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor);
void EncryptFile(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor, std::string& MasterPassword);

int main()
{
	std::cout << "Loading...\n";
	PasswordGenerator PassGen;
	FileIO FileOperations;
	StrengthCheck StrengthChecker("words_alpha.txt");
	PBKDF2 PBKDF2;
	CryptoPP::Base64Encoder encoder;
	Encryption Encryptor;
	
	if (FileOperations.MPSearch().hash.empty() || FileOperations.MPSearch().salt.empty()) {
		PBKDF2.PwordGen(StrengthChecker, FileOperations);
		menu(PassGen, StrengthChecker, FileOperations, Encryptor);
		
	}

	else {
		PBKDF2.Login(FileOperations);
		menu(PassGen, StrengthChecker, FileOperations, Encryptor);
	}

	return 0;
}

void menu(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor) {
	int option;

	std::cout << "What would you like to do?\n" << "Option 1: Check the strength of an existing password?\n" << "Option 2: Generate a new, secure password?\n" << "Option 3: Retrieve credentials for an existing account?\n" << "Option 4: Encrypt a file?\n" << "Please enter the number of the option you would like: ";
	std::cin >> option;
	while (1 == 1) {
		while (option != 1 && option != 2 && option != 3 && option != 4) {
			std::cout << "Enter a valid option number: ";
			std::cin >> option;
		}

		if (option == 1) {
			PasswordStrengthCheck(PassGen, StrengthChecker, FileOperations, Encryptor);
		} if (option == 2) {
			PasswordGen(PassGen, StrengthChecker, FileOperations, Encryptor);
		} if (option == 3) {
			PasswordSearch(PassGen, StrengthChecker, FileOperations, Encryptor);
		} if (option == 4) {
			PBKDF2 Verifier;
			std::string MasterPassword;
			std::cout << "Verify master password: ";
			std::cin >> MasterPassword;
			std::cout << std::endl;

			std::string salt = FileOperations.MPSearch().salt;
			std::string pwordhash = FileOperations.MPSearch().hash;
			if (Verifier.DeriveKey(Verifier.Decode(salt), MasterPassword) == Verifier.Decode(pwordhash)) {
				EncryptFile(PassGen, StrengthChecker, FileOperations, Encryptor, MasterPassword);
			}
		}
	}
}

void PasswordStrengthCheck(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor) {
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
			PasswordStrengthCheck(PassGen, StrengthChecker, FileOperations, Encryptor);
		} if (tolower(yn) == 'n') {
			menu(PassGen, StrengthChecker, FileOperations, Encryptor);
		}
	}
}

void PasswordGen(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor) {
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
			PasswordGen(PassGen, StrengthChecker, FileOperations, Encryptor);
		} if (tolower(yn) == 'n') {
			menu(PassGen, StrengthChecker, FileOperations, Encryptor);
		}

	} if (tolower(yn) == 'n') {
		menu(PassGen, StrengthChecker, FileOperations, Encryptor);
	}
}

void PasswordSearch(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor) {
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
		PasswordSearch(PassGen, StrengthChecker, FileOperations, Encryptor);
	} if (tolower(yn) == 'n') {
		menu(PassGen, StrengthChecker, FileOperations, Encryptor);
	}

}

void EncryptFile(PasswordGenerator& PassGen, StrengthCheck& StrengthChecker, FileIO& FileOperations, Encryption& Encryptor, std::string& MasterPassword) {
	char yn;

	PBKDF2 Verifier;
	std::string filepath;
	std::string e_salt = FileOperations.MPSearch().e_salt;
	int ab;

	std::cout << "Select an option:\n 1. Encrypt a file.\n 2. Decrypt a file.\n Please enter an option number: ";
	std::cin >> ab;
	while (ab != 1 && ab != 2) {
		std::cout << "Respond with a valid option number: ";
		std::cin >> ab;
	} if (ab == 1) {
		std::cout << "Enter the filepath to encrypt: ";
		std::cin >> filepath;
		std::cout << std::endl;

		Encryption::ivCipher ivc = Encryptor.AES256Encrypt(Verifier.Decode(e_salt), MasterPassword, filepath);
	} if (ab == 2) {
		std::cout << "Enter filepath to decrypt: ";
		std::cin >> filepath;
		std::cout << std::endl;
		Encryptor.AES256Decrypt(filepath, Verifier.Decode(e_salt), MasterPassword, false);
	}

	std::cout << "Would you like to encrypt something else? (y/n): ";
	std::cin >> yn;
	while (tolower(yn) != 'y' && tolower(yn) != 'n') {
		std::cout << "Respond with y or n: ";
		std::cin >> yn;
	} if (tolower(yn) == 'y') {
		EncryptFile(PassGen, StrengthChecker, FileOperations, Encryptor, MasterPassword);
	} if (tolower(yn) == 'n') {
		menu(PassGen, StrengthChecker, FileOperations, Encryptor);
	}

}