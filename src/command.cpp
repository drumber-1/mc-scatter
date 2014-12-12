#include <string>
#include <cassert>

#include "command.h"
#include "log.h"

Command::Command(std::string name, CommandFunction function) {
	cmd_name = name;
	cmd_function = function;
}

Command::ReturnCode Command::call(const std::vector<std::string>& input) {
	if (input.size() != required_arguments.size()) {
		print_usage();
		return ReturnCode::Error;
	}
	cmd_function(parse_arguments(input));
	return ReturnCode::Good;
}

void Command::print_usage() {
	logs::out << "Usage: " << cmd_name;
	for (auto arg : required_arguments) {
		logs::out << " <" << arg.name << ">";
	}
	logs::out << "\n";
}

void Command::print_help() {
	logs::out << cmd_name << "\n";
	logs::out << "\t" << description << "\n";
	logs::out << "\n";

	print_usage();
	logs::out << "\n";
	for (auto arg : required_arguments) {
		logs::out << arg.name << "\t" << arg.description;
	}
}

Command::ArgumentMap Command::parse_arguments(const std::vector<std::string> & input) {
	
	//This should be guaranteed by Command::call
	assert (input.size() == required_arguments.size()); 
	
	ArgumentMap map;
	for (unsigned int i = 0; i < input.size(); i++) {
		map[required_arguments[i].name] = input[i];
	}
	
	return map;
}
