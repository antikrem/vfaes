#ifndef __INT128_H__

#define __INT128_H__

#include "stdint.h"

// Size in bytes
#define SIZEOF128 16

/** Definition for a struct representing a 128 bit unsigned interger
 */
struct int128 {
	unsigned char data[SIZEOF128];
};

/**
 * Clears a 128, i.e. sets all bytes to zero
 */
void clear_int128(int128* dst) {
	memset((void*)dst, 0, sizeof(int128));
}

/**
 * Sets a 128 to be equal to a another
 */
void assign_int128(int128* dst, int128* src) {
	memcpy((void*)dst, (void*)src, sizeof(int128));
}

/**
 * Sets the 128 bits to have the value of the input string
 * will be padded with zeros
 * Returns the number of bytes written
 */
size_t write_int128(int128* dst, const char* ptr) {
	
	clear_int128(dst);

	unsigned int i = 0;
	for (; ptr[i] && i < sizeof(int128); i++) {
		dst->data[i] = ptr[i];
	}

	return i + 1;
}




#endif // !__INT128_H__

