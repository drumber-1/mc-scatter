#include <string>
#include "fileio_mg.h"

FileMetadata FileIOMG::getMetadata(){
	FileMetadata m_data;
	m_data.grid_spacing = 1.0;
	m_data.time_stamp = 0.0;
	return m_data;
}

std::string FileIOMG::getName(){
	return "mg";
}

std::string FileIOMG::getExtension(){
	return ".rho";
}

bool FileIOMG::readFile(std::string fileName){
	return false;
}

bool FileIOMG::writeFile(std::string fileName){
	return false;
}
