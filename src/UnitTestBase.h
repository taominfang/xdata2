/*
 * UnitTestBase.h
 *
 *  Created on: 2016Äê4ÔÂ9ÈÕ
 *      Author: minfang
 */

#ifndef UNITTESTBASE_H_
#define UNITTESTBASE_H_
#include <string>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class UnitTestBase {
public:
	UnitTestBase();
	virtual ~UnitTestBase();


	virtual bool test()=0;

	string & getName(){
		return name;
	}

	void setName(string  n){
		name=n;
	}

	void printErrors(){
		for(list<string>::iterator i1=error_messages.begin();i1!=error_messages.end();i1++){
			cerr<<*i1<<endl;
		}
	}

	bool areEqual (int v1,int v2,const char * m){
		if(v1==v2){
			return true;
		}
		else{
			string message;
			char t[120];
			if(m!=NULL){
				message=m;
			}
			sprintf(t,"%d is not equal to %d",v1,v2);
			message += t;
			error_messages.push_back(message);
			return false;
		}
	}
	bool areEqual (string v1,string v2,const char * m){
			if(v1==v2){
				return true;
			}
			else{
				string message;
				char t[120];
				if(m!=NULL){
					message=m;
				}
				sprintf(t,"[%s] is not equal to [%s]",v1.c_str(),v2.c_str());
				message += t;
				error_messages.push_back(message);
				return false;
			}
		}

protected:
	string name;
	list<string> error_messages;
};

#endif /* UNITTESTBASE_H_ */
