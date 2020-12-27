// vfaes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "encrypter.hpp"
#include "decrypter.hpp"



int main()
{
	Random::initialise_random();

	int128 nonce = Key::create_random_nonce();

	Parameters parameters("very secure key", nonce);

	Encrypter::initialise_encrypter(parameters);

	{
		Encrypter e("test.png");
		e.encrypt(0);
		e.save(parameters);
	}

	Decrypter::initialise_decrypter(parameters);
	
	{
		Decrypter e("test.png.vfaesd");
		e.decrypt(0);
		e.save(parameters);
	}

	return 0;
}