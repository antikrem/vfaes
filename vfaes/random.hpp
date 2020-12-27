#ifndef __RANDOM_HPP__

#define __RANDOM_HPP__

namespace Random {
	// Initialises random engine
	void initialise_random();

	// Gets a random char
	void gen_rand_byte(unsigned char* byte);
}

#endif // !__RANDOM_HPP__

