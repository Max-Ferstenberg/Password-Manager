#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "tinyxml2.h"
#include "PBKDF2.h"
#include "FileIO.h"

#include <fstream>
#include <cryptlib.h>
#include <aes.h>
#include <rsa.h>
#include <rijndael.h>
#include <modes.h>
#include <files.h>
#include <osrng.h>
#include <hex.h>
#include <eax.h>
#include <string>

class Encryption {
public:
	Encryption();
	struct ivCipher {
		std::string ciphertext;
		CryptoPP::SecByteBlock iv;
	};
	ivCipher AES256Encrypt(CryptoPP::SecByteBlock salt, std::string& password, std::string& filename);
	void AES256Decrypt(std::string& filename, CryptoPP::SecByteBlock salt, std::string& password, bool binary);
};


#endif // !ENCRYPTION_H
