//TelBook.cpp
//Class TelBook defines the operation for address book

#include "TelBook.h"

using namespace std;

//Class TelBook constructor
TelBook::TelBook( string &TelBookNameString, const SOCKET &client )
{
	//set the name of the TelBook and its file
	setTelBookName( TelBookNameString );
	strcpy( FileName, TelBookName);
	strcat( FileName, ".dat" );

	//read entries from the file
	//if the file not exists, create one 
	readFile( FileName, client );

	//the output file stream for the TelBook
	outFile.open( FileName, ios::in|ios::out|ios::binary );

	char temp[200];
	sprintf( temp, "    **** This is address book: %s ****\n", TelBookName );
	sendMsg( client, temp );
}

//read entries from file squentially
void TelBook::readFile( char FileName[], const SOCKET &client )
{
	int i = 0;

	PeopleData people;
	ifstream inputFile( FileName, ios::in );

	//create the file if not exists
	if( inputFile.fail() )			
	{
		char temp[200];
		sprintf( temp, "\nCreating the file %s. \n", FileName );
		sendMsg( client, temp );
		ofstream outputFile( FileName, ios::out);
		
		//a blank person is defined by -1 as its Key
		PeopleData blankPerson(-1);
		outputFile.seekp(0);
		
		//move the pointer and initialize the records in the file
		for(int i = 0; i<10000; i++)
			outputFile.write( reinterpret_cast<const char*>(&blankPerson), sizeof(PeopleData) );
		
		outputFile.close();
		return;
	}

	char temp[200];
	sprintf( temp, "\nReading from file %s...\n", FileName );
	sendMsg( client, temp );
	
	//read first record in the file
	inputFile.seekg(0);
	inputFile.read( reinterpret_cast< char* >( &people ), sizeof(PeopleData) );

	//read all records in the fils
	while( inputFile && !inputFile.eof() )
	{
		//push the record into list
		if( people.getKey() != -1 )
		{
			i++;
			records.push_back( people );
		}

		//read the next record
		inputFile.read( reinterpret_cast< char* >( &people ), sizeof(PeopleData) );
	}

	inputFile.close();

	sendMsg( client, "Reading file finished.\n" );	
	sprintf( temp, "%d entries in the address book.\n\n", i );
	sendMsg( client, temp );	
}

//write entry into file
void TelBook::writeFile( PeopleData &person  )
{
	if( !outFile )
	{
		cerr<<"File cannot be opened!!"<<endl;
		exit(1);
	}

	//move the pointer to the correct position and write
	outFile.seekp( person.getKey() * sizeof(PeopleData) );
	outFile.write( reinterpret_cast<const char*>(&person), sizeof(PeopleData) );
}

//delete entry in the file
void TelBook::deleteFile( int Key )
{
	if( !outFile )
	{
		cerr<<"File cannot be opened!!"<<endl;
		exit(1);
	}

	PeopleData blankPerson(-1);

	//move the pointer to the correct position and write a blank record 
	outFile.seekp( Key * sizeof(PeopleData) );
	outFile.write( reinterpret_cast<const char*>(&blankPerson), sizeof(PeopleData) );
}

//set the TelBookName value
void TelBook::setTelBookName( string &TelBookNameString )
{
	memset( TelBookName, 0, sizeof(TelBookName) );

	const char *NameValue = TelBookNameString.data();
	int length = TelBookNameString.size();
	length = length < 21? length : 20;

	strncpy( TelBookName, NameValue, length );
	TelBookName[length] = '\0';
}

//add one entry into the address book
void TelBook::addRecord( PeopleData &person, const SOCKET &client )
{
	//the entries are sorted by its key
	list<PeopleData>::iterator iter;
	for( iter = records.begin(); iter != records.end(); iter++ )
	{
		if( (*iter).getKey() > person.getKey() )
		{	
			writeFile( person );
			records.insert( iter, person );
			break;
		}
	}

	//insert at the tail of the list
	if( iter == records.end() )
	{
		writeFile( person );
		records.push_back( person );
	}

	sendMsg( client, "Entry Added!\n" );	
}

//get the name of the file
char* TelBook::getFileName()
{
	return FileName;
}

//check the mobile number
bool TelBook::checkMobile( string &str )
{
	if( str.size() < 8 || str.size() > 15 )				//fixed line number = 8 digits
		return false;									//mobile number = 11 digits
														//international code/area code = 3 digits
	for( int i = 0; i<str.length(); i++ )
	{
		if( (str[i]<'0'||str[i]>'9') && str[i]!=32 )
			return false;
	}

	return true;
}

