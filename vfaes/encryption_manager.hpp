#ifndef __ENCRYPTION_MANAGER_HPP__

#define __ENCRYPTION_MANAGER_HPP__

#include <map>
#include <iostream>

#include <chrono> 

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

	// Stanadrd single file verbose output
	void standard_single_file_out(size_t blocks, std::string task) {
		std::cout << "Successfully " << task << " " << parameters.get_target() << " with AES in CTR Mode" << std::endl;
		std::cout << blocks << " blocks encrypted with key: " << parameters.get_input_key() << std::endl;
		std::cout << "Hard Key : " << Key::convert_key_to_hex_string(parameters.get_key()) << std::endl;
		std::cout << "Nonce    : " << Key::convert_key_to_hex_string(parameters.get_nonce()) << std::endl;
		std::cout << "Output   : " << parameters.get_output() << std::endl;
	}

private:
	int single_file_encrypt() {
		Encrypter::initialise_encrypter(parameters);
		Encrypter e(parameters);
		size_t blocks = e.encrypt(0);
		e.save(parameters);

		if (parameters.get_verbose_mode()) {
			standard_single_file_out(blocks, "encrypted");
		}

		return 0;
	}

	int single_file_decrypt() {
		Decrypter::initialise_decrypter(parameters);
		Decrypter e(parameters);

		size_t blocks;
		if (blocks = e.decrypt(0)) {
			e.save(parameters);

			if (parameters.get_verbose_mode()) {
				standard_single_file_out(blocks, "decrypted");
			}

			return 0;
		}
		else {
			std::cerr << "Invalid key: " << parameters.get_input_key()
					<< " (" << Key::convert_key_to_hex_string(parameters.get_key()) 
					<< ")" << std::endl;
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
		
		auto start = std::chrono::high_resolution_clock::now();
		int result = (this->*action)();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);

		return result;
	}
};

#endif // !__ENCRYPTION_MANAGER_HPP__

