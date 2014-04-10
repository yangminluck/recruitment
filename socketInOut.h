//socketInOut.h
//environment configuraiton

#ifndef SOCKETINOUT_H
#define SOCKETINOUT_H

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

//header files and macro for linux
#ifndef _LINUX_
#include <pthread.h>     
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <netinet/in.h>   

typedef int SOCKET;
typedef unsigned char BYTE;
typedef unsigned long DWORD;
#define FALSE 0
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define MAX_PATH 260

#endif

using namespace std;

//send message by dst socket
void sendMsg( const SOCKET &dst, const string &msg );
//receive message from dst socket
void recvMsg( const SOCKET &dst, string &msg );
//empty loop
void waitTime( int count );


#endif
