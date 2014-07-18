#include <string>

struct FileMetadata {
	double grid_spacing;
	double time_stamp;
};

class FileIOInterface {	
	public:
		virtual ~FileIOInterface() {};
		virtual FileMetadata getMetadata(std::string fileName) = 0;
		virtual std::string getName() = 0;
		virtual bool readFile(std::string fileName) = 0;
		virtual bool writeFile(std::string fileName) = 0;
};
