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

#include "VariablePatternParser.h"


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

class TestStringVariableParser : public UnitTestBase {

public:
	virtual bool test() {
		const char * s="a${b}${1}${2}${c}\\$c";
		map<string,string> vm;
		vm["b"]="bb";
		vm["2"]="22";
		vector<string> rs;
		rs.push_back("000");
		rs.push_back("1111");

		vector<StringValue> rr;
		std::ostringstream  errorMessage;
		int size=VariablePatternParser::parse(s,rr,errorMessage);
		if(size<=0){
			cerr<<errorMessage.str()<<endl;
			return false;
		}
		std::ostringstream  result;
		VariablePatternParser::contruct(&rs,&vm,rr,result);
		cout<<result.str()<<endl;
		return true;
	}
};
#endif /* UNITFUNCTIONSTESTCASES_H_ */
