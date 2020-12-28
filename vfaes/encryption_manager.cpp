#include "encryption_manager.hpp"

const  std::map<Modes, int(EncryptionManager::*)()> EncryptionManager::ACTION_LOOKUP = {
	{Modes::file_encrypt, &EncryptionManager::single_file_encrypt},
	{Modes::file_decrypt, &EncryptionManager::single_file_decrypt}
};