//search for entries using name value
bool TelBook::searchName( string &name, const SOCKET &client )
{
	list<PeopleData>::iterator iter;
	bool bFlag = false;			//whether the entry is found or not 
	int	 EntryFound = 0;
	int  i = 0;
	int  nameLength = name.length();

	//fuzzy search 
	if( name[nameLength-1] == '*' && name[nameLength-2] == '.' )
	{
		name = name.substr(0, nameLength-2);
		
		//move the iterator along the list to search
		for( iter=records.begin(); iter!=records.end(); iter++ )
		{
			string PeopleName = (*iter).getName().substr(0, nameLength-2);
			if( PeopleName == name )
			{
				i++;

				char temp[256];				
				sprintf( temp, "-No. %d\n", i );
				string ii = temp;
				sendMsg( client, ii );			
				
				(*iter).showData( client );
				EntryFound++;
				bFlag = true;	
			}
		}
	}
	else
	{
		//move the iterator along the list to search
		for( iter=records.begin(); iter!=records.end(); iter++ )
		{
			if( (*iter).getName() == name )
			{
				i++;

				char temp[256];				
				sprintf( temp, "-No. %d\n", i );
				string ii = temp;
				sendMsg( client, ii );		

				(*iter).showData( client );
				EntryFound++;
				bFlag = true;	
			}
		}
	}

	if( !bFlag )
		sendMsg( client, "There is no such name in the address book.\n" ); 
	else
		if( EntryFound == 1 )
		{
			char temp[256];
			sprintf( temp, "%d  entry has been found.\n", EntryFound );
			sendMsg( client, temp );
		}
		else
		{
			char temp[256];
			sprintf( temp, "%d  entries has been found.\n", EntryFound );
			sendMsg( client, temp );
		}

	return bFlag;
}

//search for entries using mobile value
bool TelBook::searchMobile( string &mobile, const SOCKET &client )
{
	list<PeopleData>::iterator iter;
	bool bFlag = false;			//whether the entry is found or not 
	int EntryFound = 0;
	int i = 0;
	int  mobileLength = mobile.length();

	//fuzzy search
	if( mobile[mobileLength-1] == '*' && mobile[mobileLength-2] == '.' )
	{
		mobile = mobile.substr(0, mobileLength-2);

		for( iter=records.begin(); iter!=records.end(); iter++ )
		{
			string PeopleMobile = (*iter).getMobile().substr(0, mobileLength-2);
			if( PeopleMobile == mobile )
			{
				i++;

				char temp[256];				
				sprintf( temp, "-No. %d\n", i );
				string ii = temp;
				sendMsg( client, ii );	

				(*iter).showData( client );
				EntryFound++;
				bFlag = true;	
			}
		}
	}
	else
	{
		//move the iterator along the list to search
		for( iter=records.begin(); iter!=records.end(); iter++ )
		{
			if( (*iter).getMobile() == mobile )
			{
				i++;

				char temp[256];				
				sprintf( temp, "-No. %d\n", i );
				string ii = temp;
				sendMsg( client, ii );			

				(*iter).showData( client );
				EntryFound++;
				bFlag = true;	
			}
		}
	}

	if( !bFlag )
		sendMsg( client, "There is no such mobile in the address book.\n" ); 
	else
		if( EntryFound == 1 )
		{
			char temp[256];
			sprintf( temp, "%d  entry has been found.\n", EntryFound );
			sendMsg( client, temp );
		}
		else
		{
			char temp[256];
			sprintf( temp, "%d  entries has been found.\n", EntryFound );
			sendMsg( client, temp );
		}

	return bFlag;
}

//search for entries using address value
bool TelBook::searchAddress( string &address, const SOCKET &client )
{
	list<PeopleData>::iterator iter;
	bool bFlag = false;			//whether the entry is found or not 
	int EntryFound = 0;
	int i = 0;
	int  addressLength = address.length();

	//fuzzy search
	if( address[addressLength-1] == '*' && address[addressLength-2] == '.' )
	{
		address = address.substr(0, addressLength-2);

		for( iter=records.begin(); iter!=records.end(); iter++ )
		{
			string PeopleAddress = (*iter).getAddress().substr(0, addressLength-2);
			if( PeopleAddress == address )
			{
				i++;

				char temp[256];				
				sprintf( temp, "-No. %d\n", i );
				string ii = temp;
				sendMsg( client, ii );					

				(*iter).showData(client);
				EntryFound++;
				bFlag = true;	
			}
		}
	}
	else
	{
		//move the iterator along the list to search
		for( iter=records.begin(); iter!=records.end(); iter++ )
		{
			if( (*iter).getAddress() == address )
			{
				i++;

				char temp[256];				
				sprintf( temp, "-No. %d\n", i );
				string ii = temp;
				sendMsg( client, ii );					

				(*iter).showData(client);
				EntryFound++;
				bFlag = true;	
			}
		}
	}

	if( !bFlag )
		sendMsg( client, "There is no such address in the address book.\n" );
	else
		if( EntryFound == 1 )
		{
			char temp[256];
			sprintf( temp, "%d  entry has been found.\n", EntryFound );
			sendMsg( client, temp );
		}
		else
		{
			char temp[256];
			sprintf( temp, "%d  entries has been found.\n", EntryFound );
			sendMsg( client, temp );
		}

	return bFlag;
}

