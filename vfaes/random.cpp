#include "random.hpp"

#include <random>
#include <mutex>


std::default_random_engine randgen;
std::uniform_int_distribution<int> dist(0, 255);

#define RANDOM_BURN 100

void Random::initialise_random() {
	// Burn some values
	for (int i = 0; i < RANDOM_BURN; i++) {
		dist(randgen);
	}
}

void Random::gen_rand_byte(unsigned char* byte) {
	*byte = (unsigned char) dist(randgen);
}


