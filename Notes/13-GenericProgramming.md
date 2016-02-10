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

A template data type must be used to define the type of ate least one parameter,
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
main()
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
