#include "PBKDF2.h"


PBKDF2::PBKDF2() {
}

CryptoPP::SecByteBlock PBKDF2::Salt() {  
    const unsigned int BLOCKSIZE = 16 * 8;  
    CryptoPP::SecByteBlock scratch(BLOCKSIZE);  
    CryptoPP::AutoSeededRandomPool rng;  

    rng.GenerateBlock(scratch, scratch.size());  
    return scratch;
}

std::string PBKDF2::Encode(CryptoPP::SecByteBlock block) {
    CryptoPP::Base64Encoder encoder; //you should actually reconstruct this class each time this method is called; it clears the buffer of the encoder
    std::string s_block;
    encoder.Put(block.data(), block.size());
    encoder.MessageEnd();
    CryptoPP::word64 size = encoder.MaxRetrievable();
    if (size) {
        s_block.resize(size);
        encoder.Get((CryptoPP::byte*)&s_block[0], s_block.size());
    }

    return s_block;
}

CryptoPP::SecByteBlock PBKDF2::Decode(std::string string) {
	CryptoPP::Base64Decoder decoder;
	CryptoPP::SecByteBlock block;

	decoder.Put((CryptoPP::byte*)string.data(), string.size());
	decoder.MessageEnd();

	CryptoPP::word64 size = decoder.MaxRetrievable();
	if (size && size <= SIZE_MAX) {
		block.resize(size);
		decoder.Get((CryptoPP::byte*)&block[0], block.size());
	}

	return block;
}

void PBKDF2::PwordGen(StrengthCheck& StrengthChecker, FileIO& FileOperations) {
	//prompt user for master password creation here
	std::string MasterPassword1;
	std::string MasterPassword2;
	std::cout << "Please enter a master password: ";
	std::cin >> MasterPassword1;
	if (StrengthChecker.check(MasterPassword1)) {
		std::cout << "Please confirm master password: ";
		std::cin >> MasterPassword2;

		while (MasterPassword1 != MasterPassword2) {
			std::cout << "Passwords do not match.\n";
			std::cout << "Please enter a master password: ";
			std::cin >> MasterPassword1;
			std::cout << "Please confirm master password: ";
			std::cin >> MasterPassword2;
		}

	}

	else {
		PBKDF2::PwordGen(StrengthChecker, FileOperations);
	}

	std::string s_salt;
	std::string s_derivedKey;
	std::string es_salt;

	std::cout << "Encoding..." << std::endl;

	CryptoPP::SecByteBlock salt = PBKDF2::Salt();
	CryptoPP::SecByteBlock e_salt = PBKDF2::Salt();

	CryptoPP::SecByteBlock derivedKey = PBKDF2::DeriveKey(salt, MasterPassword1);

	s_salt = PBKDF2::Encode(salt);
	s_derivedKey = PBKDF2::Encode(derivedKey);
	es_salt = PBKDF2::Encode(e_salt);
	FileOperations.MPWrite(s_salt, s_derivedKey, es_salt);
}

void PBKDF2::Login(FileIO& FileOperations) {
	std::string MasterPassword;
	std::cout << "Please enter the master password: ";
	std::cin >> MasterPassword;

	std::string salt = FileOperations.MPSearch().salt;
	std::string pwordhash = FileOperations.MPSearch().hash;
	CryptoPP::SecByteBlock b_salt = PBKDF2::Decode(salt);
	CryptoPP::SecByteBlock derivedKey = PBKDF2::DeriveKey(b_salt, MasterPassword);
	CryptoPP::SecByteBlock d_derivedKey = PBKDF2::Decode(pwordhash);

	if (derivedKey == d_derivedKey) {
		std::cout << "Password correct, logging in..." << std::endl;
	}
	else {
		std::cout << "Password incorrect, try again." << std::endl;
		PBKDF2::Login(FileOperations);
	}
}

CryptoPP::SecByteBlock PBKDF2::DeriveKey(CryptoPP::SecByteBlock salt, std::string& MasterPassword) {
	CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> key;
	const CryptoPP::byte* b_password = reinterpret_cast<const CryptoPP::byte*>(MasterPassword.data());
	CryptoPP::SecByteBlock derivedKey(CryptoPP::AES::DEFAULT_KEYLENGTH); //make a buffer to hold the derived key
	key.DeriveKey(
		derivedKey.BytePtr(), //output key buffer - this just tells the DeriveKey function where to put the key
		derivedKey.size(), //output key size - tells the function how big the output key should be
		0, //purpose identifer unused, set to 0
		b_password, //user's master pword - entered above, acts as the secret key for this instance of PBKDF2
		MasterPassword.size(), //pword size  - tells the function the size of the secret key
		salt.BytePtr(), //salt buffer - tells the function where to find the salt
		salt.size(), //salt size - tells the function how big the salt is going to be
		500000 //number of iterations
	);

	return derivedKey;
}