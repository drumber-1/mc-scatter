#include <string>
#include <cassert>

#include "command.h"
#include "log.h"

Command::Command(std::string name, std::string description, CommandFunction function) {
	cmd_name = name;
	description = description;
	cmd_function = function;
}

Command::ReturnCode Command::call(const std::vector<std::string>& input) const {
	if (input.size() != required_arguments.size()) {
		print_usage();
		return ReturnCode::Error;
	}
	cmd_function(parse_arguments(input));
	return ReturnCode::Good;
}

std::string Command::name() const {
	return cmd_name;
}

void Command::print_usage() const {
	logs::out << "Usage: " << cmd_name;
	for (auto arg : required_arguments) {
		logs::out << " <" << arg.first << ">";
	}
	logs::out << "\n";
}

void Command::print_help() const {
	logs::out << cmd_name << "\n";
	logs::out << "\t" << description << "\n";
	logs::out << "\n";

	print_usage();
	logs::out << "\n";
	for (auto arg : required_arguments) {
		logs::out << arg.first << "\t" << arg.second;
	}
}

void Command::add_argument(std::string name, std::string description) {
	required_arguments.push_back(CommandArgument(name, description));
}

Command::ArgumentMap Command::parse_arguments(const std::vector<std::string> & input) const {
	
	//This should be guaranteed by Command::call
	assert (input.size() == required_arguments.size()); 
	
	ArgumentMap map;
	for (unsigned int i = 0; i < input.size(); i++) {
		map[required_arguments[i].first] = input[i];
	}
	
	return map;
}
