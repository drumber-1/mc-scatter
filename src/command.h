#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

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
		
		//Maps names of arguments to their values
		using ArgumentMap = std::unordered_map<std::string, std::string>;
	
		//The function that determines the behaviour of the command
		using CommandFunction = std::function<ReturnCode(ArgumentMap)>;

		Command(std::string name, CommandFunction function);
		
		ReturnCode call(const std::vector<std::string>& input);
		void print_usage();
		void print_help();
		
	private:
	
		std::string cmd_name;
		std::string description = "";
		CommandFunction cmd_function;
		std::vector<CommandArgument> required_arguments;
		
		ArgumentMap parse_arguments(const std::vector<std::string> &);
		
};
