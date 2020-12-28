#ifndef __ENCRYPTION_MANAGER_HPP__

#define __ENCRYPTION_MANAGER_HPP__

#include <map>
#include <iostream>

#include "encrypter.hpp"
#include "decrypter.hpp"

class EncryptionManager {
	// Reference to parameters
	// At this point, references can't be changed
	const Parameters& parameters;

	// Look up for action
	static const std::map<Modes, int(EncryptionManager::*)()> ACTION_LOOKUP;

	// Function ptr to action, set at runtime
	int(EncryptionManager::*action)()  = nullptr;

private:
	int single_file_encrypt() {
		Encrypter::initialise_encrypter(parameters);
		Encrypter e(parameters);
		e.encrypt(0);
		e.save(parameters);

		return 0;
	}

	int single_file_decrypt() {
		Decrypter::initialise_decrypter(parameters);
		Decrypter e(parameters);
		if (e.decrypt(0)) {
			e.save(parameters);
			return 0;
		}
		else {
			std::cerr << "Invalid key: " << parameters.get_input_key() << " (" << parameters.get_key().data << ")" << std::endl;
			return 1;
		}

	}

public:
	EncryptionManager(Parameters& parameters)
	: parameters(parameters) {
		// Set action
		action = ACTION_LOOKUP.find(parameters.get_mode())->second;
	}

	// Works action and returns exit value
	int do_action() {
		
		return (this->*action)();
	}
};

#endif // !__ENCRYPTION_MANAGER_HPP__

