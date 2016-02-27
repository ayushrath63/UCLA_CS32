# Generic Programming

Generic programming is building algorithms that operate on multiple types of data

### Part 1: Allowing Generic Comparisons

Consider the following object comparisons
```
main()
{
  int i1 = 3, i2 = 5;
  if (i1 > i2)
    cout << “i1 is bigger”;

  Circ a(5), b(6);
  if (a.radius() > b.radius())
    cout << “a was bigger”;

  Dog fido(10), spot(20);
  if (fido.weight() > spot.weight())
    cout << “fido is bigger”;
}
```

Note that ```Circ``` objects are compared by radius & ```Dog```objects by weight

Comparison operators (similar to assignment operators) can be created to do this
instead:
```
bool operator>=(const Dog &a, const Dog &b)
{
  if (a.getWeight() >= b.getWeight())
    return true;
  return false;
}
```
_All_ necessary comparison operators must be defined, defining ```operator!=```
does not generate ```operator==```, etc.

### Part 2: Writing Generic Functions

Consider the following swap functions:
```
void SwapCircle(Circ & a, Circ & b)
{
  Circle temp;
  temp = a;
  a = b;
  b = temp;
}

void SwapDog(Dog & d1, Dog & d2)
{
  Dog temp;
  temp = a;
  a = b;
  b = temp;
}

main()
{
  Circle a(5), b(6);
  Dog c(100), d(750);
  SwapCircle(a, b);
  SwapDog(c, d);
}
```

These can be replaced by a generic swap function:
```
template <typename Item>
void swap(Item &a, Item &b)
{
  Item temp;
  temp = a;
  a = b;
  b = temp;
}

main() {
  Circle a(5), b(6);
  Dog c(100), d(750);
  swap(a, b);
  swap(c, d);
}
```
alternately, ```template <class Item>``` can be used instead of ```template <template Item>```.

_Always_ put templated functions in a header file.

A template data type must be used to define the type of at least one parameter,
otherwise an error will occur.

Suppose you have the following  function:
```
template <typename Data>
void winner(Data &x, Data &y)
{
  if (x > y)
    cout << “first one wins!\n”; else cout << “second one wins!\n”;
}
```
A comparison operator must be defined for every type that the templated function
is used for, otherwise a compilation error will occur.

### Part 3: Writing Generic Classes

Consider the following templated class:
```
template <template Item>
class HoldOneValue
{
public:
  void setVal(Item a)
  {
    m_a = a;
  }

  void print()
  {
    cout << a << endl;
  }
private:
  Item a;
};
```
To use this class with multiple types include the type between ```<``` and ```>```
Similar to the syntax for STL stacks and queues:
```
main()
{
  HoldOneValue<int> v1;
  v1.setVal(10);
  v1.print();

  HoldOneValue<string> v2;
  v2.setVal("ouch");
  v2.print();
}
```

With externally defined member functions:
```
template <template Item>
void ClassName<Item>::FunctionName1()
{
  //function stuff
}

template <template Item>
void ClassName<Item>::FunctionName2(Item a)
{
  //function stuff
}
```

Inline Methods: function logic is directly embedded when a function is called
(for speed). By default, all methods defined inside a class are inline. Inline
methods will increase program size. To make an externally defined function inline,
use the keyword inline before the function return type.

Example:
```
template <template Item>
inline void ClassName<Item>::FunctionName(Item a)
{
  //function stuff
}
```

Template classes are useful when building container classes, such as linked lists.

1. A derived class that must initialize member variables of the base class must use the initializer list
2. A derived function should not access private variables from the base class.

### Part 4: The Standard Template Library (STL)

The STL is a collection of pre-written classes that are built using templates.
The classes ```stack ```and ```queue``` are examples of STL classes. They are
called container classes, since they store things.


##### Vector

The class ```vector``` is a useful STL class that works similarly to an array,
but has a variable size and has all elements initialized to 0. Resizing a ```vector```
takes significant computational resources.

Example:
```
#include <vector>
using namespace std;
int main()
{
  vector<string> strs; //vector of strings, size 0
  vector<int> vals(3, 55); //vector of ints, size 3, each element is 55
}
```

Elements of a ```vector``` can be accessed using ```[]```, like  with arrays.
To add a new item to the end of a vector, use the ```push_back()``` method.

Example:
```
#include <vector>
using namespace std;
int main()
{
  vector<string> strs; //vector of strings, size 0
  strs.push_back("string1");

  vector<int> vals(3); //vector of ints, size 3
  vals[0] = 42
}
```

Accessing elements outside the ```vector``` can cause crashes.

The ```front()``` and ```back()``` methods can be used to access the first and
last elements of a ```vector``` respectively. The ```size()``` method can be used
to return the size of a vector.

To remove the last element of a ```vector``` use the ```pop_back()``` method.
An item that has been removed can no longer be accessed.

##### List

The ```list``` class is a doubly-linked list.
It has ```push_back()```, ```back()```, ```push_front()``` and ```front()```
methods.

