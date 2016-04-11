/*
 * UnitFunctionsTestCases.h
 *
 *  Created on: 2016Äê4ÔÂ9ÈÕ
 *      Author: minfang
 */

#ifndef UNITFUNCTIONSTESTCASES_H_
#define UNITFUNCTIONSTESTCASES_H_

#include "UnitTestBase.h"
#include "RegAwk.h"

class TestRegexSplit: public UnitTestBase {

public:
	virtual bool test() {

		RegAwk t;
		boost::regex exp("[sc]");
		vector<string> re;
		string test("s");
		areEqual(0, t.regexSplit(exp, test, re), "'s' split by 's':");

		re.clear();
		test = " s";
		areEqual(1, t.regexSplit(exp, test, re), "' s' split by 's':");
		if (re.size() > 0)
			areEqual(" ", re[0], "' s' split by 's', the first element:");

		test = "asca";
		areEqual(3, t.regexSplit(exp, test, re), "' s' split by 's':");
		if (re.size() > 2)
			areEqual("", re[2], "' s' split by 's', the second element:");
		return error_messages.size() == 0;
	}
};

#endif /* UNITFUNCTIONSTESTCASES_H_ */
