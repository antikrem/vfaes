#ifndef __PARAMETERS_HPP__

#define __PARAMETERS_HPP__


#include <string>

#include "int128.h"
#include "random.hpp"

#include "key.hpp"


class Parameters {

	int128 key;

	int128 nonce;

public:

	
	Parameters(std::string keystr, int128 nonce) {
		key = Key::hash_key(keystr); 
		assign_int128(&this->nonce, &nonce);
	}

	int128 get_key() const {
		return key;
	}

	int128 get_nonce() const {
		return nonce;
	}


};



#endif // !__KEY_HPP__

