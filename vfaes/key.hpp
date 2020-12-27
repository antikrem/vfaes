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
	int128 hash_key(std::string key) {

		int128 hash;
		clear_int128(&hash);

		// Trash algo
		// Keep iterating over the key, until hash is populated
		// Or go until each character in the key is used
		for (size_t i = 0; i < std::max(sizeof(int128), key.size()); i++) {
			hash.data[i % sizeof(int128)] ^= key[i % key.size()];
		}

		return hash;
	}

	/**
	 * Generates a random nonce
	 */
	int128 create_random_nonce() {
		int128 result;
		clear_int128(&result);

		// Set all bytes to be random
		for (int i = 0; i < sizeof(int128); i++) {
			Random::gen_rand_byte(result.data + i);
			
		}

		return result;
	}

}



#endif // !__KEY_HPP__

