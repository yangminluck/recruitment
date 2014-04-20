//cmd.cpp
//functions in the address book sever

#include "cmd.h"

const CMD_FUN_PAIR cmd_pairs[] =		//command in command line and its operation
{
	{ "add",     addEntry },
	{ "search",  findEntry },
	{ "remove",  deleteEntry },
	{ "!help",   showHelp },
	{ "display", displayEntry },
	{ "delete",  deleteAllEntry },
	{ "END", NULL }
};

int main_cmd( const SOCKET &client )
{
	string aa = "Default Book";
	TelBook book1(aa, client);				//declare an object of TelBook
	showHelp( book1, client );
	
	string cmd;
	while(true)
	{
		//input the command
		sendMsg( client, "Please input the command.\n>");  
		sendMsg( client, "$" );							
		recvMsg( client, cmd );									

		if( cmd == "!quit" )
		{
			break;
		}

		//the command and its corresponding operation
		int i = 0;
		while( cmd_pairs[i].StrCmd != "END" )
		{
			string strCmd_Pairs = cmd_pairs[i].StrCmd;
			if( strCmd_Pairs == cmd )
				break;
			else
				i++;
		}

		if( cmd_pairs[i].StrCmd == "END" )
		{
			sendMsg( client, "!!Invalid Input Command\n");		
			showHelp( book1, client );
		}
		else
			cmd_pairs[i].CmdFun( book1, client );
	}

	//close the file stream in TelBook class
	book1.closefstream();

	return 0;
}

//add an entry into the TelBook
void addEntry( TelBook &book, const SOCKET &client )
{
	string name, mobile, address;

	sendMsg( client, "name: ");			
	sendMsg( client, "$" );	
	recvMsg( client, name );			
	while( name == "" )
	{
		sendMsg( client, "!! Please input the name.\n" );	
		sendMsg( client, "name: " );					
    	sendMsg( client, "$" );	
		recvMsg( client, name );						
	}

	//input the mobile number
	//the length of the mobile need to be checked 
	sendMsg( client, "mobile: " );						
	sendMsg( client, "$" );	
	recvMsg( client, mobile );							
	while( !book.checkMobile(mobile) )
	{
		sendMsg( client, "!! Please input correct mobile number.\n" );	
		sendMsg( client, "mobile: " );									
		sendMsg( client, "$" );	
		recvMsg( client, mobile );										
	}

	sendMsg( client, "address: " );						
	sendMsg( client, "$" );	
	recvMsg( client, address );						
	while( address == "" )
	{
		sendMsg( client, "!! Please input correct address.\n" );			
		sendMsg( client, "address: " );									
		recvMsg( client, address );											
	}

	PeopleData person(name, mobile, address);
	book.addRecord(person, client);

}

//find entries in the TelBook and its file using name|mobile|address
void findEntry( TelBook &book, const SOCKET &client )
{
	string inputCMD;
	sendMsg( client, "by (name|mobile|address): " );		
	sendMsg( client, "$" );	
	recvMsg( client, inputCMD );							
															

	while(true)
	{
		if( inputCMD == "name" )
		{	
			string name;
			sendMsg( client, "name: " );					
			sendMsg( client, "$" );	
			recvMsg( client, name );						
			book.searchName( name, client );
			break;
		}
		else
			if( inputCMD == "mobile" )
			{	
				string mobile;
				sendMsg( client, "mobile: " );					
				sendMsg( client, "$" );	
				recvMsg( client, mobile );						
				book.searchMobile( mobile, client ); 
				break;
			}
			else
				if( inputCMD == "address" )
				{	
					string address;
					sendMsg( client, "address: " );					
					sendMsg( client, "$" );	
					recvMsg( client, address );						
					book.searchAddress( address, client );
					break;
				}
				else
				{
					sendMsg( client, "!!Invalid Command\n" );				
					sendMsg( client, "by (name|mobile|address): " );	
					sendMsg( client, "$" );	
					recvMsg( client, inputCMD );						
				}
	}

}

//delete entries in TelBook and its file using name|mobile|address
void deleteEntry( TelBook &book, const SOCKET &client )
{
	string inputCMD;
	sendMsg( client, "by (name|mobile|address): " );		
	sendMsg( client, "$" );	
	recvMsg( client, inputCMD );							
														

	while(true)
	{
		if( inputCMD == "name" )
		{	
			string name;
			sendMsg( client, "name: " );					
			sendMsg( client, "$" );	
			recvMsg( client, name );						
			book.deleteName( name, client );
			break;
		}
		else
			if( inputCMD == "mobile" )
			{	
				string mobile;
				sendMsg( client, "mobile: " );				
				sendMsg( client, "$" );	
				recvMsg( client, mobile );					
				book.deleteMobile( mobile, client ); 
				break;
			}
			else
				if( inputCMD == "address" )
				{	
					string address;
					sendMsg( client, "address: " );				
					sendMsg( client, "$" );	
					recvMsg( client, address );						
					book.deleteAddress( address, client );
					break;
				}
				else
				{
					sendMsg( client, "!!Invalid Command\n" );				
					sendMsg( client, "by (name|mobile|address): " );	
					sendMsg( client, "$" );	
					recvMsg( client, inputCMD );						
				}
	}

}

//help menu
void showHelp( TelBook &book, const SOCKET &client )
{
	sendMsg( client, "    add       --add a new entry into the address book;\n" );
	sendMsg( client, "    search    --search for and display entries with a specific name|mobile|address;\n" );
	sendMsg( client, "              use .* for fuzzy search\n" );
	sendMsg( client, "    remove    --remove entries with a specific name|mobile|address;\n" );
	sendMsg( client, "              use .* for fuzzy delete\n" );
	sendMsg( client, "    display   --display all the entries in the address book;\n" );
	sendMsg( client, "    delete    --delete all the entries in the address book;\n" );
	sendMsg( client, "    !help     --display this help;\n" );
	sendMsg( client, "    !quit     --exit.\n" );
}

//deleta all the entries in TelBook and its file
void deleteAllEntry( TelBook &book, const SOCKET &client )
{
	string yesno;

	do
	{
		//the delete operation need to be checked by the user
		sendMsg( client, "Do you really want to delete all the entries? (y/n): " );
		sendMsg( client, "$" );	
		recvMsg( client, yesno );		

		if( yesno == "y" )
		{
			sendMsg( client, "Deleting...\n" );					
			book.deletaAll();
			sendMsg( client, "All entries are deleted.\n" );		
		}

	}while( yesno!="y" && yesno!="n" );

}

//display all the entries
void displayEntry( TelBook &book, const SOCKET &client )
{
	book.displayAllRecords( client );
}
