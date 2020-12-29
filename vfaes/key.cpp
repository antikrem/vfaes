#include "key.hpp"

#include <sstream>

int128 Key::hash_key(std::string key) {

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

std::string Key::convert_key_to_hex_string(int128 block) {
	std::stringstream ss;
	for (int i = 0; i < sizeof(int128); ++i) {
		ss << std::hex << (int)block.data[i];
	}
		
	return ss.str();
}

int128 Key::create_random_nonce() {
	int128 result;
	clear_int128(&result);

	// Set all bytes to be random
	for (int i = 0; i < sizeof(int128); i++) {
		Random::gen_rand_byte(result.data + i);
	}

	return result;
}


