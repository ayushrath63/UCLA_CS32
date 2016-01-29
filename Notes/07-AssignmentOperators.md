# Assignment Operators

The assignment operator constructs a new variable using the value of an existing
variable.

First - how to change to value of an existing variable to the value of another
variable.

```
int main()
{
  Circle foo(1,2,3);
  Circle bar(4,5,6);
  bar = foo;
}
```
A special member function called an assignment operator is used to copy foo's
values into bar.

C++ provides a default assignment operator if one isn't defined.

Defining a simple function equivalent to assignment operator:
```
class Circle
{
public:
  Circle(int x, int y, int r)
  {
    m_x = x;
    m_y = y;
    m_rad = r;
  }

  void setMeEqualTo(const Circle &src)
  {
      m_x = src.m_x;
      m_y = src.m_y;
      m_rad = src.m_rad;
  }

  float GetArea(void) const;
private:
  float m_x, m_y, m_rad;
};
```

Defining an assignment operator:
```
class Circle
{
public:
  Circle(int x, int y, int r)
  {
    m_x = x;
    m_y = y;
    m_rad = r;
  }

  Circle &operator=(const Circle &src)
  {
      m_x = src.m_x;
      m_y = src.m_y;
      m_rad = src.m_rad;
      return *this;
  }

  float GetArea(void) const;
private:
  float m_x, m_y, m_rad;
};
```

The assignment operator is of type reference to the class type and returns \*this.
An assignment operator can be created for other classes as well. The assignment
operator acts on the target variable.

Other operators (e.g. operator+, operator>, operator-, etc.) can also be defined.

Consider the following:
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

int main()
{
  Squares a(3);
  Squares b(4);
  b = a;
}
```
When the default assignment operator is used, the array originally pointed to
by b is lost and a memory leak occurs. Also, when the destructor for a and b is
called respectively, a double free error will occur (calling delete on the same
memory twice).

In this case, a custom assignment operator is needed.
It must:
- Free any memory held by the target variable (b).
- determine how much memory is used by the source variable (a).
- Allocate memory in the target variable.
- Copy the contents of the source variable to the target variable.
- Return a reference to the target variable.

```
Squares Squares::&operator=(const Squares &src)
{
  delete [] m_sw;
  m_n = src.m_n;
  m_sq = new int[m_n];
  for(int j = 0; j<m_n; j++)
    m_sq[j] = src.m.src[j];
  return *this;
}
```

This assignment operator will correctly assign b to a.

The assignment operator returns \*this in order to facilitate multiple assignment,
i.e. ```a = b = c;``` is possible.

Copying or assignment of class instances can be prevented by making the copy
constructor and assignment operator private.

The assignment operator above still has issues, it fails for ```a = a;```.
To fix, check if the variable is being assigned to itself:
```
Squares Squares::&operator=(const Squares &src)
{
  if(&src == this)
    return *this;
  delete [] m_sw;
  m_n = src.m_n;
  m_sq = new int[m_n];
  for(int j = 0; j<m_n; j++)
    m_sq[j] = src.m.src[j];
  return *this;
}
```
