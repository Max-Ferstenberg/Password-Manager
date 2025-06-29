#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <string>
#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>
#include <random>

#include "StrengthCheck.h"

class PasswordGenerator {
public:
	PasswordGenerator();
	std::string Generate(int length);
	bool isValid(StrengthCheck& checker);

private:
	std::string password = {};
	std::string ascii = {};
	std::uniform_int_distribution<> dist_;
	std::mt19937 rng_;
};

#endif