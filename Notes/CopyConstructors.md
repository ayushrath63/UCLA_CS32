# Copy Constructors

---

Suppose we have a class Circle:
```
class Circ
{
public:
  Circ(int x, int y, int r)
  {
    m_x = x;
    m_y = y;
    m_rad = r;
  }
  float GetArea(void) const;
private:
  float m_x, m_y, m_rad;
};
```
An additional constructor can be created that allows us to initialize a new
Circle variable based on the value of an existing class instance:
```
Circ::Circ(const Circ &old)
{
  m_x = old.m_x;
  m_y = old.m_y;
  m_rad = old.m_rad;
}

int main()
{
  Circ a(1,2,3);
  Circ b(a);
...
}
```
Note: Circle b can operate on Circle a's private members since they are of the
same class, privates in C++ can be accessed by all members of the same class.

This is called a copy constructor. A copy constructor is used to initialize a
new instance from an existing one of the same type. The parameter of the copy
constructor should be const, but does not have to be. The instance being passed
to the copy constructor must be passed by reference, or the copy constructor
will be continuously called on the parameter.

The syntax ``` Circle b(a);``` is equivalent to ``` Circle b = a;```, it
defines b and calls the copy constructor.

C++ will create a default copy constructor if there is not one defined.

Why would a non-default copy constructor be needed? Consider the class Squares:
```
class Squares
{
public:
  Squares(int n) {
    m_n = n;
    for (int j=0;j<n;j++)
      m_sq[j] = (j+1)*(j+1);
  }
  ~Squares() { }
  void printSquares()
  {
    for (int j=0;j<m_n;j++)
      cout << m_sq[j] << endl;
  }
private:
  int m_sq[5], m_n;
};

int main()
{
  Squares a(3);
  if (...)
  {
    Squares b = a;
  ...
  }
  a.printSquares();
}
```

This creates shallow copies of a, copies with the same values.

Now consider an updated version of Squares:

```
class Squares
{
public:
  Squares(int n) {
  m_n = n;
  m_sq = new int[n];
  for (int j=0;j<n;j++)
    m_sq[j] = (j+1)*(j+1); }

  ~ Squares()
  {
    delete []m_sq;
  }

  void printSquares()
  {
  for (int j=0;j<m_n;j++)
    cout << m_sq[j] << endl;
  }

private:
  int *m_sq, m_n;
};

int main()
{
  Squares a(3);
  if (...)
  {
    Squares b = a; //creates a shallow copy of a
    ...
  }
  a.printSquares();
}
```

If the default copy constructor called, b and a will refer to the same memory,
which is problematic, if b is destructed, the memory pointed to by a will be
freed as well.

This can be fixed by writing a custom copy constructor:
```
Squares::Squares(const Squares &src)
{
  m_n = src.m_n;            //first determine how much memory is needed for the    
                            //new instance
  m_sq = new int[m_n];      //allocate a new copy of any dynamic memory
  for (int j=0;j<m_n;j++)   //copy the contents of dynamic memory
    m_sq[j] = src.m_sq[j];
}

int main()
{
  Squares a(3);
  if (...)
  {
    Squares b = a; //creates a shallow copy of a
    ...
  }
  a.printSquares();
}
```
This will produce a valid copy of a.
