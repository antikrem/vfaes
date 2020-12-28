#ifndef __INT128_H__

#define __INT128_H__

#include <stdint.h>
#include <stdbool.h>

// Size in bytes
#define SIZEOF128 16

/** Definition for a struct representing a 128 bit unsigned interger
 */
typedef struct {
	unsigned char data[SIZEOF128];
} int128;

#ifdef __cplusplus 
extern "C" {
#endif

	/**
	 * Clears a 128, i.e. sets all bytes to zero
	 */
	void clear_int128(int128* dst);

	/**
	 * Sets a 128 to be equal to a another
	 */
	void assign_int128(int128* dst, int128* src);

	/**
	 * Sets the 128 bits to have the value of the input string
	 * will be padded with zeros
	 * Returns the number of bytes written
	 */
	size_t write_int128(int128* dst, const char* ptr);

	/**
	 * Compares an int128 to the bytes of a string
	 * Continues until meeting a null terminal
	 */
	bool compare_int128_front_bits(int128* dst, const char* ptr);

#ifdef __cplusplus 
}
#endif

#endif // !__INT128_H__

