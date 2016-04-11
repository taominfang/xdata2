/*
 * RegAwk.h
 *
 *  Created on: Apr 8, 2016
 *      Author: mtao60
 */

#ifndef REGAWK_H_
#define REGAWK_H_
#include <vector>
#include <boost/regex.hpp>
#include "CommandLineParameterParser.h"
using namespace openfly;
class RegAwk {
public:
	RegAwk();
	virtual ~RegAwk();

	int run(int argc, char ** argv);


	int regexSplit(boost::regex & expression,string & input, vector<string> &result);

protected:
	boost::regex regex;
	list<string> input_files;
};

#endif /* REGAWK_H_ */
