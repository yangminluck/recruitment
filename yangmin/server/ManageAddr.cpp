using namespace std;
#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<string.h>
#include"CommUnit.h"
#include"ManageAddr.h"

ManageAddr::~ManageAddr()//put temporary changed entries into file and delete data
{
    entryaddr *p;
    if(filechanged&&data!=NULL)
    {
        WriteFile(data,0);//clear file and put entries into file
    }
    while(data!=NULL)//delete data
    {
        p=data;
        data=data->next;
        delete p;
    }
}
/*
name    :entryaddr* ReadFile()
function:open file,read dentries and saved as entryaddr*
return  :the head of the list of dentries
*/
entryaddr* ManageAddr::ReadFile()
{
    entryaddr *head=NULL;
    if(filename.size()==0)
    {
        cerr<<"can't read:no file name!"<<endl;
        return NULL;
    }
    ifstream infile(filename.c_str());
    if(!infile)
    {
        cerr<<"can't read:file "<<filename<<" not exit!"<<endl;
        return NULL;
    }
    string word,str;
    entryaddr *temp;
    string::size_type pos1,pos2;
    while(getline(infile,word))
    {
        if(word=="")continue;
        temp=new entryaddr;
        //get name from dentry
        pos1=0;
        pos2=word.find(separator,pos1);
        if(pos2==string::npos){delete temp;continue;}
        temp->name=word.substr(pos1,pos2-pos1);
        //get mobile from dentry
        pos1=pos2+1;
        pos2=word.find(separator,pos1);
        if(pos2==string::npos){delete temp;continue;}
        temp->mobile=word.substr(pos1,pos2-pos1);
        //get address from dentry
        temp->address=word.substr(pos2+1);
        
        //add temp to the list of dentry
        if(head==NULL){head=temp;temp->next=NULL;}
        else{temp->next=head;head=temp;}
    }
    return head;
}
/*
name    :void WriteFile()
function:open file,write dentries(data) into file
          flag=0: clear the file and write
         flag!=0: append data to file
*/
void ManageAddr::WriteFile(entryaddr *head,int flag)
{
    if(head==NULL&&flag!=0)
    {
        cerr<<"can't write:no data!"<<endl;
        return;
    }
    if(filename.size()==0)
    {
        cerr<<"can't write:no file name"<<endl;
        return;
    }
    ofstream outfile;
    if(flag==0)//clear file and write
    {
        outfile.open(filename.c_str());
    }
    else //append data to file
    {
        outfile.open(filename.c_str(),ios::app);
    }

    if(!outfile)
    {
        cerr<<"can't write:unable to open file "<<filename<<endl;
        return;
    }
    entryaddr *p=head;
    while(p!=NULL)
    {
        outfile<<p->name<<","<<p->mobile<<","<<p->address<<endl;
        p=p->next;
    }
}

