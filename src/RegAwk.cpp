/*
 * RegAwk.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: mtao60
 */

#include "RegAwk.h"

RegAwk::RegAwk():keep_variable(true) {
	// TODO Auto-generated constructor stub

}

RegAwk::~RegAwk() {

}

int __main(int argc, char ** argv) {
	RegAwk regawk;
	return regawk.run(argc, argv);
}

int RegAwk::run(int argc, char** argv) {
	CommandLineParameterParser command_line_parameters(argc, argv);

	command_line_parameters.add(
			CommandLineParameter("regex-spliter").setOrder(10).addAliase("-F").setDescription("regular express, using to split a line into columns").setFollowParametersSize(
					1).setRequired(false).addValue("[ \t]"));

	command_line_parameters.add(
			CommandLineParameter("output-pattern").setOrder(10).addAliase("-P").setDescription("How to output the split column").setFollowParametersSize(1).setRequired(
					false).addValue("${0}"));

	command_line_parameters.add(
				CommandLineParameter("keep-unfound-varaibles").setOrder(10).addAliase("-K").setDescription("if the column is not exist, keep it, for example, there are 4 column, but in the output you give ${5}, with this parameter set, the output string will keep ${5}").setFollowParametersSize(0).setRequired(
						false));
	if (command_line_parameters.parsing() > 0) {
		cout << "There is error in parameters:" << endl;
		for (list<string>::const_iterator i1 = command_line_parameters.getErrorMessages().begin(); i1 != command_line_parameters.getErrorMessages().end();
				i1++) {
			cout << *i1 << endl;
		}

		command_line_parameters.printUsage(cout);
		return 1;
	}

	cout<<command_line_parameters.getParameter("regex-spliter")->getFirstValue()<<endl;

	keep_variable=command_line_parameters.getParameter("keep-unfound-varaibles")->isSet();

	try {
		regex = boost::regex(command_line_parameters.getParameter("regex-spliter")->getFirstValue().c_str());
	} catch (boost::regex_error e) {
		cerr << e.what() << endl;
		return 1;
	}

	int pre = VariablePatternParser::parse(command_line_parameters.getParameter("output-pattern")->getFirstValue().c_str(), processor, cerr);
	if (pre < 0) {
		return 1;
	}

	input_files = command_line_parameters.getExtraParameters();

	if (input_files.size() == 0) {

		//read from stdin
		handleInput(cin);
	} else {
		cout << "read from file" << endl;
		for (list<string>::iterator i1 = input_files.begin(); i1 != input_files.end(); i1++) {
			ifstream fin(*i1, iostream::in);
			if (fin.is_open()) {
				handleInput(fin);
				fin.close();
			} else {
				cerr << "Can not open file:" << *i1 << endl;
			}
		}
		//read from files
	}

	return 0;
}

int RegAwk::regexSplit(boost::regex& expression, string& input, vector<string>& result) {
	boost::regex_iterator<string::iterator> m1(input.begin(), input.end(), expression);
	boost::regex_iterator<string::iterator> stop;

	string::iterator start = input.begin();
	size_t size = 0;
	size_t total = 0;
	for (size = 0; m1 != stop; m1++, size++) {
		const boost::match_results<string::iterator>& what = *m1;

		if (what.size() > 0) {
			//if the input only contain split string,for example "s".split("s") should be 0 element !
			if ((what[0].first == input.begin() && what[0].second == input.end())) {
				return 0;
			}
			result.push_back(string(start, what[0].first));
			start = what[0].second;
			total++;
		}

	}

	if (size == 0) {
		result.push_back(input);
		total++;

	} else {
		if (start != input.end()) {
			result.push_back(string(start, input.end()));
			total++;

		}
	}
	return total;
}

void RegAwk::handleInput(istream& in) {
	vector<string> columns;
	while (!in.eof()) {
		string line;
		columns.clear();
		std::getline(in, line);
		this->regexSplit(regex, line, columns);

		processor.contruct(&columns, NULL,keep_variable, cout);
		cout << endl;
	}
}
