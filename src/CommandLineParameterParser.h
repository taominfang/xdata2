/*
 * CommandLineParameterParser.h
 *
 *  Created on: Dec 21, 2015
 *      Author: mtao60
 */
#ifndef COMMANDLINEPARAMETERPARSER_H_
#define COMMANDLINEPARAMETERPARSER_H_
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

namespace openfly {

class CommandLineParameter {

public:
	CommandLineParameter() :
			order(0), followParametersSize(0), required(false), value_level(0), is_set(false), is_available(false) {

	}
	CommandLineParameter(string keyWord) :
			keyword(keyWord), order(0), followParametersSize(0), required(false), value_level(0), is_set(false), is_available(false) {
		aliases.push_back(keyword2FirstAliase());
	}
	CommandLineParameter(string keyWord, string firstAliase) :
			keyword(keyWord), order(0), followParametersSize(0), required(false), value_level(0), is_set(false), is_available(false) {
		aliases.push_back(firstAliase);
	}

	virtual ~CommandLineParameter() {

	}

	string keyword2FirstAliase() {
		const char * s = keyword.c_str();
		string re = "--";
		for (int i1 = 0, len = strlen(s); i1 < len; i1++) {
			char c = s[i1];
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') {
				re += c;
			} else {
				re += "-";
			}
		}
		return re;
	}

	void printUsage(ostream & out) {
		out << aliases[0] << " ";
		for (int i1 = 0; i1 <= this->followParametersSize; i1++) {
			out << "value_" << i1 << " ";
		}
		out << endl;
		if (this->aliases.size() > 1) {
			out << "\tAlias:";
			for (size_t i1 = 1; i1 < aliases.size(); i1++) {
				if (i1 > 1) {
					out << ", ";
				}
				out << "   " << aliases[i1];
			}
			out << endl;
		}
		out << "\tIs required:" << (this->required ? "True" : "False") << endl;
		if (description.length() > 0) {
			out << "\tDescription:" << description << endl;
		}

		out << endl;

	}

	int putValues(char ** ps, int start) {
		if (this->followParametersSize > 0) {
			this->addValueLevel();
			for (int i1 = 0; i1 < this->followParametersSize; i1++) {
				this->addValue(ps[start + i1]);
			}

		}
		is_available = true;
		is_set = true;
		return this->followParametersSize;
	}
public:
	static bool compare(CommandLineParameter * f, CommandLineParameter *s) {
		return f->getOrder() < s->getOrder();
	}

public:
	string getKeyword() {
		return keyword;
	}

	int getOrder() {
		return order;
	}

	CommandLineParameter & setOrder(int o) {
		order = o;
		return *this;
	}

	int getFollowParametersSize() const {
		return followParametersSize;
	}

	CommandLineParameter & setFollowParametersSize(int followParametersSize) {
		this->followParametersSize = followParametersSize;
		return *this;
	}

	const vector<string>& getAliases() const {
		return aliases;
	}

	CommandLineParameter & addAliase(string alias) {
		this->aliases.push_back(alias);
		return *this;
	}

	const list<string>& getDependences() const {
		return dependences;
	}

	CommandLineParameter & addDependences(string d) {
		this->dependences.push_back(d);
		return *this;
	}

	bool isRequired() const {
		return required;
	}

	CommandLineParameter & setRequired(bool required) {
		this->required = required;
		return *this;
	}

	bool getValue(string &returnValue, size_t level = 0, size_t index = 0) const {
		if (values.size() > level && values[level].size() > index) {
			returnValue = values[level][index];
			return true;
		} else {
			return false;
		}
	}

	bool isSet() const {
		return is_set;
	}
	bool isAvailable() const {
		return is_available;
	}
	int getValueLevelSize() const {
		return values.size();
	}

	CommandLineParameter & addValueLevel() {

		values.push_back(vector<string>());
		value_level = values.size() - 1;
		return *this;
	}
	CommandLineParameter & addValue(string v) {

		is_available = true;
		if (values.size() == 0) {
			addValueLevel();
		}

		values[values.size() - 1].push_back(v);

		return *this;
	}

	const string& getDescription() const {
		return description;
	}

	CommandLineParameter & setDescription(const string& description) {
		this->description = description;
		return *this;
	}

