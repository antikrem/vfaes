#ifndef __KEY_HPP__

#define __KEY_HPP__

#include <string>
#include <algorithm>

#include "int128.h"
#include "random.hpp"

namespace Key {
	/**
	 * Given a string, generates a 128 bit key
	 */
	int128 hash_key(std::string key);

	/**
	 * Takes a block and returns a printable string of hex values
	 */
	std::string convert_key_to_hex_string(int128 block);

	/**
	 * Generates a random nonce
	 */
	int128 create_random_nonce();

}



#endif // !__KEY_HPP__

