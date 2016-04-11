/*
 * RegAwk.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: mtao60
 */

#include "RegAwk.h"

RegAwk::RegAwk() {
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
			CommandLineParameter("regex-spliter").setOrder(10).addAliase("-F").setDescription(
					"regular express, using to split a line into columns").setFollowParametersSize(
					1).setRequired(false).addValue("[ \t]"));



	if (command_line_parameters.parsing() > 0) {
		cout << "There is error in parameters:" << endl;
		for (list<string>::const_iterator i1 =
				command_line_parameters.getErrorMessages().begin();
				i1 != command_line_parameters.getErrorMessages().end(); i1++) {
			cout << *i1 << endl;
		}

		command_line_parameters.printUsage(cout);
		return 1;
	}
	const CommandLineParameter * r1 = command_line_parameters.getParameter(
			"input file");
	try {
		regex=boost::regex(r1->getFirstValue().c_str());
	} catch (boost::regex_error e) {
		cerr << e.what() << endl;
		return 1;
	}

	input_files=command_line_parameters.getExtraParameters();


	if(input_files.size()==0){
		//read from stdin
		vector<string> columns;
		while(!cin.eof()){
			string line;
			columns.clear();
			std::getline(cin,line);
			int cs=this->regexSplit(regex,line,columns);
		}
	}
	else{
		//read from files
	}

	return 0;
}

int RegAwk::regexSplit(boost::regex& expression, string& input,
		vector<string>& result) {
	boost::regex_iterator<string::iterator> m1(input.begin(), input.end(),
			expression);
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
