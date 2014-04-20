#include <iostream>
#include <cstring>
#include "LinkedClass.h"
using namespace std;

int main()
{
    LinkedClass link;
    string order,name,mobile,address,key,search_key;
    LinkedList *head = new LinkedList;
    head->next = NULL;
    link.IntLinkedList(head);
    link.Print(head);

    //input order command
    while(order != "!quit")
    {
        cout<<"Please input command:"<<endl; 
        cin>>order;
        if(order == "add")
        {
            head = link.InsertLinkedList(head);
            cout<<"address entry added"<<endl;
            link.WriteFile(head);
        }
        else if(order == "search")
        {
            cout<<"by (name|mobile|address):";
            cin>>key;
            if(key == "name")
            {
                cout<<"name:";
                cin>>search_key;
                head = link.LinkedListSearch(head, search_key, 1);
            }
            if(key == "mobile")
            {
                cout<<"mobile:";
                cin>>search_key;
                head = link.LinkedListSearch(head, search_key, 2);
            }
            if(key == "address")
            {
                cout<<"address:";
                cin>>search_key;
                head = link.LinkedListSearch(head, search_key, 3);
            }
        }
        else if(order == "delete")
        {
            cout<<"by (name|mobile|address):";
            cin>>key;
            if(key == "name")
            {
                cout<<"name:";
                cin>>search_key;
                head = link.LinkedListDelete(head, search_key, 1);
                link.WriteFile(head);
            }
            if(key == "mobile")
            {
                cout<<"mobile:";
                cin>>search_key;
                head = link.LinkedListDelete(head, search_key, 2);
                link.WriteFile(head);
            }
            if(key == "address")
            {
                cout<<"address:";
                cin>>search_key;
                head = link.LinkedListDelete(head, search_key, 3);
                link.WriteFile(head);
            }
        }
        else if(order == "display")
            link.Print(head);
        else if(order == "!help")
            link.ShowHelp();
        else if(order == "!quit")
            break;
        else
            link.ShowHelp();
    }
    return 0;
}
