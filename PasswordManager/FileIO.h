#ifndef FILEIO_H
#define FILEIO_H

#include "tinyxml2.h"
#include <vector>
#include <string>
#include <iostream>

class FileIO {
public:
	FileIO();

	struct SaltHash {
		std::string salt;
		std::string hash;
		std::string e_salt;
	};

	void Write(const std::string& accountName, const std::string& passwordString, const std::string& usernameString);
	void Search(std::string accountName);
	FileIO::SaltHash MPSearch();
	void MPWrite(const std::string& salt, const std::string& pwordhash, const std::string& e_salt);

private:
	tinyxml2::XMLDocument passwords;
	tinyxml2::XMLDocument salthash;
};

#endif // !FILEIO_H