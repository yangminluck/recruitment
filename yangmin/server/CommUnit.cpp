using namespace std;
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<errno.h>
#include"CommUnit.h"
void TCPServer::Socket()
{
    if(sockfd==-1)
    sockfd=socket(AF_INET,SOCK_STREAM,0);

}
void TCPServer::Bind()
{
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(51111);
    servaddr.sin_addr.s_addr=INADDR_ANY;
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
}
void TCPServer::Listen()
{
    listen(sockfd,10);
}
int TCPServer::Accept()
{
    return accept(sockfd,NULL,NULL);
}

ssize_t readn(int fd,void *buf, size_t num)
{
    ssize_t res;
    size_t n;
    char *ptr;
    n=num;
    ptr =(char*) buf;
    while(n>0)
    {
        if((res=read(fd, ptr, n))==-1)
        {
          if(errno==EINTR)
            res=0;
          else
            return (-1);
        }
        else if(res==0)
        break;

        ptr+=res;
        n-=res;
    }
    return(num-n);
}
ssize_t writen(int fd, const void *buf, size_t num)
{
    ssize_t res;
    size_t n;
    const char *ptr;
    n = num;
    ptr =(const char*)buf;
    while(n>0)
    {

        if((res=write(fd, ptr, n))<=0)
        {
           if (errno==EINTR)
             res=0;
           else
             return(-1);
        }
        ptr+=res;
        n-=res;
    }
    return(num);
}
/*
name  : ssize_t Send(int MSG,char *data,size_t length)
usage : package the data and send it
return: -1  connection error or failed to send a package containing length bytes of data
        0   send 0
        n   send succeed
*/
ssize_t TCPServer::Send(int MSG,char *data,size_t length)
{
    if(sockfd==-1)return(-1);
    if(data==NULL)return(0);
    if(length==0) return(0);
    DataHead headpkg;
    headpkg.MSG=MSG;
    headpkg.length=length;
    ssize_t n=writen(sockfd,(void*)&headpkg,sizeof(headpkg));
    if(n<0||n!=sizeof(headpkg)) return (-1);

    n=writen(sockfd,(void*)data,length);
    if(n<0||n!=length) return (-1);
    else return n;
}
/*
name  : ssize_t Recv(int &MSG,char *buf,size_t length)
usage : receive a package and unpack it to buf
return: -1  connection error or failed to receive a whole package
        0   receive 0
        n   receive a whole package
*/
ssize_t TCPServer::Recv(int &MSG,char *buf,size_t length)
{
    if(sockfd==-1)return(-1);
    if(buf==NULL) return(0);
    if(length==0) return(0);
    DataHead headpkg;
    bzero(buf,sizeof(buf));
    bzero((char *)&headpkg,sizeof(headpkg));

    //read the head of data diagram
    ssize_t n=readn(sockfd,(void *)&headpkg,sizeof(headpkg));
    if(n!=sizeof(headpkg))return (-1);
    if(length < headpkg.length)return (-1);
    MSG=headpkg.MSG;

    //read the body of data diagram
    n=readn(sockfd,(void *)buf,headpkg.length);
    if(n<0||n!=headpkg.length) return (-1);
    else return n;
}

void TCPServer::Close()
{
    if(sockfd==-1) return;
    close(sockfd);
    sockfd=-1;
}

