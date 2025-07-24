#ifndef PBKDF2_H
#define PBKDF2_H

#include "FileIO.h"
#include "StrengthCheck.h"

#include <string>
#include <sha.h>
#include <hmac.h>
#include <cryptlib.h>
#include <osrng.h>
#include <pwdbased.h>
#include <base64.h>

class PBKDF2 {
public:
	PBKDF2();
	CryptoPP::SecByteBlock Salt();
	void Login(FileIO& FileOperations);
	std::string Encode(CryptoPP::SecByteBlock block);
	CryptoPP::SecByteBlock Decode(std::string string);
	void PwordGen(StrengthCheck& StrengthChecker, FileIO& FileOperations);
	CryptoPP::SecByteBlock DeriveKey(CryptoPP::SecByteBlock salt, std::string& MasterPassword);
};

#endif // !PBKDF2_H
