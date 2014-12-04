#include <vector>
#include <string>
#include <iostream>

#include "commands.h"
#include "mcscatter.h"
#include "console.h"
#include "log.h"
#include "util.h"

//TODO create command class and clean/condense this up

Console::ReturnCode print_info(std::vector<std::string> input);
Console::ReturnCode do_slices(std::vector<std::string> input);
Console::ReturnCode do_scatter(std::vector<std::string> input);
Console::ReturnCode do_colden(std::vector<std::string> input);
Console::ReturnCode clear_item(std::vector<std::string> input);
Console::ReturnCode set_data_location(std::vector<std::string> input);
Console::ReturnCode add_image(std::vector<std::string> input);
Console::ReturnCode read(std::vector<std::string> input);
Console::ReturnCode write(std::vector<std::string> input);
Console::ReturnCode config(std::vector<std::string> input);

void commands::init() {
	Console::get_instance().register_command("info", print_info);
	Console::get_instance().register_command("slice", do_slices);
	Console::get_instance().register_command("colden", do_colden);
	Console::get_instance().register_command("scatter", do_scatter);
	Console::get_instance().register_command("clear", clear_item);
	Console::get_instance().register_command("addimage", add_image);
	Console::get_instance().register_command("read", read);
	Console::get_instance().register_command("write", write);
	Console::get_instance().register_command("config", config);
}

Console::ReturnCode print_info(std::vector<std::string> input) {
	
	if (input.size() != 2) {
		logs::err << "Usage: " << input[0] << " grid|images|config\n";
		return Console::ReturnCode::Error;
	}

	if (input[1] == "grid") {
		MCScatter::print_grid_info();
	} else if (input[1] == "images") {
		MCScatter::print_image_info();
	} else if (input[1] == "config") {
		MCScatter::print_config_info();
	} else {
		logs::err << "Usage: " << input[0] << " grid|images|config\n";
		return Console::ReturnCode::Error;
	}
	
	return Console::ReturnCode::Good;
}

Console::ReturnCode do_slices(std::vector<std::string> input) {
	MCScatter::do_slices();
	return Console::ReturnCode::Good;
}

Console::ReturnCode do_scatter(std::vector<std::string> input) {
	if (input.size() != 2) {
		logs::err << "Usage: " << input[0] << " nphotons\n";
		return Console::ReturnCode::Error;
	}
	int nphotons = std::stoi(input[1]);
	MCScatter::do_scatter_simulation(nphotons);
	return Console::ReturnCode::Good;
}

Console::ReturnCode do_colden(std::vector<std::string> input) {
	MCScatter::do_colden_calculation();
	return Console::ReturnCode::Good;
}

Console::ReturnCode clear_item(std::vector<std::string> input) {
	if (input.size() != 2) {
		logs::err << "Usage: " << input[0] << " grid|images\n";
		return Console::ReturnCode::Error;
	}
	
	if (input[1] == "grid") {
		MCScatter::clear_grid();
	} else if (input[1] == "images") {
		MCScatter::clear_images();
	} else {
		logs::err << "Usage: " << input[0] << " grid|images\n";
		return Console::ReturnCode::Error;
	}

	
	return Console::ReturnCode::Good;
}

Console::ReturnCode add_image(std::vector<std::string> input) {
	if (input.size() != 4) {
		logs::err << "Usage: " << input[0] << " colden|scatter theta phi\n";
		return Console::ReturnCode::Error;
	}
	
	double theta = util::toRad(std::stod(input[2]));
	double phi = util::toRad(std::stod(input[3]));
	
	MCScatter::add_image(theta, phi, input[1]);
	return Console::ReturnCode::Good;
}

//This function is a little messy currently
//When command class is created it should handle things like this more
//elegantly
Console::ReturnCode read(std::vector<std::string> input) {
	if (input.size() != 3 && input.size() != 4) {
		logs::err << "Usage: " << input[0] << " filetype [-d] filename\n";
		return Console::ReturnCode::Error;
	}
	
	if (input.size() == 4) {
		if (input[2] == "-d") {
			if (!MCScatter::read_grid(input[1], input[3], true)) {
				return Console::ReturnCode::Error;
			}
		} else {
			logs::err << "Usage: " << input[0] << " filetype [-d] filename\n";
			return Console::ReturnCode::Error;
		}
	} else {
		if (!MCScatter::read_grid(input[1], input[2], false)) {
			return Console::ReturnCode::Error;
		}
	}
	
	return Console::ReturnCode::Good;
}

Console::ReturnCode write(std::vector<std::string> input) {	
	if (input.size() != 3 && input.size() != 4) {
		logs::err << "Usage: " << input[0] << " filetype [-d] filename\n";
		return Console::ReturnCode::Error;
	}
	
	if (input.size() == 4) {
		if (input[2] == "-d") {
			if (!MCScatter::write_grid(input[1], input[3], true)) {
				return Console::ReturnCode::Error;
			}
		} else {
			logs::err << "Usage: " << input[0] << " filetype [-d] filename\n";
			return Console::ReturnCode::Error;
		}
	} else {
		if (!MCScatter::write_grid(input[1], input[2], false)) {
			return Console::ReturnCode::Error;
		}
	}
	
	return Console::ReturnCode::Good;
}

Console::ReturnCode config(std::vector<std::string> input) {
	if (input.size() != 2) {
		logs::err << "Usage: " << input[0] << " configfile\n";
		return Console::ReturnCode::Error;
	}
	
	if (!MCScatter::read_config(input[1])) {
		return Console::ReturnCode::Error;
	}
	
	return Console::ReturnCode::Good;
}



