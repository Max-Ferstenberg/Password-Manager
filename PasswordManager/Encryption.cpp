#include "Encryption.h"

/*
* TODO:
* - Read in files
* - On master password initialisation, use the secondary encryption salt along with the master password
* to make an encryption key for the ciphers - DO NOT STORE THE ENCRYPTION KEY ANYWHERE, ONLY EVER DERIVE IT
* - Make sure to store the master password in session on login, so that the key can be remade every time a file is edited
*/

Encryption::Encryption() {

}

Encryption::ivCipher Encryption::AES256Encrypt(CryptoPP::SecByteBlock salt, std::string& password, std::string& filename) {
	using namespace CryptoPP;
	PBKDF2 KeyGen;
	AutoSeededRandomPool prng;
	HexEncoder encoder(new FileSink(std::cout));
	SecByteBlock key = KeyGen.DeriveKey(salt, password);
	SecByteBlock iv(AES::BLOCKSIZE);
	
	prng.GenerateBlock(iv, iv.size());

	std::string cipher;
	std::string plaintext;

	try {
		FileSource fs(filename.c_str(), true, new StringSink(plaintext));

		EAX<AES>::Encryption e;
		e.SetKeyWithIV(key, key.size(), iv);

		StringSource ss(plaintext, true, new AuthenticatedEncryptionFilter(e, new StringSink(cipher)));

		std::string encodedIV = KeyGen.Encode(iv);
		std::string encodedCiphertext = KeyGen.Encode(SecByteBlock((const byte*)cipher.data(), cipher.size()));

		std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
		if (!ofs.is_open()) {
			throw Exception(Exception::IO_ERROR, "Failed to open file: " + filename);
		}

		ofs << encodedCiphertext;
		ofs << encodedIV;
		ofs.close();

		std::cout << "Key: " << KeyGen.Encode(key) << std::endl;
		std::cout << "Raw IV: " << iv << std::endl;
		std::cout << "IV after encoding: " << encodedIV << std::endl;
		std::cout << "Ciphertext (Base64): " << encodedCiphertext << std::endl;
	}
	catch (const Exception& e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	return { cipher, iv };
}

void Encryption::AES256Decrypt(std::string& filename, CryptoPP::SecByteBlock salt, std::string& password, bool binary = false) {
	using namespace CryptoPP;
	PBKDF2 KeyGen;

	SecByteBlock key = KeyGen.DeriveKey(salt, password);
	std::string plaintext;

	try {
		std::ifstream ifs(filename, std::ios::binary);
		if (!ifs.is_open()) {
			throw Exception(Exception::IO_ERROR, "Failed to open file: " + filename);
		}
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		std::string filecontent = buffer.str();
		ifs.close();

		const size_t ENCODED_IV_LENGTH = 25;

		std::string encodedIV = filecontent.substr(filecontent.length() - ENCODED_IV_LENGTH);

		std::string encodedCiphertext = filecontent.substr(0, filecontent.length() - ENCODED_IV_LENGTH);

		std::cout << "Extracted Encoded IV: " << encodedIV << std::endl;
		std::cout << "Extracted Encoded Ciphertext Length: " << encodedCiphertext.length() << std::endl;

		SecByteBlock iv = KeyGen.Decode(encodedIV);
		SecByteBlock ciphertextBinary = KeyGen.Decode(encodedCiphertext);

		if (iv.size() != AES::BLOCKSIZE) {
			throw Exception(Exception::OTHER_ERROR, "Decoded IV has incorrect size.");
		}

		EAX<AES>::Decryption d;
		d.SetKeyWithIV(key, key.size(), iv);

		StringSource ss(ciphertextBinary.data(), ciphertextBinary.size(), true, new AuthenticatedDecryptionFilter(d, new StringSink(plaintext)));
		std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
		if (!ofs.is_open()) {
			throw Exception(Exception::IO_ERROR, "Failed to open file: " + filename);
		}

		ofs << plaintext;
		ofs.close();
		std::cout << "Recovered text: " << plaintext << std::endl;

	}
	catch(const Exception& e){
		std::cerr << e.what() << std::endl;
		exit(1);
	}

}