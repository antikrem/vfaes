/**
 * Overview and constants for the vfaesd file format
 */

#ifndef __VFAESD_HPP__

#define __VFAESD_HPP__

#include <string>

/**
 * Format:
 *
 * 0000: 16 byte unencrypted identifier 
 * 0128: 16 byte random nonce
 * 0256: -- 8 byte unsigned counter
 * 0320: -- 1 byte flag set to true if encrypted check block is not randomised
 * 0328: -- 7 bytes of padding
 * 
 * 0384: 8 byte encrypted check block (or random padding if check flag not set)
 * 0448: 8 byte encrypted unsigned int size of original file
 * 
 * 0512: Encrypted data section begins
 * 
 */


namespace VFAESD {
	// File format suffix
	const std::string VFAESD_SUFFIX = ".vfaesd";

	// Size of unencrypted block
	const size_t UNENCRYPTED_BLOCK_SIZE = 3;

	// Block offset for id
	const size_t ID_BLOCK_OFFSET = 0;
	// 16 byte identifier
	const char* UNENCRYPTED_IDENTIFIER = "!VFAES_ENCRYPTED";

	// Block offset for nonce
	const size_t NONCE_BLOCK_OFFSET = 1;

	// Block offset for counter 
	const size_t COUNTER_BLOCK_OFFSET = 2;


	// Block offset for encrypted meta data
	const size_t ENCRYPTED_META_BLOCK_OFFSET = 3;
	const char* ENCRYPTED_CHECK_STRING = "!SECRET!";
	// Byte offset within block for original length
	const size_t LENGTH_BYTE_OFFSET = 8;


	// Block offset for encrypted data
	const size_t ENCRYPTED_DATA_BLOCK_OFFSET = 4;

	// checks if string has a VFAESD_SUFFIX
	static bool ends_vsfed(std::string str) {

	}
}

struct VFAESDUnencryptedHeader {
	int128 identifier;
	int128 nonce;
	int128 metadatablock;
};

struct VFAESDEncryptedHeader {
	char checkblock[sizeof(VFAESD::ENCRYPTED_CHECK_STRING)];
	size_t length;
};


#endif // !__VFAESD_HPP__

