#ifndef STRENGTHCHECK_H
#define STRENGTHCHECK_H

#include <string>
#include <iostream>
#include <cctype>
#include <fstream>
#include <vector>

//declarations

class StrengthCheck {
public:
	StrengthCheck(const std::string& filename);
	bool check(const std::string& password);
	bool isAlphaNumeric(const std::string& password);
	bool isLongEnough(const std::string& password);
	bool isSequential(const std::string& password);
	bool isWord(const std::string& password);
	bool upperLower(const std::string& password);
	bool isRepetitive(const std::string& password);

private:
	std::string alphabet;
	std::vector<std::string> v;
};

#endif