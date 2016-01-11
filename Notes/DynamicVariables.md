# Dynamic Variables

Sometimes you won't know how many variables you need until the program runs.
You can dynamically ask the operating system to reserve new memory for variables
and return a pointer to the memory. The memory must be freed after use.

The new command can be used to allocate an arbitrary amount of memory for an
array, and delete used to free it. Brackets are used when using new or delete
on an array. The pointer's type must be the same as the elements of the array.

Example:
```
int main(void)
{
  int *arr;
  int size;
  cin >> size;
  arr = new int[size];
  arr[0] = 10;
  arr[2] = 75;
  delete [] arr;
}
```

If new fails (i.e. not enough memory), the program will crash.

Example of new and delete within a class:
```
class MathProf
{
public:
  MathProf()
  {
    for (int j=0;j< 100 ;j++)
      m_arr[j] = j*j;
  }
  ~MathProf()
  {
    delete [] m_arr; // free memory
  }
  void printSquares()
  {
    for (int j=0;j< 100 ;j++)
      cout << m_arr[j];
  }
private:
  int *m_arr, m_n;
};

int main(void)
{
  MathProf stupidProf(5);
  MathProf smartProf(100);
  stupidProf.printSquares();
  smartProf.printSquares();
}
```

the new and delete operators can also be used to allocate non-array variables,
including classes and structs. The delete operator can safely be used on the
null pointer.

Example:
```
class Waldo
{
public:
  Waldo(int weight)
  {
    m_weight = weight; m_bacteria = 0;
  }
  ~Waldo()
  {
    m_weight = 0;
    m_bacteria = 1000000;
  }
private:
  int m_weight, m_bacteria;
}

int main(void)
{
  Waldo *ptr = new Waldo(165); //reserves memory, then calls constructor
  ...
  delete ptr;                  //calls destructor, then frees memory
}
```

When new is used to allocate a class instance, memory is allocated by the
operating system, then the constructor is called. When delete is used on
a class instance, the destructor is called, then the memory is released to the
OS.
