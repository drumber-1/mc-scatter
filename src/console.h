#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include "command.h"

class Console {
	public:
		
		static Console& get_instance();
		std::vector<std::string> get_commands() const;
		void register_command(const std::string& name, const std::string& description, const Command::CommandFunction& function);
		void register_command(const Command& cmd);
		Command::ReturnCode run_command(const std::string& command);
		Command::ReturnCode run_script(const std::string& filename);
		Command::ReturnCode run_lua_script(const std::string& filename);
		Command::ReturnCode read_line();
		Command::ReturnCode print_help();
	private:
		Console(std::string prompt);
		~Console();
		Console(const Console&);
		void operator=(const Console&);		
		
		//Maps strings (taken from stdin from user) to the functions to execute
		using CommandMap = std::unordered_map<std::string, Command>;
		
		//To interface with GNU readline
		//They need to be static, as rl_completion_matches takes a function pointer
		//but NOT a member function pointer
		static char** get_command_completions(const char* text, int start, int end);
		static char* command_iterator(const char* text, int state);
		
		std::string prompt;
		CommandMap command_map;
};

