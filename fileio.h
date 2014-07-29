#pragma once

#include <string>
#include "grid.h"

class FileIOInterface {	
	public:
		virtual ~FileIOInterface() {};
		virtual Grid read_file(std::string, const GridParameters&) = 0; //TODO: Check compiler is optimizing out copying on return
		virtual void write_file(std::string, const Grid&) = 0;
};
