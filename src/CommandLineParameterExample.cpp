//============================================================================
// Name        : CommandLineParameters.cpp
// Author      : Minfang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "CommandLineParameterParser.h"
#include <string.h>
using namespace std;
using namespace openfly;
char testBuff[1024 * 1024];
char * argv[1000];

int parseLineToCharPointArray(const char *s) {
	strcpy(testBuff, s);
	int pi = 0;
	argv[pi++] = testBuff;

	for (int i1 = 0; pi < 1000; i1++) {

		if (testBuff[i1] == ' ' || testBuff[i1] == '\t') {
			for (;; i1++) {
				if (testBuff[i1] == 0) {
					break;
				} else if (testBuff[i1] == ' ' || testBuff[i1] == '\t') {
					testBuff[i1] = 0;

				} else {
					argv[pi++] = &testBuff[i1];
					break;
				}

			}
		}
		if (testBuff[i1] == 0) {
			break;
		}
	}

	return pi;

}

void example1() {
	cout << "Example 1, basic usage" << endl;
	const char * tests = "-i my_input_file.txt -O out_file1 out_file2";
	int argc = parseLineToCharPointArray(tests);
	CommandLineParameterParser parser(argc, argv);
	parser.add(CommandLineParameter("input file").setOrder(10).addAliase("-i").setDescription("input file").setFollowParametersSize(1));
	parser.add(CommandLineParameter("output files", "--out-files").setOrder(11).addAliase("-o").addAliase("-O").setFollowParametersSize(2));

	parser.parsing();

	const CommandLineParameter * pi = parser.getParameter("input file");
	cout << "input file is set:" << (pi->isSet() ? "true" : "false") << endl;
	if (pi->isAvailable()) {
		cout << "parameter value:" << pi->getFirstValue() << endl;
		string v;
		if (pi->getValue(v, 0, 0)) {
			cout << "get value from different way:" << v << endl;
		} else {
			cout << "Some wrong, it should work!" << endl;
		}
	}

	const CommandLineParameter * po = parser.getParameter("output files");
	cout << "output files are set:" << (po->isSet() ? "true" : "false") << endl;
	if (po->isAvailable()) {
		string v;
		if (po->getValue(v, 0, 0)) {
			cout << "get first value :" << v << endl;
		} else {
			cout << "Some wrong, it should work!" << endl;
		}

		if (po->getValue(v, 0, 1)) {
			cout << "get second value :" << v << endl;
		} else {
			cout << "Some wrong, it should work!" << endl;
		}
	} else {
		cout << "No values available !" << endl;
	}

	cout << "End Example 1" << endl << endl;
}

void example2() {
	cout << "Example 2 check error" << endl;

	const char * tests = "-O out_file1 out_file2";
	int argc = parseLineToCharPointArray(tests);
	CommandLineParameterParser parser(argc, argv);
	parser.add(CommandLineParameter("input file").setOrder(10).addAliase("-i").setDescription("input file").setFollowParametersSize(1).setRequired(true));
	parser.add(
			CommandLineParameter("output files", "--out-files").setOrder(11).addAliase("-o").addAliase("-O").setFollowParametersSize(2).addDependences(
					"input file"));

	if (parser.parsing() > 0) {
		cout << "There is error in parameters:" << endl;
		for (list<string>::const_iterator i1 = parser.getErrorMessages().begin(); i1 != parser.getErrorMessages().end(); i1++) {
			cout << *i1 << endl;
		}

		parser.printUsage(cout);
	}

	else{
		cout<<"There is not error"<<endl;
	}
	cout << "End Example 2" << endl << endl;

}


void example3() {
	cout << "Example3 set default value and multiple values" << endl;

	const char * tests = "-O out_file1-1 out_file1-2 -O outfile2-1 outfile2-2";
	int argc = parseLineToCharPointArray(tests);
	CommandLineParameterParser parser(argc, argv);
	parser.add(CommandLineParameter("input file").setOrder(10).addAliase("-i").setDescription("input file").setFollowParametersSize(1).setRequired(true).addValue("default_input_file"));
	parser.add(
			CommandLineParameter("output files", "--out-files").setOrder(11).addAliase("-o").addAliase("-O").setFollowParametersSize(2).addDependences(
					"input file"));

	if (parser.parsing() > 0) {
		cout << "There is error in parameters:" << endl;
		for (list<string>::const_iterator i1 = parser.getErrorMessages().begin(); i1 != parser.getErrorMessages().end(); i1++) {
			cout << *i1 << endl;
		}

		parser.printUsage(cout);
	}

	else{
		cout<<"There is not error"<<endl;
		cout<<"How may set of values:"<<(parser.getParameter("output files")->getValueLevelSize())<<endl;
		for(int i1=0;i1<parser.getParameter("output files")->getValueLevelSize();i1++){
			for(int i2=0;i2<parser.getParameter("output files")->getFollowParametersSize();i2++){
				string v;
				parser.getParameter("output files")->getValue(v,i1,i2);
				cout<<"Set "<<(i1+1)<<", value :"<<(i2+1)<<":"<<v<<endl;
			}
		}

		cout<<endl;
		cout<<"The value for [input file] is "<<parser.getParameter("input file")->getFirstValue()<<", which is default value, because the set flag is "<<(parser.getParameter("input file")->isSet()?"true":"false")<<endl;
	}
	cout << "End Example 3" << endl << endl;

}




int main(int argc, char **argv) {

	example1();

	example2();

	example3();
	return 0;
}
