#ifndef FILEIO_H
#define FILEIO_H

#include "tinyxml2.h"
#include <string>

class FileIO {
public:
	FileIO();
	void Write(const std::string& accountName, const std::string& passwordString, const std::string& usernameString);
	void Search(std::string accountName);

private:
	tinyxml2::XMLDocument passwords;
};

#endif // !FILEIO_H