#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "LinkedClass.h"
using namespace std;

LinkedClass::LinkedClass(void)
{
}

LinkedClass::~LinkedClass(void)
{
}

//read the address information from file
void LinkedClass::IntLinkedList(LinkedList* head)
{
    LinkedList *node, *h; 
    string temp;
    ifstream fin;
    fin.open("data.txt",ios::in); 
    cout<<"The file is:"<<endl;
    if(fin)
    {             
        fin>>noskipws;     
        char *word = new char[100];
        while(!fin.eof())
        {  
            getline(fin, temp,'\n');
            strcpy(word, temp.c_str());
            char *p;
            p = strtok(word," ");
            int number = 0;
            string sa[3];
            while(p)
            {   
                sa[number++] = p;
                p = strtok(NULL, " ");   
            }
            h = head;
            node = new LinkedList;
            for(int i=0; i<number; i++)
            {
                if(sa[i] == "\n")
                    cout<<"ok"<<endl;
            }
            node->name = sa[0];
            node->mobile = sa[1];
            node->address = sa[2];
            node->next = NULL;
            while(h->next)
                h = h->next;
            h->next = node;
        }
    }
    else
        cout<<"No data now!"<<endl; 
    fin.close();
}

//display the address information in term of linked list
void LinkedClass::Print(LinkedList* head)
{
    LinkedList *h;
    h = head->next;
    while(h)
    {
        cout<<h->name<<" "<<h->mobile<<" "<<h->address<<endl;
        h = h->next;
    }
}

//Insert the address information into linked list
LinkedList* LinkedClass::InsertLinkedList(LinkedList* head)
{
    LinkedList *node, *h;
    h = head;
    node = new LinkedList;
    cout<<"name:";
    cin>>node->name;
    cout<<"mobile:";
    cin>>node->mobile;
    cout<<"address:";
    cin>>node->address;
    node->next = NULL;   
    while(h->next)
        h = h->next;
    h->next = node;
    return head;
}

//Write the address information into file
void LinkedClass::WriteFile(LinkedList* head)
{
    LinkedList *h;
    h = head->next;
    ofstream fout;
    fout.open("data.txt");
    if(h)
    {
        while(h->next)
        {
            fout<<h->name<<" "<<h->mobile<<" "<<h->address<<endl;
            h = h->next;
        }
            fout<<h->name<<" "<<h->mobile<<" "<<h->address;
    }
    fout.close();
}

//judge similiar string
int is_sub_str(string str, string sub_str)
{
    int i,j,k;
    int number = 0;
    int len = sub_str.length();
    int flag = 0;
    char *str_char = new char;
    char *sub_str_char = new char;
    strcpy(str_char, str.c_str());
    strcpy(sub_str_char, sub_str.c_str());

    for(i=0; str_char[i]!=0; i+=k)
    {
        k = 1;
        for(j=0; sub_str_char[j]!=0; j++)
        {
            if(str_char[i+j] != sub_str_char[j])
                break;
        }
        if(j == len)
        {
            k = len;
            flag = 1;
        }
    }
    return flag;
}

//Search the address information from the linked list
LinkedList* LinkedClass::LinkedListSearch(LinkedList *head, string search_key, int search_id)
{
    LinkedList *p, *h;
    h=head;
    string result;
    string key = search_key;
    int count = 0;
    if(is_sub_str(search_key, ".*") == 1)
        key = key.substr(0, search_key.length()-strlen(".*"));
    while(h)
    {
        switch(search_id)
        {
            case 1:
                result = h->name;
                break;
            case 2:
                result = h->mobile;
                break;
            case 3:
                result = h->address;
                break;
        }
        if( is_sub_str(result, key) == 0 )
        {
            p = h;      
            h = h->next;
        }
        else
        {
            cout<<h->name<<" "<<h->mobile<<" "<<h->address<<endl;
            count++;
            h = h->next;
        }
    }
    cout<<count<<" address entries searched"<<endl;
    return head;
}

//Delete the address information from the linked list
LinkedList* LinkedClass::LinkedListDelete(LinkedList *head, string search_key, int search_id)
{
    LinkedList *p, *h;
    h = head->next;
    int count = 0;
    string result;
    string key = search_key;
    if(is_sub_str(search_key, ".*") == 1)
        key = key.substr(0, search_key.length()-strlen(".*"));
    while (h)
    {
        switch(search_id)
        {
            case 1:
                result = h->name;
                break;
            case 2:
                result = h->mobile;
                break;
            case 3:
                result = h->address;
                break;
        }
        if( is_sub_str(result, key) == 0 )
        {
            p = h;      
            h = h->next;
        }
        else
        {
            if ( h == head->next)
            {
                head->next = h ->next;
                delete h;
                h = head->next;
                count++;
            } 
            else
            {              
                p->next=h->next;
                delete h;
                h = p->next;
                count++;
            } 
        }
    }
    cout<<count<<" address entries deleted"<<endl;
    return head;
}

//Show help message
void LinkedClass::ShowHelp()
{
    cout<<"help message"<<endl;
    cout<<"#add  this order is used to add relevent information"<<endl;
    cout<<"#search  this order is used to search relevent information"<<endl;
    cout<<"#delete  this order is used to delete relevent information"<<endl;
    cout<<"#dispaly  this order is used to display relevent information"<<endl;
}
