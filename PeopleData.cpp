//PeopleDate.cpp
//Class PeopleData stores people's name, mobile and address information

#include "PeopleData.h"

using namespace std;

//Class PeopleData constructor
PeopleData::PeopleData()
{
	name[0] = '\0';
	mobile[0] = '\0';
	address[0] = '\0';
	key = 0;
};

//Class PeopleData constructor
PeopleData::PeopleData( string Name, string Mobile, string Address )
{
	setName( Name );
	setMobile( Mobile );
	setAddress( Address );

	key = 0;
	int length = strlen( mobile );
	for( int i = 0; i<4; i++)
		key = key*10 + (mobile[length-4+i]-'0');
}

//Class PeopleData constructor
PeopleData::PeopleData( int Key )
{
	string Name = "";
	string Mobile = "";
	string Address = "";
	setName( Name );
	setMobile( Mobile );
	setAddress( Address );
	key = Key;
}

//set name value
void PeopleData::setName( string &Name )
{
	const char *NameValue = Name.data();
	int length = Name.size();
	length = length < 16? length : 15;
	strncpy( name, NameValue, length );
	name[length] = '\0';
}

//delete name value
void PeopleData::deleteName()
{
	memset( name, 0, sizeof(name) );
	name[0] = '\0';
}

//set mobile value
void PeopleData::setMobile( string &Mobile )
{
	const char *MobileValue = Mobile.data();
	int length = Mobile.size();
	length = length < 16? length : 15;
	strncpy( mobile, MobileValue, length );
	mobile[length] = '\0';
}

//delete mobile value
void PeopleData::deleteMobile()
{
	memset( mobile, 0, sizeof(mobile) );
	mobile[0] = '\0';
}

//set address value
void PeopleData::setAddress( string &Address )
{
	const char *AddressValue = Address.data();
	int length = Address.size();
	length = length < 31? length : 20;
	strncpy( address, AddressValue, length );
	address[length] = '\0';
}

//delete address value
void PeopleData::deleteAddress()
{
	memset( address, 0, sizeof(address) );
	address[0] = '\0';
}

//display the class data
void PeopleData::showData( const SOCKET &client )
{
	char temp[100];
	sprintf( temp, "name:    %s\n", name );
	sendMsg( client, temp);
	sprintf( temp, "mobile:  %s\n", mobile );
	sendMsg( client, temp);
	sprintf( temp, "address: %s\n", address );
	sendMsg( client, temp);
}
