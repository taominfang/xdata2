/*
 * RegAwk.h
 *
 *  Created on: Apr 8, 2016
 *      Author: mtao60
 */

#ifndef REGAWK_H_
#define REGAWK_H_
#include "CommandLineParameterParser.h"
using namespace openfly;
class RegAwk {
public:
	RegAwk();
	virtual ~RegAwk();

	int run(int argc, char ** argv);

protected:
	CommandLineParameterParser * command_line_parameters;
};

#endif /* REGAWK_H_ */
