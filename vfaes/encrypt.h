#ifndef __ENCRYPT_H__

#define __ENCRYPT_H__

#include "stdint.h"
#include "int128.h"

extern "C" int _cdecl initialise_encryption_env(void*, void*);
extern "C" int _cdecl encrypt(void*);
extern "C" int _cdecl initialise_decryption_env(void*, void*);
extern "C" int _cdecl decrypt(void*);

/**
 * Neccessary to be run atleast once before encryption for correct performance
 */
void initialise_encryption(int128 key, int128 nonce) {
	initialise_encryption_env(key.data, nonce.data);
}

/**
 * Given setup in initialise_encryption, encrypts startptr in place 
 */
void encrypt_blocks(size_t size, size_t offset, int128* startptr) {
	for (unsigned long long i = 0; i < size / sizeof(int128); i++) {
		encrypt((void*)(startptr + i));
	}
}

/**
 * Neccessary to be run atleast once before decryption for correct performance
 */
void initialise_decryption(int128 key, int128 nonce) {
	initialise_decryption_env(key.data, nonce.data);
}

/**
 * Given setup in initialise_decryption, decrypts startptr in place
 */
void decrypt_blocks(size_t size, size_t offset, int128* startptr) {
	for (unsigned long long i = 0; i < size / sizeof(int128); i++) {
		decrypt((void*)(startptr + i));
	}
}


#endif // !__ENCRYPT_H__

