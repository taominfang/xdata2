/*
 * RegAwk.h
 *
 *  Created on: Apr 8, 2016
 *      Author: mtao60
 */

#ifndef REGAWK_H_
#define REGAWK_H_
#include <vector>
#include <fstream>

#include <boost/regex.hpp>
#include "CommandLineParameterParser.h"
#include "VariablePatternParser.h"
using namespace openfly;
class RegAwk {
public:
	RegAwk();
	virtual ~RegAwk();

	int run(int argc, char ** argv);


	int regexSplit(boost::regex & expression,string & input, vector<string> &result);


	void handleInput(istream &in);

protected:
	boost::regex regex;
	bool keep_variable;
	list<string> input_files;
	VariableProcessor processor;
};

#endif /* REGAWK_H_ */