//delete entries using name value
bool TelBook::deleteName( string &name, const SOCKET &client )
{
	list<PeopleData>::iterator iter = records.begin();
	bool bFlag = false;			//whether the entry is found or not 
	int EntryFound = 0;
	int  nameLength = name.length();

	//fuzzy search and delete
	if( name[nameLength-1] == '*' && name[nameLength-2] == '.' )
	{
		name = name.substr(0, nameLength-2);
		
		for( ; iter!=records.end();  )
		{
			string PeopleName = (*iter).getName().substr(0, nameLength-2);
			if( PeopleName == name )
			{
				deleteFile( (*iter).getKey() );
				iter = records.erase(iter);
	
				bFlag = true;	
				EntryFound++;
			}
			else
				iter++;
		}
	}
	else
	{
		//move the iterator along the list to search
		for( ; iter!=records.end();  )
		{
			if( (*iter).getName() == name )
			{
				//iterator will move to the next position after erase operation
				deleteFile( (*iter).getKey() );
				iter = records.erase(iter);

				bFlag = true;
				EntryFound++;
			}
			else
				iter++;
		}
	}

	if( !bFlag )
		sendMsg( client, "There is no such name in the address book.\n" ); 
	else
		if( EntryFound == 1 )
		{
			char temp[256];
			sprintf( temp, "%d  entry has been deleted.\n", EntryFound );
			sendMsg( client, temp );
		}
		else
		{
			char temp[256];
			sprintf( temp, "%d  entries has been deleted.\n", EntryFound );
			sendMsg( client, temp );
		}

	return bFlag;
}

//delete entries using mobile value
bool TelBook::deleteMobile( string &mobile, const SOCKET &client )
{
	list<PeopleData>::iterator iter = records.begin();;
	bool bFlag = false;				//whether the entry is found or not 
	int EntryFound = 0;
	int  mobileLength = mobile.length();

	//fuzzy search and delete
	if( mobile[mobileLength-1] == '*' && mobile[mobileLength-2] == '.' )
	{
		mobile = mobile.substr(0, mobileLength-2);
		
		for( ; iter!=records.end();  )
		{
			string PeopleMobile = (*iter).getMobile().substr(0, mobileLength-2);
			if( PeopleMobile == mobile )
			{
				deleteFile( (*iter).getKey() );
				iter = records.erase(iter);
	
				bFlag = true;	
				EntryFound++;
			}
			else
				iter++;
		}
	}
	else
	{
		for( ; iter!=records.end();  )
		{
			if( (*iter).getMobile() == mobile )
			{
				//iterator will move to the next position after erase operation
				deleteFile( (*iter).getKey() );
				iter = records.erase(iter);

				bFlag = true;
				EntryFound++;
			}
			else
				iter++;
		}
	}

	if( !bFlag )
		sendMsg( client, "There is no such mobile in the address book.\n" ); 
	else
		if( EntryFound == 1 )
		{
			char temp[256];
			sprintf( temp, "%d  entry has been deleted.\n", EntryFound );
			sendMsg( client, temp );
		}
		else
		{
			char temp[256];
			sprintf( temp, "%d  entries has been deleted.\n", EntryFound );
			sendMsg( client, temp );
		}

	return bFlag;
}

//delete entries using address value
bool TelBook::deleteAddress( string &address, const SOCKET &client )
{
	list<PeopleData>::iterator iter = records.begin();;
	bool bFlag = false;			//whether the entry is found or not 
	int EntryFound = 0;
	int  addressLength = address.length();

	//fuzzy search and delete
	if( address[addressLength-1] == '*' && address[addressLength-2] == '.' )
	{
		address = address.substr(0, addressLength-2);
		for( ; iter!=records.end();  )
		{
			string PeopleAddress = (*iter).getAddress().substr(0, addressLength-2);
			if( PeopleAddress == address )
			{
				deleteFile( (*iter).getKey() );
				iter = records.erase(iter);
	
				bFlag = true;	
				EntryFound++;
			}
			else
				iter++;
		}
	}
	else
	{
		for( ; iter!=records.end();  )
		{
			if( (*iter).getAddress() == address )
			{
				//iterator will move to the next position after erase operation
				deleteFile( (*iter).getKey() );
				iter = records.erase(iter);

				bFlag = true;
				EntryFound++;
			}
			else
				iter++;
		}
	}

	if( !bFlag )
		sendMsg( client, "There is no such address in the address book.\n" );
	else
		if( EntryFound == 1 )
		{
			char temp[256];
			sprintf( temp, "%d  entry has been deleted.\n", EntryFound );
			sendMsg( client, temp );
		}
		else
		{
			char temp[256];
			sprintf( temp, "%d  entries has been deleted.\n", EntryFound );
			sendMsg( client, temp );
		}

	return bFlag;
}

//deleta all the entries
void TelBook::deletaAll()
{
	list<PeopleData>::iterator iter;
	while( !records.empty() )
	{
		iter = records.begin();
		
		//delete in file
		deleteFile( (*iter).getKey() );

		//delete in list
		records.pop_front();
	}
}

void TelBook::displayAllRecords( const SOCKET &client )
{
	list<PeopleData>::iterator iter;
	int i = 1;

	for( iter = records.begin(); iter!=records.end(); iter++, i++ )
	{	
		char temp[100];
		sprintf( temp, "-No. %d\n", i );		
		sendMsg( client, temp );
	
		(*iter).showData( client );
	}
}
