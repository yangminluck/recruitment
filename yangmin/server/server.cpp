using namespace std;
#include<iostream>
#include<string>
#include<pthread.h>
#include<errno.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include"CommUnit.h"
#include"ManageAddr.h"


struct CmdPkg
{
    char cmd[20];
    char data[200];
};
pthread_mutex_t mutex;
pthread_mutexattr_t attr;
ManageAddr texts("/home/yangmin.txt");
int Screen(CommUnit *serv)
{
    int MSG,ret;
    CmdPkg recvpkg;
    char *tips="avalible commands:\nadd  search  delete  !help  !quit\n";
    while(1)
    {
        ssize_t n=serv->Recv(MSG,(char*)&recvpkg,sizeof(recvpkg));
        if(n<0) return -1;
        if(MSG!=1)return -1;
        
        pthread_mutex_lock(&mutex);
        if((strcmp(recvpkg.cmd,"add"))==0)
        {
            ret=texts.AddEntry(serv,string(recvpkg.data));
            if(ret==-1){pthread_mutex_unlock(&mutex);return -1;}
            else if(ret==0)cout<<"add failed\n";
        }
        else if(strcmp(recvpkg.cmd,"search")==0)
        {
            ret=texts.SearchEntry(serv,string(recvpkg.data));
            if(ret==-1){pthread_mutex_unlock(&mutex);return -1;}
            else if(ret==0)cout<<"search failed\n";
        }
        else if(strcmp(recvpkg.cmd,"delete")==0)
        {
            ret=texts.RemoveEntry(serv,string(recvpkg.data));
            if(ret==-1){pthread_mutex_unlock(&mutex);return -1;}
            else if(ret==0)cout<<"delete failed\n";
        }
        else if(strcmp(recvpkg.cmd,"!help")==0)
        {
            ssize_t n=serv->Send(0,tips,strlen(tips)+1);
            if(n<0) {pthread_mutex_unlock(&mutex);return -1;}
        }
        else if(strcmp(recvpkg.cmd,"!quit")==0)
            {pthread_mutex_unlock(&mutex);return 0;}
        else
        {
            ssize_t n=serv->Send(0,tips,strlen(tips)+1);
            if(n<0){pthread_mutex_unlock(&mutex);return -1;}
        }
        pthread_mutex_unlock(&mutex);
    }
}
int tcpthread(void* fd)
{
    TCPServer serv((int)fd);
    cout<<"thread "<<pthread_self()<<" init\n";
    int ret=Screen(&serv);
    close((int)fd);
    cout<<"thread "<<pthread_self()<<" exit "<<ret<<endl;
   return 0;
}
void handle(int sig)
{
    exit(0);
}

int main()
{
  TCPServer serv;
  serv.Socket();
  serv.Bind();
  serv.Listen();
  int connfd,ret; 
  pthread_t tid;
  typedef void*(*func)(void*);
  signal(SIGINT,handle);
  pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&mutex, &attr);
  while(1)
  {
    connfd=serv.Accept();
    if(connfd>0)
    {

       ret=pthread_create(&tid,NULL,(func)&tcpthread,(void*)connfd);
       if(ret!=0)
       {
          cout<<"create thread error!\n";
          return -1;
       }
    }        
  }
}



