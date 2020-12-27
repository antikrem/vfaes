#ifndef __ENCRYPTER_HPP__

#define __ENCRYPTER_HPP__

#include <string>
#include <fstream>

#include "encrypt.h"
#include "parameters.hpp"
#include "vfaesd_format.hpp"

/**
 * Manages the encryption of a single file
 */
class Encrypter {

	// Name of the original file being encypted
	std::string name;

	// Length of original file in bytes
	size_t length;

	// Size of encryption target (includes block of original length and padding)
	// In bytes
	size_t realsize;
	int128* body = nullptr;

public:

	// Static function that must be run once before
	// encryption begins
	static void initialise_encrypter(const Parameters& parameter) {
		initialise_encryption(parameter.get_key(), parameter.get_nonce());
	}

	// Loads the target file onto memory
	// and pads
	// Also deletes the original file
	Encrypter(const std::string& name) {
		this->name = name;
		std::ifstream fileStream(this->name, std::ios::binary);

		// Get length of file
		fileStream.seekg(0, std::ios::end);
		length = fileStream.tellg();
		fileStream.seekg(0, std::ios::beg);

		// Compute real size

		realsize = (length / 16 + 2) * 16;

		// Allocate buffer
		body = (int128*)malloc(realsize);

		// Read into buffer
		fileStream.read((char*)(body + 1), length);
		fileStream.close();

	//	remove(this->name.c_str());
	}

	void encrypt(size_t offset) {

		// Set encrypted block up
		write_int128(body, VFAESD::ENCRYPTED_CHECK_STRING);
		memcpy(
			(void*)((char*)body + VFAESD::LENGTH_BYTE_OFFSET),
			(void*)&length, 
			sizeof(size_t)
		);

		encrypt_blocks(realsize, offset, body);

	}

	// Save to file
	void save(const Parameters& parameter) {
		// Write file back
		std::ofstream fileStream(name + VFAESD::VFAESD_SUFFIX, std::ios::binary);

		// Write unencrypted header
		int128 unencrypted_block[VFAESD::UNENCRYPTED_BLOCK_SIZE];

		write_int128(unencrypted_block + VFAESD::ID_BLOCK_OFFSET, VFAESD::UNENCRYPTED_IDENTIFIER);
		unencrypted_block[VFAESD::NONCE_BLOCK_OFFSET] = parameter.get_nonce();
		clear_int128(unencrypted_block + VFAESD::COUNTER_BLOCK_OFFSET);

		fileStream.write((char*)unencrypted_block, VFAESD::UNENCRYPTED_BLOCK_SIZE * sizeof(int128));
		fileStream.write((char*)body, realsize);
		fileStream.close();
	}

	~Encrypter() {
		free((void*)body);
	}
};




#endif // !__ENCRYPTER_HPP__