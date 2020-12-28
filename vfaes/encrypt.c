#include "encrypt.h"

/**
 * Declared in relevent pages (encrypt.sub.asm)
 */
int _cdecl initialise_encryption_env(void*, void*);
int _cdecl encrypt_enb(void*, size_t);
int _cdecl initialise_decryption_env(void*, void*);
int _cdecl decrypt_enb(void*, size_t);
int _cdecl encrypt_ctr(void*, size_t);

// Pointers to functions used at run time
// Defaults to CTR mode, no interface for ENB yet
int (*initialise_encryption_env_runtime)(void*, void*) = initialise_encryption_env;
int (*initialise_decryption_env_runtime)(void*, void*) = initialise_encryption_env;
int (*encrypt_runtime)(void*, size_t) = encrypt_ctr;
int (*decrypt_runtime)(void*, size_t) = encrypt_ctr;


void initialise_encryption(int128 key, int128 nonce) {
	initialise_encryption_env_runtime(key.data, nonce.data);
}

void encrypt_blocks(size_t size, size_t offset, int128* startptr) {
	// Running counter variable of offset

	for (size_t i = 0; i < size / sizeof(int128); i++) {
		encrypt_runtime((void*)(startptr + i), offset + i);
	}
}

void initialise_decryption(int128 key, int128 nonce) {
	initialise_decryption_env_runtime(key.data, nonce.data);
}

void decrypt_blocks(size_t size, size_t offset, int128* startptr) {
	for (size_t i = 0; i < size / sizeof(int128); i++) {
		decrypt_runtime((void*)(startptr + i), offset + i);
	}
}
