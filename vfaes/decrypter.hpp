#ifndef __DECRYPTER_HPP__

#define __DECRYPTER_HPP__

#include <string>
#include <fstream>

#include "encrypt.h"
#include "parameters.hpp"

/**
 * Manages the decryption of a single file
 */
class Decrypter {

	// Name of the original file being encypted
	std::string name;

	// Length of original file in bytes
	size_t length;

	// Size of encryption target (includes block of original length and padding)
	// In bytes
	size_t realsize;
	int128* body = nullptr;

	// Unencrypted header
	VFAESDUnencryptedHeader header;
	VFAESDEncryptedHeader eheader;

public:

	// Static function that must be run once before
	// Decryption begins
	static void initialise_decrypter(const Parameters& parameter) {
		initialise_decryption(parameter.get_key(), parameter.get_nonce());
	}

	Decrypter(const Parameters& parameter) {
		this->name = parameter.get_target();
		std::ifstream fileStream(this->name, std::ios::binary);

		// Get real length of file
		fileStream.seekg(0, std::ios::end);
		realsize = (size_t)fileStream.tellg() - 3 * sizeof(int128);
		fileStream.seekg(0, std::ios::beg);

		// Allocate buffer
		body = (int128*)malloc(realsize);


		// Read into buffer
		fileStream.read((char*)&header, 3 * sizeof(int128));
		fileStream.read((char*)body, realsize);
		fileStream.close();

		//	remove(this->name.c_str());
	}

	

	bool decrypt(size_t offset) {
		decrypt_blocks(realsize, offset, body);
		memcpy(&eheader, body, sizeof(int128));

	}

	void save(const Parameters& parameter) {

		// Write file
		std::ofstream fileStream(parameter.get_output(), std::ios::binary);
		fileStream.write((char*)body + sizeof(int128), eheader.length);
		fileStream.close();
	}

	~Decrypter() {
		

		free((void*)body);
	}
};




#endif // !__ENCRYPTER_HPP__