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
  void addItem(...);
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

Traversal of linked list, i.e. printItems():
```
void LinkedList::printItems()
{
    for(Node *p = head; p != nullptr; p = p->next)
      cout << p->value << endl;
}
```

Adding nodes:
```
void LinkedList::addToFront(string v)
{
  Node *p = new Node;
  p->value = v;
  p->next = head;
  head = p;
}
```
```
void LinkedList::addToRear(string v)
{
  if(head == nullptr)
    addToFront(v);
  else
  {
    Node *p = head;
    while(p->next != nullptr)
      p = p->next;
    Node *q = new Node;
    q->value = v;
    q->next = nullptr;
    p->next = q;
  }
  //alternately a tail pointer can be used
}
```
```
//assume list is ordered alphabetically
void LinkedList::addItem(string v)
{
  if(head == nullptr)
    addToFront(v);
  else if(v < head->value)
    addToFront(v);
  else
  {
    Node *p = head;
    while(p->next != nullptr)
      if(v >= p->value && v <= p->next->value)
        break;
      p = p->next
  }
  Node *q = new Node;
  q-> value = v;
  q->next = p->next;
  p->next = q;
}
```

Deleting items:
```
void LinkedList::deleteItem(string v)
{
    if(head == nullptr)
      return;
    else if(v == head->value)
    {
      Node *deleteThis = head;
      head = head->next;
      delete deleteThis;
      return;
    }
    else
    {
      Node *p = head;
      while(p != nullptr)
      {
        if(p->next != nullptr &&
           p->next->value == v)
          break;
        p = p->next;
      }
      if(p != nullptr)
      {
        Node *deleteThis = p->next;
        p->next = deleteThis->next;
        delete deleteThis;
      }
    }
}
```

Finding Items:
```
bool LinkedList::findItem(string v)
{
  Node *p = head;
  while(p != nullptr)
  {
    if(p->value == v)
      return true;
    p = p->next;
  }
  return false;
}
```

Destructing the linked list:
```
LinkedList::~LinkedList()
{
  Node *p = head;
  while(p != nullptr)
  {
    Node *temp = p->next;
    delete p;
    p = temp;
  }
}
```

Downsides of linked lists:
-More complex than Arrays
-Searching is inefficient, accessing the kth item requires traversing k-1 items
