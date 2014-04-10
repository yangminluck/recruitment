//main.cpp
//--Address book management application using linked list.
//--Defined search, add and delete operation in the address book.
//--A file with the same name as the address book persists the data in the file system.
//--Sub thread operates as an address book sever with ip 127.0.0.2, port 4000.
//--Client on the main thread can access the address book sever and edit the data in the book.
//--Please use "exitmain" when you are required to input the ip to exit the program. 

//!!Sorry, for the time limits, the formats of the ip address and port are not checked in this code.
//!!Please be careful when you input the ip and port number.

#include <iostream>
#include <string>
#include "socket.h"

using namespace std;

bool bSeverBuilt = false;

//thread for sever socket
void *severThread( void *arg )
{
	//ip and port for binding
	string ip = "127.0.0.2";
	int port = 4000;

	SeverSocket s;
	if( s.build( ip, 4000 ) )
	{
		bSeverBuilt = true;
		
		//listening
		s.waitClient();
	}
	else
	{
		bSeverBuilt = false;
		return 0;
	}

	return 0;
}

int main()
{

        //create a thread for the sever socket
	//SeverThread = CreateThread(NULL, 0, severThread, NULL, 0, NULL );
	pthread_t ptrThreadID;
	int error;
	error = pthread_create( &ptrThreadID, NULL, severThread, NULL );
	if( error != 0 )
	{
		cout<<"Create Thread Failed!"<<endl;
		return -1;
	}

	//wait for the sever socket be created
	while( !bSeverBuilt ){}
	
	while( bSeverBuilt )
	{
		string linkIP;
		int linkPort;

		cout<<endl;
		cout<<"*** !!Caution: exitmain  --quit the program. ***"<<endl;
		cout<<"Input the IP of sever: ";
		cin>>linkIP;

		//exit the main thread
		if( linkIP == "exitmain" )
		{
			//CloseHandle(SeverThread);
			pthread_cancel( ptrThreadID );
			return 0;
		}

		cout<<"Input the Port: ";
		cin>>linkPort;

		//Client socket
		ClientSocket c;
		if( c.link( linkIP, linkPort ) )
		{
			bool bDis = true;
			bool bAddBook = true;
			while( bAddBook )
			{
				if( bDis )
				{
					//receive message from sever and print
					string recvStr;
					c.recvMsg( recvStr );

					//"$" indicates the input in the next pass
					if( recvStr == "$" )
					{
						bDis = false;
						continue;
					}
					else
						cout<<recvStr;
				}
				else
				{
					string sendStr;
					getline(cin, sendStr);

					if( sendStr == "" )
						continue;

					if( sendStr == "!quit" )
						bAddBook = false;

					c.sendMsg( sendStr );
					bDis = true;
				}
			}
		}

		//destructor of c
	}

	pthread_cancel( ptrThreadID );

	return 0;
}
