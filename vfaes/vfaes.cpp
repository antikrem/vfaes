// vfaes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "encryption_manager.hpp"



int main(int argc, char** argv)
{
	Random::initialise_random();


	Parameters parameters(argc, argv);
	parameters.check();
	parameters.finalise();

	EncryptionManager manager(parameters);
	manager.do_action();


	return 0;
}