#ifndef FILEIOLUA
#define FILEIOLUA

#include <string>

#include "fileio.h"
#include "grid.h"

class FileIOLua : public FileIOInterface {
	private:
		Grid read_file(std::string, const GridParameters&);
		void write_file(std::string, const Grid&);
};

#endif
