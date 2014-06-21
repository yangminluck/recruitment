#ifndef _CommUnit_Head
#define _CommUnit_Head
#include<netinet/in.h>
struct DataHead
{
    int MSG;
    size_t length;
};
class CommUnit
{
    public:
    virtual ssize_t Send(int ,char *,size_t){}
    virtual ssize_t Recv(int &,char *,size_t){}
    virtual ~CommUnit(){}
};
class TCPServer:public CommUnit
{
    private:
    int sockfd;
    struct sockaddr_in servaddr;
    public:
    TCPServer(){sockfd=-1;}
    TCPServer(int i){if(sockfd=-1&&i>0)sockfd=i;}
    ~TCPServer(){if(sockfd!=-1)close(sockfd);}

    void Socket();
    void Bind();
    void Listen();
    int Accept();
    ssize_t Send(int ,char *,size_t);
    ssize_t Recv(int &,char *,size_t);
    void Close();
};
#endif
