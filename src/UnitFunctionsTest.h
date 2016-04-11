/*
 * UnitFunctionsTest.h
 *
 *  Created on: 2016Äê4ÔÂ9ÈÕ
 *      Author: minfang
 */

#ifndef UNITFUNCTIONSTEST_H_
#define UNITFUNCTIONSTEST_H_

#include "UnitFunctionsTestCases.h"

#include <map>
#include <list>
class UnitFunctionsTest {
public:
	UnitFunctionsTest(int argc,char **argv);
	virtual ~UnitFunctionsTest();
	void test();
	void initAllCalse();

private:
	map<string,UnitTestBase *> test_cases;
	list<UnitTestBase *> required_cases;
};

#endif /* UNITFUNCTIONSTEST_H_ */
