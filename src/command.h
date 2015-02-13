#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <utility>

class Command {
	public:
		enum ReturnCode {
			Exit = -1,
			Good = 0,
			Error = 1
		};
		
		//Maps names of arguments to their values
		using ArgumentMap = std::unordered_map<std::string, std::string>;
	
		//The function that determines the behaviour of the command
		using CommandFunction = std::function<ReturnCode(ArgumentMap)>;
		
		//A name/description pair for single argument
		using CommandArgument = std::pair<std::string, std::string>;
		
		Command(std::string name, std::string description, CommandFunction function);
		
		ReturnCode call(const std::vector<std::string>& input) const;
		std::string name() const;
		void print_usage() const;
		void print_help() const;
		void add_argument(std::string name, std::string description);
		
	private:
		std::string cmd_name;
		std::string description = "";
		CommandFunction cmd_function;
		std::vector<CommandArgument> required_arguments;
		
		ArgumentMap parse_arguments(const std::vector<std::string> &) const;
		
};
