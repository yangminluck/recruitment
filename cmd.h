//cmd.h
//main operations in address book sever

#ifndef CMD_H
#define CMD_H

#include <iostream>
#include <limits.h>

#include "TelBook.h"			//TelBook class definition

using namespace std;

//function prototype
void addEntry( TelBook &book, const SOCKET &client );			
void findEntry( TelBook &book, const SOCKET &client );
void deleteEntry( TelBook &book, const SOCKET &client );
void showHelp( TelBook &book, const SOCKET &client );
void displayEntry( TelBook &book, const SOCKET &client );
void deleteAllEntry( TelBook &book, const SOCKET &client );

//the struct of command and operation pair
typedef struct cmd_fun_pair{			
   const char* StrCmd;
   void (*CmdFun)( TelBook &book, const SOCKET &client );
}CMD_FUN_PAIR; 

//main function in the sever thread
int main_cmd( const SOCKET &client );



#endif