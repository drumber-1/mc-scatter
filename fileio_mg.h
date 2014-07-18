#include <string>
#include "fileio.h"

class FileIOMG : public FileIOInterface {
	FileMetadata getMetadata(std::string fileName);
	std::string getName();
	bool readFile(std::string fileName);
	bool writeFile(std::string fileName);
};
