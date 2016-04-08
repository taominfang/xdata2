/*
 * RegAwk.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: mtao60
 */

#include "RegAwk.h"

RegAwk::RegAwk() :
		command_line_parameters(NULL) {
	// TODO Auto-generated constructor stub

}

RegAwk::~RegAwk() {
	// TODO Auto-generated destructor stub
	if (command_line_parameters != NULL) {
		delete command_line_parameters;
	}
}

int __main(int argc, char ** argv) {
	RegAwk regawk;
	return regawk.run(argc, argv);
}

int RegAwk::run(int argc, char** argv) {
	if (command_line_parameters != NULL) {
		delete command_line_parameters;
	}
	command_line_parameters = new CommandLineParameterParser(argc, argv);

	command_line_parameters->add(CommandLineParameter("regex-spliter").setOrder(10).addAliase("-F").setDescription("regular express, using to split a line into columns").setFollowParametersSize(1).setRequired(false).addValue(" "));



	return 0;
}
