#include <cstdlib>
#include "mcscatter.h"
#include "console.h"
#include "random.h"
#include "commands.h"
#include "fileio.h"
#include "log.h"
#include "lua.h"

void dispose(); //Clean up
void term(); //Exit safely, as failure

int main(int argc, char *argv[]) {

	para::init_para(argc, argv);
	random_gen::init();
	commands::init();
	FileIOInterface::init();
	lua::init();

	//Ensure the mcscatter object is created (and created now)
	MCScatter::get_instance();
	
	Console::ReturnCode return_code = Console::ReturnCode::Exit;
	do {
		return_code = Console::get_instance().read_line();
	} while (return_code != Console::ReturnCode::Exit);
	
	dispose();
	return 0;
}

void dispose() {
	para::dispose_para();
}

void term() {
	para::dispose_para();
	exit(EXIT_FAILURE);
}