/*
interface:int AddEntry()
usage    :called by command "add"
return   :0   operation failed
          1   operation succeed
          -1  connection error
*/
int ManageAddr::AddEntry(CommUnit *serv,const string &word)
{
    if(serv==NULL)return -1;
    entryaddr *temp=new entryaddr;
    string::size_type pos1,pos2;
    //get name from word
    pos1=0;
    pos2=word.find(separator,pos1);
    if(pos2==string::npos){delete temp;return 0;}
    temp->name=word.substr(pos1,pos2-pos1);
    //get mobile from word
    pos1=pos2+1;
    pos2=word.find(separator,pos1);
    if(pos2==string::npos){delete temp;return 0;}
    temp->mobile=word.substr(pos1,pos2-pos1);
    //get address from word
    temp->address=word.substr(pos2+1);
    temp->next=NULL;
    if(!filechanged){WriteFile(temp,1);delete temp;}
    else if(data!=NULL)
    {
        temp->next=data;
        data=temp;
    }
    else data=temp;
    char buf[100];
    strcpy(buf,"address entry added\n");
    ssize_t n=serv->Send(0,buf,strlen(buf)+1);
    if(n<0)return -1;
    return 1;
}
/*
name     :entryaddr* doremove()
usage    :called by interface "RemoveEntry()"
function :remove dentries(=str) from the list of entries
return   :the head of the list of entries
*/
entryaddr * ManageAddr::doremove(entryaddr *head,string &str,int flag,int &count)
{
    if(head==NULL) return head;

    bool finded;
    entryaddr *pre=head,*temp=head->next;
    while(temp!=NULL)
    {
        finded=false;
        switch(flag)
        {
            case 1:if(matching(temp->name,str))finded=true;break;
            case 2:if(matching(temp->mobile,str))finded=true;break;
            case 3:if(matching(temp->address,str))finded=true;break;
            default:break;
        }
        if(finded)
        {
            count++;
            pre->next=temp->next;
            delete temp;
            temp=pre->next;
        }
        else
        {
            pre=pre->next;
            temp=temp->next;
        }
    }
    finded=false;
    switch(flag)
    {
        case 1:if(matching(head->name,str))finded=true;break;
        case 2:if(matching(head->mobile,str))finded=true;break;
        case 3:if(matching(head->address,str))finded=true;break;
        default:break;
    }
    if(finded)
    {
        count++;
        temp=head;
        head=head->next;
        delete temp;
    }
    return head;
}
/*
interface:int RemoveEntry()
usage    :called by command "delete"
return   :0   operation failed
          1   operation succeed
          -1  connection error
*/
int ManageAddr::RemoveEntry(CommUnit *serv,const string &word)
{
    if(serv==NULL) return -1;
    int flag,count=0;
    string choice,str;
    
    string::size_type pos;
    pos=word.find(separator,0);
    if(pos==string::npos)return 0;
    choice=word.substr(0,pos);
    str=word.substr(pos+1);
    
    if(choice=="name")        flag=1;
    else if(choice=="mobile") flag=2;
    else if(choice=="address")flag=3;
    else
    {
        cerr<<"wrong option"<<endl;
        return 0;
    }
    if(filechanged)
    {
        data=doremove(data,str,flag,count);
    }
    else
    {
        if(data==NULL) data=ReadFile();
        data=doremove(data,str,flag,count);
        filechanged=true;
    }
    char buf[100];
    sprintf(buf,"%d address entries deleted\n",count);
    ssize_t n=serv->Send(0,buf,strlen(buf)+1);
    if(n<0) return -1;
    return 1;
}
bool ManageAddr::matching(string &dest,string &str)
{
    string::size_type pos;
    if((pos=str.find(".*",0))==string::npos)
    {
        if(dest==str)return true;
        else return false;
    }
    else if(dest.substr(0,pos)==str.substr(0,pos))
        return true;
    else return false;
}
/*
interface:int SearchEntry()
usage    :called by command "search"
return   :0   operation failed
          1   operation succeed
          -1  connection error
*/
int ManageAddr::SearchEntry(CommUnit *serv,const string &word)
{
    if(serv==NULL) return -1;
    int flag;
    string choice,str;
    string::size_type pos;
    pos=word.find(separator,0);
    if(pos==string::npos)return 0;
    choice=word.substr(0,pos);
    str=word.substr(pos+1);

    if(choice=="name")        flag=1;
    else if(choice=="mobile") flag=2;
    else if(choice=="address")flag=3;
    else
    {
        cerr<<"wrong option"<<endl;
        return 0;
    }
    if(!filechanged&&data==NULL)  data=ReadFile();
    bool finded;
    entryaddr *temp=data;
    char buf[1000];
    char item[200];
    unsigned int length=0,len;
    while(temp!=NULL)
    {
        finded=false;
        switch(flag)
        {
            case 1:if(matching(temp->name,str))finded=true;break;
            case 2:if(matching(temp->mobile,str))finded=true;break;
            case 3:if(matching(temp->address,str))finded=true;break;
            default:break;
        }
        if(finded)
        {
            sprintf(item,"name:%s,mobile:%s,address:%s\n",temp->name.c_str(),
            temp->mobile.c_str(),temp->address.c_str());
            len=strlen(item);
            if(length>0&&length+len>=1000)
            {
                buf[length]='\0';
                ssize_t n=serv->Send(0,buf,length+1);
                if(n<0)return -1;
                length=0;
            }
            strcpy(buf+length,item);
            length+=len;
        }
        temp=temp->next;
    }
    buf[length]='\0';
    ssize_t n=serv->Send(0,buf,length+1);
    if(n<0)return -1;
    return 1;
}




