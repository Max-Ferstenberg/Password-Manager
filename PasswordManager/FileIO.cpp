#include "FileIO.h"
#include "tinyxml2.h"
#include <string>
#include <iostream>

FileIO::FileIO() : passwords() {
	
}

void FileIO::Write(const std::string& accountName, const std::string& passwordString, const std::string& usernameString) {
	passwords.LoadFile("passwords.xml");
	tinyxml2::XMLNode* root = passwords.FirstChild();;
	tinyxml2::XMLElement* account = passwords.NewElement("account");
	account->SetAttribute("account", accountName.c_str());
	root->InsertEndChild(account);

	tinyxml2::XMLElement* username = passwords.NewElement("username");
	username->SetText(usernameString.c_str());
	account->InsertEndChild(username);

	tinyxml2::XMLElement* password = passwords.NewElement("password");
	password->SetText(passwordString.c_str());
	account->InsertEndChild(password);

	passwords.SaveFile("passwords.xml");
}

void FileIO::Search(std::string accountName) {
	if (passwords.LoadFile("passwords.xml") != tinyxml2::XML_SUCCESS) {
		std::cerr << "Error loading XML file: passwords.xml" << std::endl;
		return;
	}
	
	tinyxml2::XMLElement* root = passwords.RootElement();
	if (!root) {
		std::cerr << "Error: No root element found in XML file." << std::endl;
		return;
	}

	for (tinyxml2::XMLElement* accountElement = root->FirstChildElement("account");
		accountElement != nullptr;
		accountElement = accountElement->NextSiblingElement("account")) 
	{
		const char* accountNameAttr = accountElement->Attribute("account");
		if (accountNameAttr && std::string(accountNameAttr) == accountName) {
			std::cout << "Account '" << accountName << "' found.\n";
			tinyxml2::XMLElement* usernameElement = accountElement->FirstChildElement("username");
			tinyxml2::XMLElement* passwordElement = accountElement->FirstChildElement("password");

			if (usernameElement && usernameElement->GetText()) {
				std::cout << "Username: " << usernameElement->GetText() << std::endl;
			}

			if (passwordElement && passwordElement->GetText()) {
				std::cout << "Password: " << passwordElement->GetText() << std::endl;
			}

			return;
		}
	}

	std::cout << "Account '" << accountName << "' not found." << std::endl;
}