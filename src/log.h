#include <iostream>
#include <string>
#include "para.h"

class LogStream;

namespace logs {
	enum LevelFlag {
		ERROR = 0,
		BASE = 1,
		WARNING = 2,
		VERBOSE = 3,
		DEBUG = 4
	};
	enum ProcessFlag {
		MASTER,
		ALL
	};
	
	extern LogStream err;
	extern LogStream out;
	extern LogStream warn;
	extern LogStream verbose;
	extern LogStream debug;
	
	extern LevelFlag current_level;
}

using manip = std::ostream& (*)(std::ostream&);

class LogStream {
	public:
		LogStream(logs::LevelFlag lf);
		
		template<typename T>
		friend LogStream& operator<<(LogStream& ls, const T& t);
		friend LogStream& operator<<(LogStream& ls, manip manipulator);
		friend LogStream& operator<<(LogStream& ls, logs::ProcessFlag pf);
		
		void reset_flags();
		
		void set_file_output(bool);
		void set_filename(std::string);
	private:
		const logs::LevelFlag level_flag;
		logs::ProcessFlag process_flag; //If all the processes will provide output
		bool file_output;
		std::string filename;
};

template<typename T>
LogStream& operator<<(LogStream& ls, T const& t) {
	if ((ls.process_flag == logs::ProcessFlag::ALL || para::get_process_rank() == 0) &&
	    (logs::current_level >= ls.level_flag)) {
		std::cout << t;
	}
	return ls;
}

