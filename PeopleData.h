//PeopleData.h
//Class PeopleData definition 

#ifndef PEOPLEDATA_H
#define PEOPLEDATA_H

#include <string>
#include <memory.h>
#include <iostream>
#include <iomanip>
#include "socketInOut.h"

using namespace std;

class PeopleData
{
public:
	//PeopleData constructor
	PeopleData();
	PeopleData( string Name, string Mobile, string Address );
	PeopleData( int Key );

	//functions for accountName
	string getName(){ return name; };
	void setName( string &Name ) ;
	void deleteName();

	//functions for accountMobile
	string getMobile() { return mobile; };
	void setMobile( string &Mobile );
	void deleteMobile();

	//functions for accountAddress
	string getAddress() { return address; };
	void setAddress( string &Address );
	void deleteAddress();

	//function for key
	int getKey(){ return key; };

	//display the class data
	void showData( const SOCKET &client );

private:
	char name[16];
	char mobile[16];
	char address[31];

	int  key;
};

#endif