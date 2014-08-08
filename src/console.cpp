#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>
#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>
#include "console.h"

Console::Console(std::string inital_prompt) : prompt(inital_prompt), command_map({ {"quit",{}}, {"exit",{}} }) {
	
	//Set custom completor
	rl_attempted_completion_function = &Console::get_command_completions;
	
	//Hardcoded commands:
	//Help command - prints a list of the available commands
	command_map["help"] = [this](const std::vector<std::string>& input){
		std::vector<std::string> commands = get_commands();
		std::cout << "Valid commands are:\n";
		for (auto& cmd : commands) {
			std::cout << "\t" << cmd << "\n";
		}
		return ReturnCode::Good;
	};
	
	//Run command - executes all commands in a given file
	command_map["run"] = [this](const std::vector<std::string>& input) {
		if (input.size() < 2) {
			std::cout << "Usage: " << input[0] << " <script name>\n";
			return ReturnCode::Error;
		}
		return run_script(input[1]);
	};
}

Console::~Console(){
}

Console& Console::get_instance(){
	static Console instance("mc-scatter> ");
	return instance;
}

void Console::register_command(const std::string& cmd, CommandFunction function) {
	command_map[cmd] = function;
}

std::vector<std::string> Console::get_commands() const {
	std::vector<std::string> cmds;
	for (auto& cmd_pair : command_map) {
		cmds.push_back(cmd_pair.first);
	}
	return cmds;
}

Console::ReturnCode Console::run_command(const std::string& command) {
	std::vector<std::string> inputs;
	
	//Split command up by whitespace
	std::istringstream iss(command);
	std::copy(std::istream_iterator<std::string>(iss),
	          std::istream_iterator<std::string>(),
	          std::back_inserter(inputs));
	
	if (inputs.size() == 0) {
		return ReturnCode::Good;
	}

	if (inputs[0] == "quit" || inputs[0] == "exit") {
		return ReturnCode::Exit;
	}
	
	CommandMap::iterator it = command_map.find(inputs[0]);
	if(it != end(command_map)){
		return (it->second)(inputs);
	}
	
	return ReturnCode::Good;
}

Console::ReturnCode Console::run_script(const std::string& filename){
	std::ifstream file_input(filename);
	if(!file_input.good()){
		std::cerr << "Could not open script: " << filename << "\n";
		return ReturnCode::Error;
	}
	
	std::string command;
	int counter = 0;
	ReturnCode result;
	
	while (std::getline(file_input, command)) {
		if (command[0] == '#') {
			//Skip comments
			continue;
		}
		std::cout << "[" << counter << "] " << command << '\n';
		result = run_command(command);
		if (result != ReturnCode::Good) {
			return result;
		}
		counter++;
	}
	
	return ReturnCode::Good; 
}

Console::ReturnCode Console::read_line() {
	char* input = readline(prompt.c_str());
	if (!input) {
		std::cout << "\n";
		return ReturnCode::Exit;
	}
	
	if (input[0] != '\0') {
		add_history(input);
	}
	
	std::string line(input);
	free(input);
	
	return run_command(line);
}

char** Console::get_command_completions(const char* text, int start, int end) {
	char** completions = nullptr;
	
	if (start == 0) {
		completions = rl_completion_matches(text, &Console::command_iterator);
	}
	
	return completions;
}

char* Console::command_iterator(const char* text, int state) {
	CommandMap::iterator it;
	
	if (state == 0) {
		it = begin(get_instance().command_map);
	}
	
	while (it != end(get_instance().command_map)) {
		auto& command = it->first;
		it++;
		if (command.find(text) == 0) {
			char* completion = new char[command.size()];
			strcpy(completion, command.c_str());
			return completion;
		}
	}
	
	return nullptr;
}














