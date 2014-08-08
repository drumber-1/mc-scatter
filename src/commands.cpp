#include <vector>
#include <string>
#include <iostream>

#include "commands.h"
#include "mcscatter.h"
#include "console.h"
#include "para.h"

Console::ReturnCode print_info(std::vector<std::string> input);
Console::ReturnCode do_slices(std::vector<std::string> input);
Console::ReturnCode do_scatter(std::vector<std::string> input);
Console::ReturnCode do_colden(std::vector<std::string> input);
Console::ReturnCode clear_grid(std::vector<std::string> input);
Console::ReturnCode set_data_location(std::vector<std::string> input);
Console::ReturnCode add_image(std::vector<std::string> input);

void commands::init() {
	Console::get_instance().register_command("info", print_info);
	Console::get_instance().register_command("slice", do_slices);
	Console::get_instance().register_command("colden", do_colden);
	Console::get_instance().register_command("scatter", do_scatter);
	Console::get_instance().register_command("clear", clear_grid);
	Console::get_instance().register_command("dataloc", set_data_location);
	Console::get_instance().register_command("addimage", add_image);
}

Console::ReturnCode print_info(std::vector<std::string> input) {
	
	if(input.size() != 2) {
		if(para::get_process_rank() == 0) {
			std::cout << "Usage: " << input[0] << " grid|image|misc\n";
		}
		return Console::ReturnCode::Error;
	}

	if(input[1] == "grid") {
		MCScatter::get_instance().get_grid().print_info();
	} else if(input[1] == "image") {
		MCScatter::get_instance().print_image_info();
	} else if(input[1] == "misc") {
		MCScatter::get_instance().print_misc_info();
	} else {
		if(para::get_process_rank() == 0) {
			std::cout << "Usage: " << input[0] << " grid|image|misc\n";
		}
		return Console::ReturnCode::Error;
	}
	
	return Console::ReturnCode::Good;
}

Console::ReturnCode do_slices(std::vector<std::string> input) {
	std::string dl = MCScatter::get_instance().get_data_location();
	MCScatter::get_instance().get_grid().output_slices(dl, 0);
	return Console::ReturnCode::Good;
}

Console::ReturnCode do_scatter(std::vector<std::string> input) {
	if(input.size() != 2) {
		if(para::get_process_rank() == 0) {
			std::cout << "Usage: " << input[0] << " nphotons\n";
		}
		return Console::ReturnCode::Error;
	}
	int nphotons = std::stoi(input[1]);
	MCScatter::get_instance().do_scatter_simulation(nphotons);
	return Console::ReturnCode::Good;
}

Console::ReturnCode do_colden(std::vector<std::string> input) {
	MCScatter::get_instance().do_colden_calculation();
	return Console::ReturnCode::Good;
}

Console::ReturnCode clear_grid(std::vector<std::string> input) {
	MCScatter::get_instance().clear_grid();
	return Console::ReturnCode::Good;
}

Console::ReturnCode set_data_location(std::vector<std::string> input) {
	if(input.size() != 2) {
		if(para::get_process_rank() == 0) {
			std::cout << "Usage: " << input[0] << " dir\n";
		}
		return Console::ReturnCode::Error;
	}
	
	MCScatter::get_instance().set_data_location(input[1]);
	
	return Console::ReturnCode::Good;
}

Console::ReturnCode add_image(std::vector<std::string> input) {
	if(input.size() != 4) {
		if(para::get_process_rank() == 0) {
			std::cout << "Usage: " << input[0] << " colden|scatter theta phi\n";
		}
		return Console::ReturnCode::Error;
	}
	
	double theta = std::stod(input[2]);
	double phi = std::stod(input[3]);
	
	MCScatter::get_instance().add_image(theta, phi, input[1]);
	return Console::ReturnCode::Good;
}



