#include <iostream>
#include <string>
#include "log.h"
#include "para.h"

LogStream logs::err(logs::LevelFlag::ERROR);
LogStream logs::out(logs::LevelFlag::BASE);
LogStream logs::warn(logs::LevelFlag::WARNING);
LogStream logs::verbose(logs::LevelFlag::VERBOSE);
LogStream logs::debug(logs::LevelFlag::DEBUG);

logs::LevelFlag logs::current_level = logs::LevelFlag::BASE;

using manip = std::ostream& (*)(std::ostream&);

LogStream::LogStream(logs::LevelFlag lf): level_flag(lf),
                                          file_output(false),
                                          filename("scatter.log") {
	reset_flags();
}

LogStream& operator<<(LogStream& ls, manip manipulator) {
	if ((ls.process_flag == logs::ProcessFlag::ALL || para::get_process_rank() == 0) &&
	    (logs::current_level >= ls.level_flag)) {
		std::cout << manipulator;
	}
	return ls;
}

LogStream& operator<<(LogStream& ls, logs::ProcessFlag pf) {
	ls.process_flag = pf;
	return ls;
}

void LogStream::reset_flags() {
	process_flag = logs::ProcessFlag::MASTER;
}

void LogStream::set_file_output(bool foutput) {
	file_output = foutput;
}

void LogStream::set_filename(std::string fname) {
	filename = fname;
}

