/* Bunch of useful os stuff
 */

#ifndef __OS_KIT__
#define __OS_KIT__

#include <string>
#include <vector>

/* All methods are in the str_kit namespace
 */
namespace os_kit {
	// Checks if file exists
	bool fileExists(const std::string& name);

	// Checks if folder exists
	bool folderExists(const std::string& name);

	// Loads all the contents of a file from a script
	// empty string on faliure
	std::string getFileAsString(const std::string& filePath);

	// Similar to getFileAsString, but sets each line
	// as a new element in string
	void getFileAsString(const std::string& filePath, std::vector<std::string>& results);

	// Counts number of new lines within a file
	// returns -1 if file does not exist
	int countNewLinesInFile(const std::string& filePath);

	// Returns a vector of all file names in a folder
	std::vector<std::string> getFilesInFolder(const std::string& filePath);

	// Returns what evers currently in the clipboard
	// Returns empty string on error
	std::string getClipboard();

	// Returns size of memory use
	size_t getVMemUsed();

	// Will attempt getting the terminal width in character columns
	// -1 return value on missing terminal or general failures
	int getTerminalWidth();
}

#endif