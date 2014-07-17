#include <string>
#include "fileio.h"

class FileIOMG : public FileIOInterface {
	FileMetadata getMetadata();
	std::string getName();
	std::string getExtension();
	bool readFile(std::string fileName);
	bool writeFile(std::string fileName);
};
