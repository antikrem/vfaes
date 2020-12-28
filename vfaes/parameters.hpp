#ifndef __PARAMETERS_HPP__

#define __PARAMETERS_HPP__


#include <string>

#include "int128.h"
#include "random.hpp"

#include "key.hpp"
#include "modes.hpp"
#include "vfaesd_format.hpp"


class Parameters {
	// Target file for encryption
	std::string target;

	// Output file
	std::string output;

	// Mode for this execution
	Modes mode = Modes::unset;

	// Key used for encryption
	int128 key;

	// Nonce used with AES in CTR mode
	int128 nonce;

	// Number of threads used
	size_t threads = 1;

public:

	
	// Set parameters from command line
	Parameters(int argc, char** argv) {
		for (int i = 1; i < argc; i++) {
			if (std::string(argv[i]) == "-key") {
				i++;
				key = Key::hash_key(*(argv + i));
			}
			else {
				target =  argv[i];
			}

		}

		nonce = Key::create_random_nonce();
	}

	// Returns false if not enough parameters were set
	bool check() {
		// Check target set
		if (!target.size()) {
			return false;
		}

		// Worked out
		return true;
	}

	// Based on initial parameters, finalises remaining parameters
	void finalise() {

		// Set mode based on target end if unset
		if (mode == Modes::unset) {
			if (VFAESD::check_vsfed_end(target)) {
				mode = Modes::file_decrypt;
			}
			else {
				mode = Modes::file_encrypt;
			}
		}

		// Set output based on input and mdoe
		if (output.size() == 0) {
			if (mode == Modes::file_decrypt && VFAESD::check_vsfed_end(target)) {
				output = VFAESD::remove_vsfed_end(target);
			}
			else if (mode == Modes::file_encrypt) {
				output = target + VFAESD::VFAESD_SUFFIX;
			}
		}
	}

	std::string get_target() const {
		return target;
	}

	std::string get_output() const {
		return output;
	}

	Modes get_mode() const {
		return mode;
	}

	int128 get_key() const {
		return key;
	}

	int128 get_nonce() const {
		return nonce;
	}

	// Returns true when prefile should be deleted
	bool get_delete_after() {
		return true;
	}

};



#endif // !__KEY_HPP__

