/*
 * VariablePatternParser.h
 *
 *  Created on: 2016Äê4ÔÂ11ÈÕ
 *      Author: minfang
 */

#ifndef VARIABLEPATTERNPARSER_H_
#define VARIABLEPATTERNPARSER_H_
#include <iostream>

#include <string>
#include <vector>
#include <map>
using namespace std;
using namespace openfly;
namespace openfly {

class StringValue {

public:
	StringValue(const char * v) :
			value(v) {

	}
	virtual ~StringValue() {

	}
	virtual string & getString(vector<string> * regexResult,
			map<string, string>* variableMap) {
		return value;
	}
protected:
	string value;

};

class VariableValue: public StringValue {

public:
	VariableValue(const char * v) :
			StringValue(v), regexIndex(-1) {
		key = value.substr(2, value.size() - 3);
		std::istringstream iss(key);
		if ((iss >> regexIndex).fail()) {
			regexIndex = -1;
		}

	}
	virtual ~VariableValue() {

	}
	virtual string & getString(vector<string> * regexResult,
			map<string, string>* variableMap) {
		if (regexIndex > -1 && regexResult != NULL
				&& regexResult->size() >(size_t) regexIndex) {
			return (*regexResult)[(size_t)regexIndex];
		} else if (variableMap != NULL) {
			map<string, string>::iterator f = variableMap->find(key);
			if (f == variableMap->end()) {
				return value;
			} else {
				return f->second;
			}
		} else {
			return value;
		}
	}
protected:
	int regexIndex;
	string key;

};

enum {
	NORMAL, ESCAPE, VARIABLE
};
class VariablePatternParser {
public:
	VariablePatternParser() {

	}
	virtual ~VariablePatternParser() {

	}

	void static printStatus(std::ostringstream & out) {
		out << " NORMAL:" << NORMAL << ", ESCAPE:" << ESCAPE << ", VARIABLE:"
				<< VARIABLE;
	}

	void static contruct(vector<string> * regexResult,
			map<string, string>* variableMap,vector<StringValue> & values,std::ostringstream & out){
		for(vector<StringValue>::iterator i1=values.begin();i1!=values.end();i1++){
			out<<i1->getString(regexResult,variableMap);
		}
	}

	int static parse(const char * input, vector<StringValue> & result,
			std::ostringstream & error) {
		result.clear();

		char buff[1024 * 1000];
		int bIndex = 0;
		char c;
		int status = NORMAL;
		int preStatus = NORMAL;
		while ((c = *input++) != 0) {
			switch (status) {
			case NORMAL:
				if (c == '\\') {
					preStatus = status;
					status = ESCAPE;
				} else if (c == '$' && *input == '{') {
					input++;
					status = VARIABLE;
					buff[bIndex] = 0;
					result.push_back(StringValue(buff));
					bIndex = 0;
				} else {
					buff[bIndex++] = c;
				}
				break;

			case ESCAPE:
				buff[bIndex++] = c;
				status = preStatus;
				break;

			case VARIABLE:
				if (c == '}' && *input == '$') {
					input++;
					status = NORMAL;
					buff[bIndex] = 0;
					result.push_back(VariableValue(buff));
					bIndex = 0;
				} else {
					buff[bIndex++] = c;
				}
				break;
			default:
				error << "unknown status:" << status;
				printStatus(error);
				return -1;

			}
		} //end while
		if (NORMAL != status) {
			error << "last status should be NORMAL, but it is:"
					<< status;
			printStatus(error);
		} else if (bIndex != 0) {
			result.push_back(StringValue(buff));

		}
		return result.size();
	}

};
}

#endif /* VARIABLEPATTERNPARSER_H_ */
