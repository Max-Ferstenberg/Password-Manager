#include "FileIO.h"

FileIO::FileIO() : passwords(), salthash() {
	
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

FileIO::SaltHash FileIO::MPSearch() {
	if (salthash.LoadFile("salthash.xml") != tinyxml2::XML_SUCCESS) {
		std::cerr << "Error loading XML file: salthash.xml" << std::endl;
		return {};
	}

	tinyxml2::XMLElement* root = salthash.RootElement();
	if (!root) {
		std::cerr << "Error: No root element found in XML file." << std::endl;
		return {};
	}

	tinyxml2::XMLElement* saltElement = root->FirstChildElement("salt");
	tinyxml2::XMLElement* pwordhashElement = root->FirstChildElement("pwordhash");
	tinyxml2::XMLElement* esaltElement = root->FirstChildElement("e_salt");

	if ((saltElement && saltElement->GetText()) && (pwordhashElement && pwordhashElement->GetText()) && esaltElement && esaltElement->GetText()) {
		std::string salt = saltElement->GetText();
		std::string pwordhash = pwordhashElement->GetText();
		std::string e_salt = esaltElement->GetText();
		return { salt, pwordhash, e_salt };
	}
	else {
		std::cerr << "Master password not initialsed.\n";
		return {};
	}
}

void FileIO::MPWrite(const std::string& salt, const std::string& pwordhash, const std::string& e_salt) {
	salthash.LoadFile("salthash.xml");
	tinyxml2::XMLNode* root = salthash.RootElement();;
	
	tinyxml2::XMLElement* saltElement = root->FirstChildElement("salt");
	tinyxml2::XMLElement* pwordhashElement = root->FirstChildElement("pwordhash");
	tinyxml2::XMLElement* e_saltElement = root->FirstChildElement("e_salt");

	saltElement->SetText(salt.c_str());
	pwordhashElement->SetText(pwordhash.c_str());
	e_saltElement->SetText(e_salt.c_str());

	salthash.SaveFile("salthash.xml");
}