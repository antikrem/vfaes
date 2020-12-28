#ifndef __ENCRYPT_H__

#define __ENCRYPT_H__

#include "int128.h"

#ifdef __cplusplus 
extern "C" {
#endif

/**
 * Declared in relevent pages (encrypt.sub.asm)
 */
int _cdecl initialise_encryption_env(void*, void*);
int _cdecl encrypt(void*);
int _cdecl initialise_decryption_env(void*, void*);
int _cdecl decrypt(void*);


/**
 * Neccessary to be run atleast once before encryption for correct performance
 */
void initialise_encryption(int128 key, int128 nonce);

/**
 * Given setup in initialise_encryption, encrypts startptr in place 
 */
void encrypt_blocks(size_t size, size_t offset, int128* startptr);

/**
 * Neccessary to be run atleast once before decryption for correct performance
 */
void initialise_decryption(int128 key, int128 nonce);
/**
 * Given setup in initialise_decryption, decrypts startptr in place
 */
void decrypt_blocks(size_t size, size_t offset, int128* startptr);

#ifdef __cplusplus 
}
#endif

#endif // !__ENCRYPT_H__