Example:
```
#include <list>
using namespace std;

int main()
{
  list<float> lf;
  lf.push_back(1.2);
  lf.push_back(4.2);
  lf.push_front(3.3);
}
```
##### Iterators
To iterate through STL containers, an _iterator variable_ is used an
iterator is similar to a pointer, but is used only with STL containers. Iterators
can be incremented or decremented like a pointer. The ```begin()``` method for ```list```
and ```vector``` returns and iterator to the first element. the ```*``` operator
can be used to dereference an iterator. ```vector``` iterators can have arithmetic
done, but ```list``` iterators cannot.

Example:
```
#include <list>
using namespace std;

int main()
{
  list<float> lf;
  lf.push_back(1.2);
  lf.push_back(4.2);
  lf.push_front(3.3);

  list<float>::iterator it; //iterator that can point to vectors of ints
  it = lf.begin();
  cout << *it; //prints 3.3

  it++;
  cout << *it; //prints 4.2
}
```

The ```end()``` method returns an iterator to the location past the last item.

STL containers can also be used with classes or structs. To access the items of
such a container using iterators, use the ```*``` operator, then the ```.``` operator
or the ```->``` operator.

When a cons reference to a container is passed as a parameter to a function, a
regular iterator cannot be used. A ```const_iterator``` must be used, which cannot
be used to modify the contents of the container.

Example:
```
void tickleNerds( const list<Nerd> &nerds)
{
  list<string>::const_iterator it; // works!!!
  for (it=nerds.begin(); it != nerds.end(); it++)
    cout << *it << “ says teehee!\n”;
}
int main()
{
  list<string> nerds;
  nerds.push_back(“Carey”);
  nerds.push_back(“David”);
  …
  tickleNerds(nerds);
}
```

An iterator is an object that stores:
 - What it points to
 - How to find the next element
 - How to find the previous element

###### Map
The STL container ```map``` allows two related values to be associated. To add a
pair of items to a map, use the syntax ```mapName["key"] = value;```;

Example:
```
#include <map>
#include <string>
using namespace std;

int main()
{
  map<string, int> name2phone;
  name2phone["Carey"] = 123456789; //maps "Carey" -> 123456789
}
```

A ```map``` can only associate in one direction, in the above example, int -> string
would not be possible. To efficiently search in both directions, two ```map```
containers would be needed. Only one of each key can be stored in a ```map```.
A ```map``` can be searched by key using the ```find()``` function. To map more
complicated classes or structs, the ```operator<``` method must be defined for the
class or struct.

##### Set

A ```set``` is a container that contains unique items. If you attempt to insert
a duplicate item it is ignored. A set is ordered automatically, but an operator<
must be defined.

Example:
```
#include <set>
#include <iostream>

using namespace std;

int main()
{
  set<int> a;
  a.insert(1);
  a.insert(5);
  a.insert(66);

  set<int>::iterator it;
  it = a.find(5);
  if(it == a.end())
  {
    cout << "5 not found" << endl;
    return 0;
  }
  cout << *it << "found" << endl;
}
```

Most STL containers have an ```erase()``` method that can be used to delete an
item using an iterator. The iterator will be invalid after ```erase()``` is called.

Important Note: After an item is added or removed from a ```vector```, all iterators
pointing to items are invalid. This is not true for a ```list```, ```set``` or ```map```,
unless the item pointed to by the iterator is deleted.

### Part 5: STL algorithms

The STL ```<algorithm>``` library provides template functions that cam be used
with many different data types.

The STL ```find()``` function can be used to find items in most STL containers
and arrays. ```find()``` takes 3 parameters, an iterator to the beginning, an
iterator to after the end, and the value to search for. When used with an array,
pointers are used instead of iterators. ```find()``` returns the first match using
a linear search.

The ```find_if()``` function is similar to ```find()``` but takes a pointer to a
predicate function that takes a single parameter as the third argument and returns
a pointer to the first item that causes the predicate to return true, or the
pointer to the location after the last element.

The ```sort()``` function can be used to sort an array or ```vector```. It takes
pointers or iterators to the beginning and after the end of the sorting area as
arguments. The ```operator<``` or another custom comparison function must be
defined in order to use ```sort()```.

### Part 6: Compound STL Data Structures

Suppose you want to maintain a list of courses for each UCLA student.
One method is to map student names to a ```list``` of courses.

Example:
```
#inlcude <map>
#inlcude <list>

class Course
{
  ...
};

int main()
{
  map<string,list<Course> > crsmap;

  Course c1("cs", "32"),
         c2("math", "33A"),
         c3("english", "1");

  crsmap["carey"].pushback(c1);
  crsmap["carey"].pushback(c3);
  crsmap["david"].pushback(c1);
  crsmap["david"].pushback(c2);
}
```

**Challenge - Design a compound STL data structure to map a Person object to a
set of their friends**
```
#include <set>
using namespace std;

class Person
{
public:
  string getName()
  string getPhone()
...
};

bool Person::operator<(const Person &a, const Person &b)
{
  return a.getName() < b.getName();
}

int main()
{
  map<Person,set<Person> > friendster;
}
```

**Challenge - Design a compound STL data structure to associate a person (string)
with the courses they've taken and further associate each course with the grade
recieved**

```
#include <map>

class Person
{...};

bool Person::operator<(const Person &a, const Person &b)
{...}

class Course
{...};

bool Course::operator<(const Course &a, const Course &b)
{...}

map<Person,map<Course, string> > x;
```
