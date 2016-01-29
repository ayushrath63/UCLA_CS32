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

Inheritance: We publicly derive one or more classes ```D1-Dn``` (e.g. ```Square```, ```Circle``` , ```Triangle```) from a common base class (e.g. ```Shape```). All of the derived classes,
by definition, inherit a common set of functions from our base class: e.g., getArea(),
getCircumference() Each derived class may re-define any function originally defined
in the base class; the derived class will then have its own specialized version
of those function(s).

Polymorphism: Use of a Base pointer/reference to access any variable that is of a
type that is derived from our Base class. The same function call automatically
causes different actions to occur, depending on what type of variable is currently
being referred/pointed to.
