#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

class Console {
	public:
		enum ReturnCode {
			Exit = -1,
			Good = 0,
			Error = 1
		};
		
		//A command function accepts a vector of strings as its arguments, and represents a single command
		using CommandFunction = std::function<ReturnCode(const std::vector<std::string> &)>;
		
		static Console& get_instance();
		std::vector<std::string> get_commands() const;
		void register_command(const std::string& cmd, CommandFunction function);
		ReturnCode run_command(const std::string& command);
		ReturnCode run_script(const std::string& filename);
		ReturnCode run_lua_script(const std::string& filename);
		ReturnCode read_line();
	private:
		Console(std::string prompt);
		~Console();
		Console(const Console&);
		void operator=(const Console&);		
		
		//Maps strings (taken from stdin from user) to the functions to execute
		using CommandMap = std::unordered_map<std::string, CommandFunction>;
		
		//To interface with GNU readline
		//They need to be static, as rl_completion_matches takes a function pointer
		//but NOT a member function pointer
		static char** get_command_completions(const char* text, int start, int end);
		static char* command_iterator(const char* text, int state);
		
		std::string prompt;
		CommandMap command_map;
};

