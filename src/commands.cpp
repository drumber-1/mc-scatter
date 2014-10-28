#include <vector>
#include <string>
#include <iostream>

#include "commands.h"
#include "mcscatter.h"
#include "console.h"
#include "log.h"
#include "fileio.h"
#include "util.h"

Console::ReturnCode print_info(std::vector<std::string> input);
Console::ReturnCode do_slices(std::vector<std::string> input);
Console::ReturnCode do_scatter(std::vector<std::string> input);
Console::ReturnCode do_colden(std::vector<std::string> input);
Console::ReturnCode clear_item(std::vector<std::string> input);
Console::ReturnCode set_data_location(std::vector<std::string> input);
Console::ReturnCode add_image(std::vector<std::string> input);
Console::ReturnCode read(std::vector<std::string> input);
Console::ReturnCode write(std::vector<std::string> input);

void commands::init() {
	Console::get_instance().register_command("info", print_info);
	Console::get_instance().register_command("slice", do_slices);
	Console::get_instance().register_command("colden", do_colden);
	Console::get_instance().register_command("scatter", do_scatter);
	Console::get_instance().register_command("clear", clear_item);
	Console::get_instance().register_command("dataloc", set_data_location);
	Console::get_instance().register_command("addimage", add_image);
	Console::get_instance().register_command("read", read);
	Console::get_instance().register_command("write", write);
}

Console::ReturnCode print_info(std::vector<std::string> input) {
	
	if (input.size() != 2) {
		logs::err << "Usage: " << input[0] << " grid|images|misc\n";
		return Console::ReturnCode::Error;
	}

	if (input[1] == "grid") {
		MCScatter::get_instance().get_grid().print_info();
	} else if (input[1] == "images") {
		MCScatter::get_instance().print_image_info();
	} else if (input[1] == "misc") {
		MCScatter::get_instance().print_misc_info();
	} else {
		logs::err << "Usage: " << input[0] << " grid|images|misc\n";
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
	if (input.size() != 2) {
		logs::err << "Usage: " << input[0] << " nphotons\n";
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

Console::ReturnCode clear_item(std::vector<std::string> input) {
	if (input.size() != 2) {
		logs::err << "Usage: " << input[0] << " grid|images\n";
		return Console::ReturnCode::Error;
	}
	
	if (input[1] == "grid") {
		MCScatter::get_instance().clear_grid();
	} else if (input[1] == "images") {
		MCScatter::get_instance().clear_images();
	} else {
		logs::err << "Usage: " << input[0] << " grid|images\n";
		return Console::ReturnCode::Error;
	}

	
	return Console::ReturnCode::Good;
}

Console::ReturnCode set_data_location(std::vector<std::string> input) {
	if (input.size() != 2) {
		logs::err << "Usage: " << input[0] << " dir\n";
		return Console::ReturnCode::Error;
	}
	
	MCScatter::get_instance().set_data_location(input[1]);
	
	return Console::ReturnCode::Good;
}

Console::ReturnCode add_image(std::vector<std::string> input) {
	if (input.size() != 4) {
		logs::err << "Usage: " << input[0] << " colden|scatter theta phi\n";
		return Console::ReturnCode::Error;
	}
	
	double theta = util::toRad(std::stod(input[2]));
	double phi = util::toRad(std::stod(input[3]));
	
	MCScatter::get_instance().add_image(theta, phi, input[1]);
	return Console::ReturnCode::Good;
}

Console::ReturnCode read(std::vector<std::string> input) {
	if (input.size() != 3) {
		logs::err << "Usage: " << input[0] << " filetype filename\n";
		return Console::ReturnCode::Error;
	}
	
	if (!FileIOInterface::file_type_supported(input[1])) {
		logs::err << input[1] << " is an unrecognised filetype\n";
		return Console::ReturnCode::Error;
	}
	
	MCScatter::get_instance().clear_grid();
	
	//These grid parameters determine the maximum number of cells to use
	//TODO: Use a default gridparameters from problem.cpp
	GridParameters gp;
	for (int i = 0; i < 3; i++) {
		gp.ncells[i] = 500;
		gp.left_boundary[i] = -2;
		gp.right_boundary[i] = 2;
	}
	
	std::string full_path = MCScatter::get_instance().get_data_location() + std::string("/") + input[2];
	
	Grid grid = FileIOInterface::read_file(input[1], full_path, gp);
	MCScatter::get_instance().set_grid(grid);
	return Console::ReturnCode::Good;
}

Console::ReturnCode write(std::vector<std::string> input) {
	if (input.size() != 3) {
		logs::err << "Usage: " << input[0] << " filetype filename\n";
		return Console::ReturnCode::Error;
	}
	
	if (!FileIOInterface::file_type_supported(input[1])) {
		logs::err << input[1] << " is an unrecognised filetype\n";
		return Console::ReturnCode::Error;
	}
	
	std::string full_path = MCScatter::get_instance().get_data_location() + std::string("/") + input[2];
	
	FileIOInterface::write_file(input[1], full_path, MCScatter::get_instance().get_grid());
	return Console::ReturnCode::Good;
}



