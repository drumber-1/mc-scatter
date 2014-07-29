#pragma once

#include <string>
#include "fileio.h"
#include "grid.h"

class FileIOMG : public FileIOInterface {
	public:
		Grid read_file(std::string, const GridParameters&);
		void write_file(std::string, const Grid&);
};
