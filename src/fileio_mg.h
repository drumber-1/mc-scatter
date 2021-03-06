#ifndef FILEIOMG
#define FILEIOMG

#include <string>

#include "fileio.h"
#include "grid.h"

class FileIOMG : public FileIOInterface {
	private:
		Grid read_file(std::string, const GridParameters&);
		void write_file(std::string, const Grid&);
};

#endif
