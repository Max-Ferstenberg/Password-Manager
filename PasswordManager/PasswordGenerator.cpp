#include <string>
#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>
#include <random>

#include "PasswordGenerator.h"
#include "StrengthCheck.h"

PasswordGenerator::PasswordGenerator() : ascii("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!£$%^&*()-_=+[{]}#~;:@<,>.?/|"), password(""), rng_(std::random_device{}()), dist_(0, ascii.size() - 1) {

}

std::string PasswordGenerator::Generate(int length) {
	password.clear();
	password.reserve(length);
	for (int i = 0; i < length; i++) {
		password += ascii[dist_(rng_)];
	}

	std::cout << "Password Generated: " << password << '\n';
	
	return password;
}

bool PasswordGenerator::isValid(StrengthCheck& checker) {
	if (checker.isAlphaNumeric(password) || checker.isRepetitive(password) || checker.isSequential(password) || checker.isWord(password) || !checker.upperLower(password)) return false;
	if (!checker.isLongEnough(password)) {
		std::cout << "Password length must be at least 12 characters.\n";
		return false;
	}
	
	return true;
}
