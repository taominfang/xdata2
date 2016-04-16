/*
 * VariablePatternParser.h
 *
 *  Created on: 2016��4��11��
 *      Author: minfang
 */

#ifndef VARIABLEPATTERNPARSER_H_
#define VARIABLEPATTERNPARSER_H_
#include <iostream>

#include <string>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

namespace openfly {

enum {
	TYPE_STRING, TYPE_VARIABLE
};

class StringValue {

public:
	StringValue(const char * v) :
			value(v), type(TYPE_STRING) {

	}
	virtual ~StringValue() {

	}
	virtual string & getString(vector<string> * regexResult, map<string, string>* variableMap,bool keepUnfoundVariable) {

		return value;
	}
	int getType() {
		return type;
	}
	string & getValue() {
		return value;
	}
protected:
	string value;
	int type;

};

class VariableValue: public StringValue {

public:
	VariableValue(const char * v) :
			StringValue((string("${") + string(v) + string("}")).c_str()), regexIndex(-1) {
		type = TYPE_VARIABLE;
		key = v;
		std::istringstream iss(key);
		if ((iss >> regexIndex).fail()) {
			regexIndex = -1;
		}

	}
	virtual ~VariableValue() {

	}
	virtual string & getString(vector<string> * regexResult, map<string, string>* variableMap,bool keepUnfoundVariable) {

		if (regexIndex > -1 && regexResult != NULL && regexResult->size() > (size_t) regexIndex) {
			return (*regexResult)[(size_t) regexIndex];
		} else if (variableMap != NULL) {
			map<string, string>::iterator f = variableMap->find(key);
			if (f == variableMap->end()) {
				return value;
			} else {
				return f->second;
			}
		} else {
			if(keepUnfoundVariable)
			return value;
			else{
				return empty;
			}
		}
	}

protected:
	int regexIndex;
	string key;
	string empty;

};

class VariableProcessor {
public:
	VariableProcessor() {
	}
	~VariableProcessor() {
		for (vector<StringValue *>::iterator i1 = handle.begin(); i1 != handle.end(); i1++) {
			delete *i1;
		}
		handle.clear();
	}

	void add(int type, const char * key) {
		if (type == TYPE_STRING) {
			handle.push_back(new StringValue(key));
		} else if (type == TYPE_VARIABLE) {
			handle.push_back(new VariableValue(key));
		}
	}

	void contruct(vector<string> * regexResult, map<string, string>* variableMap,bool isKeepUnfoundVariable, std::ostream & out) {
		for (vector<StringValue *>::iterator i1 = handle.begin(); i1 != handle.end(); i1++) {
			out << (*i1)->getString(regexResult, variableMap,isKeepUnfoundVariable);
		}
	}
	int size() {
		return handle.size();
	}

	void debugPrint() {
		for (int i1 = 0; i1 < handle.size(); i1++) {
			cout << handle[i1]->getType() << ":" << handle[i1]->getValue() << endl;
		}
	}

protected:
	vector<StringValue *> handle;
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

	void static printStatus(std::ostream & out) {
		out << " NORMAL:" << NORMAL << ", ESCAPE:" << ESCAPE << ", VARIABLE:" << VARIABLE;
	}

	int static parse(const char * input, VariableProcessor & result, std::ostream & error) {

		char buff[1024 * 1000];
		int bIndex = 0;

		int status = NORMAL;
		int preStatus = NORMAL;
		while (true) {
			char c = *input;
			if (c == 0) {
				break;
			}
			input++;

			switch (status) {
			case NORMAL:
				if (c == '\\') {
					preStatus = status;
					status = ESCAPE;
				} else if (c == '$' && *input == '{') {
					input++;
					status = VARIABLE;
					buff[bIndex] = 0;
					if (bIndex > 0) {
						result.add(TYPE_STRING, buff);

					}
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
				if (c == '}') {

					status = NORMAL;
					buff[bIndex] = 0;

					result.add(TYPE_VARIABLE, buff);
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
			error << "last status should be NORMAL, but it is:" << status;
			printStatus(error);
		} else if (bIndex != 0) {
			result.add(TYPE_STRING, buff);

		}
		return result.size();
	}

};
}

#endif /* VARIABLEPATTERNPARSER_H_ */
