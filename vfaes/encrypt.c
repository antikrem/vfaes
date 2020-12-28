#include "encrypt.h"


void initialise_encryption(int128 key, int128 nonce) {
	initialise_encryption_env(key.data, nonce.data);
}

void encrypt_blocks(size_t size, size_t offset, int128* startptr) {
	for (unsigned long long i = 0; i < size / sizeof(int128); i++) {
		encrypt((void*)(startptr + i));
	}
}

void initialise_decryption(int128 key, int128 nonce) {
	initialise_decryption_env(key.data, nonce.data);
}

void decrypt_blocks(size_t size, size_t offset, int128* startptr) {
	for (unsigned long long i = 0; i < size / sizeof(int128); i++) {
		decrypt((void*)(startptr + i));
	}
}
