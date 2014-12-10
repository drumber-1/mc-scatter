#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include "para.h"
#include "fileio.h"
#include "fileio_mg.h"
#include "fileio_lua.h"

std::unordered_map<std::string, std::unique_ptr<FileIOInterface>> FileIOInterface::fileio_map;

void FileIOInterface::init() {
	fileio_map["mg"] = std::unique_ptr<FileIOMG>(new FileIOMG);
	fileio_map["lua"] = std::unique_ptr<FileIOLua>(new FileIOLua);
}

bool FileIOInterface::file_type_supported(std::string filetype) {
	if (fileio_map.count(filetype) > 0) {
		return true;
	} else {
		return false;
	}
}

Grid FileIOInterface::read_file(std::string filetype, std::string filename, const GridParameters& gp) {
	std::unordered_map<std::string, std::unique_ptr<FileIOInterface>>::iterator it = fileio_map.find(filetype);
	if (it != end(fileio_map)) {
		return (it->second)->read_file(filename, gp);
	} else {
		Grid grid;
		return grid;
	}
}

void FileIOInterface::write_file(std::string filetype, std::string filename, const Grid& grid) {
	std::unordered_map<std::string, std::unique_ptr<FileIOInterface>>::iterator it = fileio_map.find(filetype);
	if (it != end(fileio_map)) {
		return (it->second)->write_file(filename, grid);
	} else {
		return;
	}
}
