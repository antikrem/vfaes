// vfaes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "encryption_manager.hpp"



int main(int argc, char** argv)
{
	Random::initialise_random();

	std::ios::sync_with_stdio(false);

	Parameters parameters(argc, argv);
	
	if (!parameters.check()) {
		return 1;
	}
	parameters.finalise();

	EncryptionManager manager(parameters);
	
	return manager.do_action();
}