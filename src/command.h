#include <string>
#include <vector>
#include <functional>

class CommandArgument {
	public:
		CommandArgument(std::string name);
		std::string name;
		std::string description = "";
};

class Command {
	public:
	
		enum ReturnCode {
			Exit = -1,
			Good = 0,
			Error = 1
		};
	
		//A command function accepts a vector of strings as its arguments, and represents a single command
		using CommandFunction = std::function<ReturnCode(const std::vector<std::string> &)>;
		
		Command(std::string name, CommandFunction function);
		
		ReturnCode call(const std::vector<std::string>& input);
		void print_usage();
		void print_help();
		
	private:
		std::string cmd_name;
		std::string description = "";
		CommandFunction cmd_function;
		std::vector<CommandArgument> required_arguments;
		
};
