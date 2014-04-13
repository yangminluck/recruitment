//LinkedClass.h
//Class LinkedClass definition 

#ifndef LINKEDCLASS_H
#define LINKEDCLASS_H

#include <iostream>
#include <cstring>
using namespace std;

typedef struct node
{
    string name;
    string mobile;
    string address;
    struct node *next;
}LinkedList;

class LinkedClass
{
public:
    LinkedClass(void);
    ~LinkedClass(void);

public:
    void IntLinkedList(LinkedList* head);
    void Print(LinkedList* head);
    LinkedList* InsertLinkedList(LinkedList* head);
    void WriteFile(LinkedList* head);
    LinkedList* LinkedListSearch(LinkedList *head, string search_key, int search_id);
    LinkedList* LinkedListDelete(LinkedList *head, string search_key, int search_id);
    void ShowHelp();
};

#endif
