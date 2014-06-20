#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<errno.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<string>
#include<iostream>
#include<pthread.h>
using namespace std;
struct DataHead
{
    int MSG;
    size_t length;
};
struct CmdPkg
{
    char cmd[20];
    char data[200];
};
ssize_t readn(int fd,void *buf, size_t num)
{
    ssize_t res;
    size_t n;
    char *ptr;
    n = num;
    ptr =(char*) buf;
    while (n > 0)
    {
        if ((res = read (fd, ptr, n)) == -1)
        {
          if (errno == EINTR)
            res = 0;
          else
            return (-1);
        }
        else if (res == 0)
        break;

        ptr += res;
        n -= res;
    }
    return (num - n);
}
ssize_t writen (int fd, const void *buf, size_t num)
{
    ssize_t res;
    size_t n;
    const char *ptr;
    n = num;
    ptr = (const char *)buf;
    while (n > 0)
    {
        if ((res = write (fd, ptr, n)) <= 0)
        {
            if (errno == EINTR)
                res = 0;
            else
               return (-1);
        }
        ptr += res;
        n -= res;
    }
    return (num);
}

ssize_t Send(int sockfd,int MSG,char *data,size_t length)
{
    if(sockfd==-1)return(-1);
    if(data==NULL)return 0;
    if(length==0) return 0;
    DataHead headpkg;
    headpkg.MSG=MSG;
    headpkg.length=length;
    //send  head
    ssize_t n=writen(sockfd,(void*)&headpkg,sizeof(headpkg));
    if(n<0||n!=sizeof(headpkg)) return (-1);
    //send data
    n=writen(sockfd,(void*)data,length);
    if(n<0||n!=length) return (-1);
    else return n;
}
ssize_t Recv(int sockfd,int &MSG,char *buf,size_t length)
{
    if(sockfd==-1)return(-1);
    if(buf==NULL) return(0);
    if(length==0) return(0);
    DataHead headpkg;
    bzero(buf,sizeof(buf));
    bzero((char *)&headpkg,sizeof(headpkg));

    //read the head of data diagram
    ssize_t n=readn(sockfd,(void *)&headpkg,sizeof(headpkg));
    if(n<0||n!=sizeof(headpkg))return (-1);
    if(length < headpkg.length)return (-1);
    MSG=headpkg.MSG;
    
    //read the body of data diagram
    n=readn(sockfd,(void *)buf,headpkg.length);
    if(n<0||n!=headpkg.length) return (-1);
    else return n;
}
void recvthread(void *fd)
{
    int MSG,connfd=(int)fd;
    ssize_t n;
    char buf[1000];
    while(1)
    {
        n=Recv(connfd,MSG,buf,sizeof(buf));
        if(n<0) {close(connfd);return;}
        else cout<<buf<<endl;
    }
}


int main(int argc,char *argv[])
{
    struct sockaddr_in servaddr;
    int connfd;
    bzero((char*)&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(51111);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    connfd=socket(AF_INET,SOCK_STREAM,0);
    connect(connfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

    int ret;
    pthread_t tid;
    typedef void*(*func)(void*);
    ret=pthread_create(&tid,NULL,(func)&recvthread,(void*)connfd);
    if(ret!=0)
    {
        cout<<"create thread error!\n";
        return -1;
    }
    CmdPkg sendpkg,recvpkg;
    size_t lenpkg;
    string str;
    while(1)
    {
       cout<<argv[0]+2<<'>';
       getline(cin,str);
       while(str=="")getline(cin,str);
       if(str.size()>sizeof(sendpkg.cmd))
       {cout<<"command exceed length!"<<endl;continue;}
       strcpy(sendpkg.cmd,str.c_str());
       string word;
       
       if((strcmp(sendpkg.cmd,"add"))==0)
       {
           cout<<"name:";
           getline(cin,str);
           while(str=="")getline(cin,str);
           word+=str+",";
           cout<<"mobile:";
           getline(cin,str);
           while(str=="")getline(cin,str);
           word+=str+",";
           cout<<"address:";
           getline(cin,str);
           while(str=="")getline(cin,str);
           word+=str;
           if(word.size()>=sizeof(sendpkg.data))
           {cout<<"message exceed package length!"<<endl;continue;}
           strcpy(sendpkg.data,word.c_str());
           lenpkg=sizeof(sendpkg.cmd)+strlen(sendpkg.data)+1;
       }
       else if((strcmp(sendpkg.cmd,"search"))==0||(strcmp(sendpkg.cmd,"delete"))==0)
       {
           int flag;
           cout<<"by (name|mobile|address):";
           getline(cin,str);
           while(str=="")getline(cin,str);
           if(str=="name")        {flag=1;word+="name,";}
           else if(str=="mobile") {flag=2;word+="mobile,";}
           else if(str=="address"){flag=3;word+="address,";}
           else
           {
              cerr<<"wrong option!"<<endl;
              continue;
           }
           switch(flag)
           {
               case 1:cout<<"name:";break;
               case 2:cout<<"mobile:";break;
               case 3:cout<<"address:";break;
               default:break;
           }
           getline(cin,str);
           while(str=="")getline(cin,str);
           word+=str;
           if(word.size()>sizeof(sendpkg.data))
           {cout<<"message exceed package length!"<<endl;continue;}
           strcpy(sendpkg.data,word.c_str());
           lenpkg=sizeof(sendpkg.cmd)+strlen(sendpkg.data)+1;
       }
       else if((strcmp(sendpkg.cmd,"!quit"))==0)
       {
           lenpkg=sizeof(sendpkg.cmd);
           ssize_t n=Send(connfd,1,(char *)&sendpkg,lenpkg);
           if(n<0||n!=lenpkg) {cout<<"connection error£¡\n";return -1;}
           return 0;
       }
       else if((strcmp(sendpkg.cmd,"!help"))==0)
       {
           lenpkg=sizeof(sendpkg.cmd);
       }
       else
       {
           cout<<"wrong command!";
           strcpy(sendpkg.cmd,"!help");
           lenpkg=sizeof(sendpkg.cmd);

       }
       ssize_t n= Send(connfd,1,(char *)&sendpkg,lenpkg);
       if(n<0||n!=lenpkg){cout<<"connection error£¡\n";return -1;}
       sleep(1);
    }
    return 0;
}

