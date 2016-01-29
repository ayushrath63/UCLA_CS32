# Inheritance

Consider the following two classes:

```
class Robot
{
public:
  void setX(int newX);
  int getX();
  void setY(int newY);
  int getY();
private:
  int m_x, m_y;
};
```
```
class ShieldedRobot
{
public:
  void setX(int newX);
  int getX();
  void setY(int newY);
  int getY();
  int getShield();
  void setShield(int s);
private:
  int m_x, m_y, m_shield;
};
```

Robot and ShieldedRobot are very similar, with member functions and variables
with the same purpose.

Inheritance allows definition of a subclass (i.e. ShieldedRobot) using a
superclass (i.e Robot). Inheritance simplifies code by avoiding duplicate code.

Example:
```
class Robot
{
public:
  void setX(int newX);
  int getX();
  void setY(int newY);
  int getY();
private:
  int m_x, m_y;
};
```
```
class ShieldedRobot: public Robot
{
public:
  int getShield();
  void set setShield(int s);
private:
  int m_shield;
}
```

Subclass data is stored separately from the superclass data, after it in memory.
Derived classes cannot access private data members of the parent class and vice
versa. A subclass can be derived from multiple base classes.

Multiple Inheritance Example:
```
class Person
{
public:
  string getName(void);
...
private:
  string m_sName;
  int m_nAge;
};
```
```
class Student: public Person
{
public:
  // new stuff:
  int GetStudentID();
...
private:
  // new stuff:
  int m_studentID;
};
```
```
class CompSciStudent: public Student
{
public:
  // new stuff:
  void saySomethingSmart();
private:
  // new stuff:
  string m_smartIdea;
};
```

Reuse is the use of code from the base class in the derived class.

Subclasses can use protected functions/data from the superclass.

Example:
```
class Robot
{
public:
  Robot(void);
  int getX() const;
  …
protected: // methods
  void chargeBattery();
private: // data
  int m_x, m_y;
};
```
```
class ShieldedRobot : public Robot {
public:
  ShieldedRobot(void)
  {
    m_shield = 1;
    chargeBattery();
  }
  void setShield(int s);
...
private:
  int m_shield;
};
```
Protected members of the superclass are protected in the derived class.
Never make member variables protected, this breaks encapsulation.

Extension is the adding of new behaviors or data to a derived class.

Specialization is the redefinition of behavior from the base class in a derived
class.

The keyword virtual must be added before the original function in order for it
to be redefined. It follows that functions without the virtual keyword cannot be
redefined. Making a function virtual makes it virtual in derived classes. Making
a function virtual reduces execution speed, so it should not be used for functions
that don't get overridden.

Example:
```
class Student
{
public:
virtual void WhatDoISay()
  {
    cout << “Go bruins!”;
  }
...
};
```
```
class NerdyStudent: public Student
{
public:
 virtual void WhatDoISay()
  {
    cout << “I love circuits!”;
  }
...
};
```

The virtual keyword is only included with the function prototype, not with the
definition, if the definition is outside the class.

A redefined function cannot access private data from the superclass.

If a function is redefined in a derived class, the redefined function hides the
base version of the function. The most derived version of the function will be
used by default.

A derived class with a redefined method CAN call the method from the parent class
using ```baseclass::method()``` syntax.

A redefined virtual function can call the base version.

Example:
```
class Student
{
public:
  Student()
  {
    myFavorite = “alcohol”;
  }
virtual string whatILike()
  {
    return myFavorite;
  }
private:
  string myFavorite;
};
```
```
class NerdyStudent: public Student
{
public:
  virtual string whatILike()
  {
    string fav = Student::whatILike();
    fav += “ bunsen burners”;
    return fav;
  }
};
```

Inheritance and Construction:

When a derived object is defined, it has both superclass and subclass parts.
The base class part is constructed first, the subclass part afterwards.

Inheritance and Destruction:

The derived object part is destructed before the superclass object part.

Inheritance and Initializer Lists:

If a superclass does not have a constructor without parameters, a constructor
with the appropriate amount of parameters must be added to the initializer list.
Adding a parameter to the subclass's constructor allows it to be used in the
constructor of the superclass object in the initializer list.

Inheritance and Assignment Operators:

A shallow copy will be created if an assignment operator for the derived class
is not defined.

Example of assignment operator for derived class with dynamic memory:
```
class Person
{
public:
  Person() { myBook = new Book; } // I allocate memory!!!
  Person(const Person &other);
  Person& operator=(const Person &other);
…
private:
  Book *myBook;
};
class Student: public Person
{
public:
  Student(const Student &other) : Person(other)
  {
    … // make a copy of other’s linked list of classes…
  }
  Student& operator=(const Student &other)
  {
      if (this == &other) return *this;
      Person::operator=(other);
      … // free my classes and then allocate room for other’s list of classes
      return(*this);
  }
private:
  LinkedList *myClasses;
};
```
