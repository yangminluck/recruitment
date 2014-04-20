#include "socketInOut.h"

//empty loop
void waitTime( int count )
{
	for( int i = 0; i < count*100; i++ ){}
}


//send message by dst socket
void sendMsg( const SOCKET &dst, const string &msg )
{
	char OutBuffer[MAX_PATH];
	memset( OutBuffer, 0, sizeof(OutBuffer) );

	strcpy( OutBuffer, msg.c_str() );
	send( dst, OutBuffer, (int)strlen(OutBuffer)+1, 0 );

	//wait
	waitTime(50000);
}

//receive message from dst socket
void recvMsg( const SOCKET &dst, string &msg )
{
	char InBuffer[MAX_PATH];
	memset( InBuffer, 0, sizeof(InBuffer) );

	recv( dst, InBuffer, sizeof(InBuffer), 0 );

	msg = InBuffer;
}

