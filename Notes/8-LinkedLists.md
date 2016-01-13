# Linked Lists

Arrays can store a fixed number of items. If the number of items is not known,
they are not helpful. Arrays are also limited when removing or adding items to
the middle.

A linked list is a data structure in which each item contains a pointer to the
next item, it is easier to insert or delete elements with one than with an array.
A linked list is less efficient to initialize than an array. Each item in a linked
list is called a node. The first pointer in a linked list is called the head.

Linked lists are typically created using dynamic variables.
Example:
```
struct Node
{
  string value;
  Node* next;
}
int main()
{
  Node *head, *second, *third;
  head = new Node;
  second = new Node;
  third = new Node;

  head->value = "shells";
  head->next = second;

  second->value = "books";
  second->next = third;

  third->value = "money";
  third->next = nullptr;

  delete head;
  delete second;
  delete third;
}
```
To allocate new nodes:
```
Node *p = new Node;
Node *q = new Node;
```
To change the value of node p:
```
p->value = "something";
```
To link node p to node q:
```
p->next = q;
```
To get the address of the node after p:
```
Node *r = p->next;
```
To make q a terminal node:
```
q->next = nullptr;
```
To free nodes (nodes must be reverse order):
```
delete q;
delete p;
```

Normally, a linked list isn't created all at once, instead a dedicated class (ADT)
is used to hold the linked list, with member functions to access and modify nodes.

Example:
```
struct Node
{
  string value;
  Node* next;
};

class LinkedList
{
public:
  LinkedList();
  void addToFront(...);
  void addToRear(...);
  void deleteItem(...);
  bool findItem(...);
  void printItems(...);
private:
  Node *head;
}
```

To make a constructor for a linked list:
```
LinkedList::LinkedList()
{
  head = nullptr;
}
```

Pseudocode for printItems():
```
void LinkedList::printItems()
{
  ????
}
```
