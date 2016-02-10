# Polymorphism

Consider a function that accepts a reference to a ````Person```` object as a
reference.

```
void LemonadeStand(Person &p)
{
  cout << "Hello" << p.getName();
  cout << "How many cups of ";
  cout << "of lemonad do you want?";
}
```

Now suppose ```class Student``` derived from person. A Student can still be passed
as a parameter to ```LemonadeStand()```.

The derived class object is treated as a superclass object by the function, so
data members of the superclass portion are available.

Polymorphism is the use of a base pointer or reference to access a derived
object.

A pointer or reference must be used otherwise _chopping_ occurs. If a derived
object is passed to a function that accepts a superclass object, only the
superclass portion of the derived object will be copied. This is legal, but
considered bad practice.

Now consider a class ``` Shape ```.
```
class Shape
{
public:
  virtual double getArea()
  { return (0); }
  ...
private:
  ...
};
```
And two derived classes:
```
class Square: public Shape
{
public:
  Square(int side){ m_side=side; }
  virtual double getArea()
  { return (m_side*m_side); }
private:
  int m_side;
};
```
```
class Circle: public Shape
{
public:
  Circle(int rad){ m_rad = rad; }
  virtual double getArea()
  { return (3.14*m_rad*m_rad); }
private:
  int m_rad;
};
```
Each derived class has it's own constructor and ```getArea()``` function that
overrides the base one.

Here is one way to print the price per unit area of squares and circles:
```
void PrintPriceSq(Square &x)
{
  cout << “Cost is: $“;
  cout << x.getArea() * 3.25;
}

void PrintPriceCir(Circle &x)
{
  cout << “Cost is: $“;
  cout << x.getArea() * 3.25;
}

main()
{
  Square s(5);
  Circle c(10);
  PrintPriceSq(s);
  PrintPriceCir(c);
}
```

This suffers from code reuse. It can be replaced by this:
```
void PrintPriceCir(Shape &x)
{
  cout << “Cost is: $“;
  cout << x.getArea() * 3.25;
}

main()
{
  Square s(5);
  Circle c(10);
  PrintPriceSq(s);
  PrintPriceCir(c);
}
```
Since ```getArea()``` is a ```virtual``` function, the most derived version is
called, i.e. it will call the correct one for a ```Square``` or ```Circle```.

**The difference between inheritance and polymorphism:**

Inheritance: We publicly derive one or more classes ```D1-Dn``` (e.g. ```Square```, ```Circle``` , ```Triangle```)
from a common base class (e.g. ```Shape```). All of the derived classes,
by definition, inherit a common set of functions from our base class: e.g., getArea(),
getCircumference() Each derived class may re-define any function originally defined
in the base class; the derived class will then have its own specialized version
of those function(s).

Polymorphism: Use of a Base pointer/reference to access any variable that is of a
type that is derived from our Base class. The same function call automatically
causes different actions to occur, depending on what type of variable is currently
being referred/pointed to.

Polymorphism can make systems more easily extensible.

Always have a virtual destructor in the base class when using inheritance.

Polymorphism can also be used with pointers:
```
int main()
{
  ...
  Square s(5);
  Shape *p;

  p = &s
  ...
}
```
In general a superclassed pointer can be pointed at a subclass variable.

upcast: accessing a derived variable using a base pointer or reference

downcast: converting a base class pointer to a derived class pointer

An array of base class items cannot hold a derived variable and vice versa, but
an array of base class pointers can contain pointers to any derived class items.

Example:
```
main()
{
  Circle c(1);
  Square s(2);
  Triangle t(4,5,6);
  Shape *arr[100];
  arr[0] = &c;
  arr[1] = &s;
  arr[2] = &t;
  // redraw all shapes
  for (int i=0;i<3;i++)
  {
    arr[i]->plotShape();
  }
}
```

A derived class pointer cannot point to a base class item.

following an upcast pointer to call a virtual function will call the most derived
version of the function.

A virtual destructor is needed to delete an upcast pointer.

Virtual destructor example:
```
class Prof
{
public:
  Prof() { m_myIQ = 95; }
  virtual ~Prof()
  {
    cout << “I died smart:”;
    cout << m_myIQ;
  }
private:
  int m_myIQ;
};
```
```
class MathProf: public Prof
{
public:
  MathProf(void)
  {
    m_pTable = new int[6];
    for (int i=0;i<6;i++)
      m_pTable[i] = i*i;
  }
  virtual ~MathProf()
  {
    delete [] m_pTable;
  }
private:
  int *m_pTable;
};
```
```
main()
{
  Prof *p;
  p = new MathProf;
  ...
  delete p;
}
```

Because the destructor is virtual, the ```MathProf``` destructor is called first,
then the ```Prof``` destructor second. If the destructor was not virtual,
undefined behavior would occur.

When a class variable is defined an invisible table, called a _vtable_, is added
to the object that points to the proper set of virtual functions to use.
Entries in the _vtable_ point to the most derived version of each virtual function
The _vtable_ is used during runtime and reduces performance.

A pure virtual function is a virtual function whose base implementation does nothing
It can be indicated by setting a virtual function to 0:
```
class Shape
{
public:
  virtual double getArea() = 0;
  virtual void someOtherFunc()
  {
    cout << “blah blah blah\n”;
    ...
  }
  ...
private:
};
```

A class that contains at least one pure virtual function is called an _abstract
base class_.

Classes derived from abstract base classes (ABCs) must implement all pure virtual
functions or become ABCs themselves.

A object of an ABC type cannot be created, but a pointer to an ABC object can.
