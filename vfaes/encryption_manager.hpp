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

	// Times for different events
	std::chrono::microseconds readDuration = std::chrono::microseconds(0);
	std::chrono::microseconds encryptDuration = std::chrono::microseconds(0);
	std::chrono::microseconds writeDuration = std::chrono::microseconds(0);

	// Internal start timer
	std::chrono::time_point<std::chrono::high_resolution_clock> start;

	// Set start
	void set_start() {
		start = std::chrono::high_resolution_clock::now();
	}

	// Returns duration from last start
	std::chrono::microseconds since_start() {
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
	}

	// Converts a count of blocks and 
	double convert_mebabytes_per_second(size_t blocks, std::chrono::microseconds time) {
		return (double)(blocks * 16) / time.count();
	}

	// Stanadrd single file verbose output
	void standard_single_file_out(size_t blocks, std::string task) {
		std::cout << "Successfully " << task << " " << parameters.get_target() << " with AES in CTR Mode" << std::endl;
		std::cout << blocks << " blocks encrypted with key: " << parameters.get_input_key() << std::endl;
		std::cout << "Hard Key : " << Key::convert_key_to_hex_string(parameters.get_key()) << std::endl;
		std::cout << "Nonce    : " << Key::convert_key_to_hex_string(parameters.get_nonce()) << std::endl;
		std::cout << "Output   : " << parameters.get_output() << std::endl;
		std::cout << "Read     : " << convert_mebabytes_per_second(blocks, readDuration) << " MB/s" << std::endl;
		std::cout << "Encrypt  : " << convert_mebabytes_per_second(blocks, encryptDuration) << " MB/s" << std::endl;
		std::cout << "save     : " << convert_mebabytes_per_second(blocks, writeDuration) << " MB/s" << std::endl;
	}

private:
	int single_file_encrypt() {
		Encrypter::initialise_encrypter(parameters);

		set_start();
		Encrypter e(parameters);
		readDuration += since_start();

		set_start();
		size_t blocks = e.encrypt(0);
		encryptDuration += since_start();

		set_start();
		e.save(parameters);
		writeDuration += since_start();

		if (parameters.get_verbose_mode()) {
			standard_single_file_out(blocks, "encrypted");
		}

		return 0;
	}

	int single_file_decrypt() {
		Decrypter::initialise_decrypter(parameters);

		set_start();
		Decrypter e(parameters);
		readDuration += since_start();

		set_start();
		size_t blocks;
		if (blocks = e.decrypt(0)) {
			encryptDuration += since_start();

			set_start();
			e.save(parameters);
			writeDuration += since_start();

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
		
		
		int result = (this->*action)();
		

		return result;
	}
};

#endif // !__ENCRYPTION_MANAGER_HPP__

