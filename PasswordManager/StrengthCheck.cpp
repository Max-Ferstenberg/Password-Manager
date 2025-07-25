#include <string>
#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>

#include "StrengthCheck.h"

//1. Read string in - DONE!
//2. Check for alphanumeric password - DONE!
//3. Check for length - DONE!
//4. Check for character sequences - DONE!
//5. Check if password is a word - DONE!
//6. Check for upper and lower case characters - DONE!
//7. Check for character repetition - DONE!

StrengthCheck::StrengthCheck(const std::string& filename) : v(), alphabet() {
	alphabet = { "abcdefghijklmnopqrstuvwxyz" };
	std::ifstream file(filename);
	std::string line;

	while (std::getline(file, line)) {
		v.push_back(line);
	}

	file.close();
}

bool StrengthCheck::check(const std::string& password) {
	if (isAlphaNumeric(password) || !isLongEnough(password) || isSequential(password) || isWord(password) || !upperLower(password) || isRepetitive(password)) {
		if (isAlphaNumeric(password)) std::cout << "Try a mix of numbers, letters and symbols!\n";
		if (!isLongEnough(password)) std::cout << "Password should be at least 12 characters long.\n";
		if (isSequential(password)) std::cout << "Avoid ascending/descending sequences of numbers and/or letters.\n";
		if (isWord(password)) std::cout << "Avoid using English words directly.\n";
		if (!upperLower(password)) std::cout << "Try a mix of upper and lowercase letters!\n";
		if (isRepetitive(password)) std::cout << "Avoid repetitive sequences of characters.\n";
		return false;
	}
	else {
		return true;
	}
}

bool StrengthCheck::isAlphaNumeric(const std::string& password) {
	//Check the password contains at least one symbol
	for (int i = 0; i < password.length(); i++) {
		if (!isalnum(static_cast<unsigned char>(password[i]))) {
			return false;
		}
	}
	return true;
}

bool StrengthCheck::isLongEnough(const std::string& password) {
	//Check the password is an appropriate length
	if (password.length() >= 12) {
		return true;
	}
	else {
		return false;
	}
}

bool StrengthCheck::isSequential(const std::string& password) {
	//Check for sequences of letters/numbers
	int position1 = {};
	int position2 = {};
	int position3 = {};

	for (int i = 0; i < (password.length() - 1); i++) { //check until password length - 1 to avoid bounds error
		//check for sequence of numbers
		if (isdigit(static_cast<unsigned char>(password[i]))) {
			if (isdigit(static_cast<unsigned char>(password[i + 1])) && ((static_cast<unsigned char>(password[i]) - 1 == static_cast<unsigned char>(password[i + 1])) || (static_cast<unsigned char>(password[i]) + 1 == static_cast<unsigned char>(password[i + 1])))) {
				std::cout << "Password contains invalid number sequence: " << password[i] << password[i + 1] << '\n';
				return true;
			}
		}
	}

	for (int i = 0; i < (password.length()); i++) {
		//check for sequence of letters
		if (isalpha(static_cast<unsigned char>(password[i])) && i != 0) {
			if (isalpha(static_cast<unsigned char>(password[i - 1])) && isalpha(static_cast<unsigned char>(password[i + 1]))) {
				size_t position1 = alphabet.find(tolower(static_cast<unsigned char>(password[i - 1])));
				size_t position2 = alphabet.find(tolower(static_cast<unsigned char>(password[i])));
				size_t position3 = alphabet.find(tolower(static_cast<unsigned char>(password[i + 1])));

				if (((position2 + 1 == position3) && (position2 - 1 == position1)) || ((position2 - 1 == position3) && (position2 + 1 == position1))) {
					std::cout << "Password contains invalid letter sequence: " << password[i - 1] << password[i] << password[i + 1] << '\n';
					return true;
				}
			}
		}
	}
	return false;
}

bool StrengthCheck::isWord(const std::string& password) {
	//Check the password is not just a commonly used word
	std::string substring = "";

	for (int i = 0; i < password.length(); i++) {
		if (isalpha(static_cast<unsigned char>(password[i]))) {
			substring += tolower(static_cast<unsigned char>(password[i]));
			for (int j = 0; j < v.size(); j++) {
				if (substring == v[j] && substring.length() > 3) {
					std::cout << "Substring: " << substring << " found in dictionary\n";
					return true;
				}
			}
		}
		else {
			substring = "";
		}
	}
	return false;
}

bool StrengthCheck::upperLower(const std::string& password) {
	//Check for a mix of upper and lowercase characters
	bool hasUpper = false;
	bool hasLower = false;
	for (int i = 0; i < password.length(); i++) {
		if (isupper(static_cast<unsigned char>(password[i]))) {
			hasUpper = true;
		}

		if (islower(static_cast<unsigned char>(password[i]))) {
			hasLower = true;
		}
	}

	if (hasUpper && hasLower) {
		return true;
	}
	else {
		return false;
	}
}

bool StrengthCheck::isRepetitive(const std::string& password) {
	//Check for repeated characters
	for (int i = 1; i < password.length(); i++) {
		if (password[i] == password[i - 1] && password[i] == password[i + 1]) {
			std::cout << "Repetitive sequence found at: " << password[i - 1] << password[i] << password[i + 1] << '\n';
			return true;
		}
	}
	return false;
}