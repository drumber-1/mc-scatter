#include <cstdlib>
#include "mcscatter.h"
#include "para.h"
#include "console.h"
#include "random.h"
#include "problem.h"
#include "commands.h"
#include "fileio.h"

void dispose(); //Clean up
void term(); //Exit safely, as failure

int main(int argc, char *argv[]){

	para::init_para(argc, argv);
	random_gen::init();
	problem::init();
	commands::init();
	FileIOInterface::init();

	//Ensure the mcscatter object is created (and created now)
	MCScatter::get_instance();
	
	Console::ReturnCode return_code = Console::ReturnCode::Exit;
	do {
		return_code = Console::get_instance().read_line();
	} while(return_code != Console::ReturnCode::Exit);
	
	para::barrier();
	dispose();
	return 0;
}

void dispose(){
	para::dispose_para();
}

void term(){
	para::dispose_para();
	exit(EXIT_FAILURE);
}
