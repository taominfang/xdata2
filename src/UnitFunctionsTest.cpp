/*
 * UnitFunctionsTest.cpp
 *
 *  Created on: 2016Äê4ÔÂ9ÈÕ
 *      Author: minfang
 */

#include "UnitFunctionsTest.h"
UnitFunctionsTest::UnitFunctionsTest(int argc, char **argv) {

	initAllCalse();
	required_cases.clear();
	if (argc < 2) {

		cout << "No special test case , test all!" << endl;
		for (map<string, UnitTestBase *>::iterator i1 = test_cases.begin();
				i1 != test_cases.end(); i1++) {
			required_cases.push_back(i1->second);
		}
	}
	for (int i1 = 1; i1 < argc; i1++) {
		string name = argv[i1];
		if (test_cases.find(name) == test_cases.end()) {
			cerr << "Did not find test case for :" << name << endl;
		} else {
			required_cases.push_back(test_cases[name]);
		}
	}

}

UnitFunctionsTest::~UnitFunctionsTest() {
	for (map<string, UnitTestBase *>::iterator i1 = test_cases.begin();
			i1 != test_cases.end(); i1++) {
		delete i1->second;
	}
}

int main(int argc, char **argv) {

	UnitFunctionsTest t(argc, argv);
	t.test();
}

void UnitFunctionsTest::test() {
	if (required_cases.size() == 0) {
		cerr << "No test case assigned !" << endl;
		return;
	}

	for (list<UnitTestBase *>::iterator i1 = required_cases.begin();
			i1 != required_cases.end(); i1++) {
		cout << (*i1)->getName() << "................ ";
		try {
			if ((*i1)->test()) {
				cout << "Pass" << endl;
			} else {
				cout << "Fault" << endl;
				(*i1)->printErrors();
			}
		} catch (...) {
			cout << "Exception" << endl;
		}

	}
}

void UnitFunctionsTest::initAllCalse() {
	test_cases["regexSplitTest"] = new TestRegexSplit();
	test_cases["TestStringVariableParser"] = new TestStringVariableParser();

	for (map<string, UnitTestBase *>::iterator i1 = test_cases.begin();
			i1 != test_cases.end(); i1++) {
		i1->second->setName(i1->first);
	}
}
