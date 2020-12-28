// vfaes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "encrypter.hpp"
#include "decrypter.hpp"



int main(int argc, char** argv)
{
	Random::initialise_random();


	Parameters parameters(argc, argv);
	parameters.check();
	parameters.finalise();

	switch (parameters.get_mode()) {

	case Modes::file_encrypt :
		{
		Encrypter::initialise_encrypter(parameters);
		Encrypter e(parameters);
		e.encrypt(0);
		e.save(parameters);
		break;
		}

	case Modes::file_decrypt:
		{
		Decrypter::initialise_decrypter(parameters);
		Decrypter e(parameters);
		e.decrypt(0);
		e.save(parameters);
		break;
		}

	}

	return 0;
}