	const string& getFirstValue() const {
		if (values.size() == 0 || values[0].size() == 0)
			return temp;
		return values[0][0];
	}

protected:
	string keyword;
	//the position in usage
	int order;
	int followParametersSize;
	bool required;
	vector<string> aliases;
	list<string> dependences;
	vector<vector<string> > values;
	int value_level;
	string description;
	bool is_set;
	bool is_available;
	string temp;
};

class CommandLineParameterParser {
public:
	CommandLineParameterParser(int pSize, char ** vs) :
			parameter_size(pSize), parameters(vs), order(1) {
	}
	virtual ~CommandLineParameterParser() {
	}

	void add(CommandLineParameter p) {
		// if there is not order in parameter, it should be the last one in the array
		order++;
		int t1 = p.getOrder();
		if (t1 == 0) {
			p.setOrder(order);
		} else {
			if (t1 > order) {
				order = t1;
			}
		}
		keywordMap[p.getKeyword()] = p;
		CommandLineParameter & t2 = keywordMap[p.getKeyword()];
		aliaseKeyMap["--" + p.getKeyword()] = &t2;
		for (vector<string>::const_iterator i1 = t2.getAliases().begin(); i1 != t2.getAliases().end(); i1++) {
			aliaseKeyMap[*i1] = &t2;
		}

	}

	/**
	 * Print usage
	 */

	void sortParameters(list<CommandLineParameter *>& result) {
		for (map<string, CommandLineParameter>::iterator i1 = keywordMap.begin(); i1 != keywordMap.end(); i1++) {
			result.push_back(&(i1->second));
		}
		result.sort(CommandLineParameter::compare);
	}

	void printUsage(ostream & out) {
		list<CommandLineParameter *> preOrders;
		sortParameters(preOrders);
		out << "Usage:" << endl;
		for (list<CommandLineParameter*>::iterator i1 = preOrders.begin(); i1 != preOrders.end(); i1++) {
			(*i1)->printUsage(out);
		}
	}
	int parsing() {
		error_messages.clear();


		extra_parameters.clear();

		for (int i1 = 0; i1 < parameter_size;) {
			if (aliaseKeyMap.find(parameters[i1]) != aliaseKeyMap.end()) {
				CommandLineParameter * p = aliaseKeyMap[parameters[i1]];
				i1++;
				int restSize = parameter_size - i1;
				if (p->getFollowParametersSize() > restSize) {
					ostringstream tt;
					tt << p->getKeyword() << " need " << p->getFollowParametersSize() << " but there are only " << restSize;
					error_messages.push_back(tt.str());
				} else {
					i1 += p->putValues(parameters, i1);
				}
			} else {
				extra_parameters.push_back(parameters[i1++]);
			}
		}

		checkError();
		return error_messages.size();
	}

	const list<string>& getErrorMessages() const {
		return error_messages;
	}

	const list<string>& getExtraParameters() const {
		return extra_parameters;
	}

	const CommandLineParameter * getParameter(string keyword) const {
		map<string, CommandLineParameter>::const_iterator r = keywordMap.find(keyword);
		if (r == keywordMap.end()) {
			string errorMessage = "The parameter :" + keyword + " is not valid, please check the typo!";
			cerr << errorMessage;
			throw errorMessage;
		}
		return &(r->second);
	}
protected:
	void checkError() {
		list<CommandLineParameter *> preOrders;
		sortParameters(preOrders);

		for (list<CommandLineParameter *>::iterator i1 = preOrders.begin(); i1 != preOrders.end(); i1++) {
			CommandLineParameter * p=*i1;
			if(p->isRequired() && (!p->isAvailable())){
				error_messages.push_back(p->getKeyword()+" is required, but not available");
			}
			if(p->getDependences().size()>0){
				for(list<string>::const_iterator i2=p->getDependences().begin();i2!=p->getDependences().end();i2++){
					if(keywordMap.find(*i2)==keywordMap.end()){
						throw p->getKeyword()+"'s dependent: "+*i2+" may be a typo, which is not existent!";
					}
					if(!keywordMap[*i2].isAvailable()){
						error_messages.push_back(p->getKeyword()+"'s dependent: ["+*i2+"] is not available");
					}
				}
			}
		}
	}

protected:
	int parameter_size;
	char ** parameters;
	int order;
	map<string, CommandLineParameter> keywordMap;
	map<string, CommandLineParameter *> aliaseKeyMap;
	list<string> extra_parameters;
	list<string> error_messages;
};

} /* namespace openfly */

#endif /* COMMANDLINEPARAMETERPARSER_H_ */
