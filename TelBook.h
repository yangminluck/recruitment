//TelBook.h
//Class TelBook definition 

#ifndef TELBOOK_H
#define TELBOOK_H

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <algorithm>
#include <iomanip>
#include "PeopleData.h"

using namespace std;

class TelBook
{
public:
	//Class TelBook constructor
	TelBook( string &TelBookNameString, const SOCKET &client );
	void setTelBookName( string &NewName );

	//add one entry into address book
	void addRecord( PeopleData &person, const SOCKET &client );

	//get the name of the file
	char* getFileName();

	//close the file stream
	void  closefstream(){ outFile.close(); };
	
	//check the mobile number
	bool checkMobile( string &str );
	
	//functions for name value in records
	bool searchName( string &name, const SOCKET &client );
	bool deleteName( string &name, const SOCKET &client );

	//functions for mobile value in records
	bool searchMobile( string &mobile, const SOCKET &client );
	bool deleteMobile( string &mobile , const SOCKET &client);

	//functions for address value in records
	bool searchAddress( string &address, const SOCKET &client );
	bool deleteAddress( string &address, const SOCKET &client );

	//functions for the operations on list
	void deletaAll();
	bool empty(){ return records.empty(); };
	void pop_back(){ records.pop_back(); };
	int  size(){ return records.size(); };

	//functions for display
	void displayAllRecords( const SOCKET &client );

private:
	char TelBookName[21];
	char FileName[25];
	list<PeopleData> records;
	fstream outFile;

	//functions on file format
	void deleteFile( int Key );
	void writeFile( PeopleData &person );
	void readFile( char fileName[], const SOCKET &client );
};

#endif