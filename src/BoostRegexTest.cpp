#include <stdio.h>
#include <stdlib.h>

#include <fstream>

#include <string.h>
#include "string"
#include "iostream"
#include <boost/regex.hpp>

using namespace std;

bool findIndex(string & data, const boost::match_results<string::iterator>& what, int ind, int & start, int & end) {

	if (ind >= what.size()) {
		return false;
	}

	int f = 0;
	int index = 0;
	for (string::iterator i1 = data.begin(); i1 != data.end(); i1++, index++) {
		if (what[ind].first == i1) {
			start = index;
			f++;
		} else if (what[ind].second == i1) {
			end = index;
			f++;
		}
	}

	if (what[ind].second == data.end()) {
		end = index;
		f++;
	}

	if (f > 1) {
		return true;
	} else {
		return false;
	}
}

void aafind(int a) {

}

int __main(int argc, char **argv) {

	cout << "Regex String:";
	string regex;
	getline(cin, regex);
	cout << endl;
	cout << "Text :";
	string text;
	getline(cin, text);

	boost::regex expression(regex.c_str());

	boost::regex_iterator<string::iterator> m1(text.begin(), text.end(), expression);
	boost::regex_iterator<string::iterator> stop;

	for (int i1 = 1; m1 != stop; m1++, i1++) {

		const boost::match_results<string::iterator>& what = *m1;
		cout << i1 << ", find, size:" << what.size() - 1 << endl;
		cout << "Full string:" << what[0].str() << endl;

		for (int i2 = 1; i2 < what.size(); i2++) {

			int start = -1, end = -1;

			if (findIndex(text, what, i2, start, end)) {
				cout << "\t" << i2 << ":" << what[i2].str() << ", index from: " << start << " until:" << end << endl;
			} else {
				cout << "\t" << i2 << ":" << what[i2].str() << ", but index range of it!" << endl;
			}

			//

		}

	}

	return 0;
}
