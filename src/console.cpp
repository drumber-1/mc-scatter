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
#include "command.h"
#include "para.h"
#include "log.h"
#include "lua.h"

Console::Console(std::string inital_prompt) : prompt(inital_prompt) {
	
	//Set custom completor
	rl_attempted_completion_function = &Console::get_command_completions;
	
	//Hardcoded commands:
	register_command("help", "Prints a list of commands", 
	[this](Command::ArgumentMap args) {
		std::vector<std::string> cmds = get_commands();
		logs::out << "Valid commands are:\n";
		for (auto& cmd : cmds) {
			logs::out << "\t" << cmd << "\n";
		}
		return Command::ReturnCode::Good;
	});
	
	register_command("exit", "Exit mc-scatter", 
	[this](Command::ArgumentMap args) {
		return Command::ReturnCode::Exit;
	});
	
	Command cmd_run("run", "Executes all commands, line by line, in a given file", 
	[this](Command::ArgumentMap args) {
		return run_script(args["filename"]);
	});
	cmd_run.add_argument("filename", "Name of the file to be run");
	register_command(cmd_run);
	
	Command cmd_runlua("runlua", "Executes all command in the \"command\" table of a lua script", 
	[this](Command::ArgumentMap args) {
		return run_lua_script(args["filename"]);
	});
	cmd_runlua.add_argument("filename", "Name of the lua script");
	register_command(cmd_runlua);
}

Console::~Console() {
}

Console& Console::get_instance() {
	static Console instance("mc-scatter> ");
	return instance;
}

void Console::register_command(const std::string& name,
                               const std::string& description,
                               const Command::CommandFunction& function) {
    //It would be nice to use emplace here, but it does seem to work with g++ 4.7.2
	register_command(Command(name, description, function));
}

void Console::register_command(const Command& cmd) {
	command_map.insert(std::pair<std::string, Command>(cmd.name(), cmd));
}

std::vector<std::string> Console::get_commands() const {
	std::vector<std::string> cmds;
	for (auto& cmd_pair : command_map) {
		cmds.push_back(cmd_pair.first);
	}
	return cmds;
}

Command::ReturnCode Console::run_command(const std::string& raw_input) {
	std::vector<std::string> split_input;
	
	//Split command up by whitespace
	std::istringstream iss(raw_input);
	std::copy(std::istream_iterator<std::string>(iss),
	          std::istream_iterator<std::string>(),
	          std::back_inserter(split_input));
	
	if (split_input.size() == 0) {
		return Command::ReturnCode::Good;
	}

	//Some hardcoded ways to quit the program
	if (split_input[0] == "quit" || split_input[0] == "exit" || split_input[0] == "q") {
		return Command::ReturnCode::Exit;
	}
	
	CommandMap::iterator it = command_map.find(split_input[0]);
	if (it != end(command_map)) {
		//Need to remove the name of the command from the split_input
		std::vector<std::string>::const_iterator start = split_input.begin() + 1;
		std::vector<std::string>::const_iterator end = split_input.end();
		return (it->second).call(std::vector<std::string>(start, end)); //Call the function
	}
	
	return Command::ReturnCode::Good;
}

Command::ReturnCode Console::run_script(const std::string& filename) {
	std::ifstream file_input(filename);
	if (!file_input.good()) {
		logs::err << "Could not open script: " << filename << "\n";
		return Command::ReturnCode::Error;
	}
	
	std::string command;
	int counter = 0;
	Command::ReturnCode result;
	
	while (std::getline(file_input, command)) {
		if (command[0] == '#') {
			//Skip comments
			continue;
		}
		logs::out << "[" << counter << "] " << command << '\n';
		result = run_command(command);
		if (result != Command::ReturnCode::Good) {
			return result;
		}
		counter++;
	}
	
	return Command::ReturnCode::Good; 
}

Command::ReturnCode Console::run_lua_script(const std::string& filename) {

	lua_State* ls;
	ls = lua::open_file(filename);
	
	std::vector<std::string> commands = lua::get_string_table(ls, "commands");
	
	int counter = 0;
	Command::ReturnCode result;
	for (auto c : commands) {
		logs::out << "[" << counter << "] " << c << '\n';
		result = run_command(c);
		if (result != Command::ReturnCode::Good) {
			return result;
		}
		counter++;
	}
	
	return Command::ReturnCode::Good; 
}

Command::ReturnCode Console::read_line() {
	int input_bad = 0;
	std::string line;
	if (para::get_process_rank() == 0) {
		char* input;
		input = readline(prompt.c_str());
		if (!input) {
			logs::out << "\n";
			input_bad = 1;
		} else {
			if (input[0] != '\0') {
				add_history(input);
			}
			line = std::string(input);
			free(input);
		}
	}
	
	para::broadcast(&input_bad);
	
	if (input_bad == 1) {
		return Command::ReturnCode::Exit;
	}
	
	para::broadcast(line);
	
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

