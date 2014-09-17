#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "grid.h"

class FileIOInterface {
	public:
		static std::unordered_map<std::string, std::unique_ptr<FileIOInterface>> fileio_map;
		static void init();
		static bool file_type_supported(std::string filetype);
		static Grid read_file(std::string filetype, std::string filename, const GridParameters& gp);
		static void write_file(std::string filetype, std::string filename, const Grid& grid);
		virtual ~FileIOInterface() {};
	private:
		virtual Grid read_file(std::string, const GridParameters&) = 0; //TODO: Check compiler is optimizing out copying on return
		virtual void write_file(std::string, const Grid&) = 0;
};
