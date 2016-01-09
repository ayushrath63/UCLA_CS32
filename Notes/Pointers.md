# Pointers
---

## Intro to pointers

Every variable has an address in memory.
When a variable is defined, an unused address is reserved for it.

Some variables require more memory than others.

The address of a variable is defined to be the lowest address in memory where
the variable is stored.The & operator retrieves the address of the variable it
 operates on.

A pointer variable is a variable that stores the address of another variable.
Pointers are declared using * after the type of the object being pointed to.
The type of the object pointed to must be declared so that the compiler can
determine what operations are valid.


Example:

```
int foo()
{
  int chickens = 5;     //suppose chickens is stored at address 1000
  int* p = &chickens    //p holds a value of 1000
}
```
The compiler tracks whether variables are pointers are not. All pointer
variables are the same size, the size depends on the amount of memory of the
machine.

Pointer variables are stored at an address, like any other variable.

---

## What are pointers used for?

The * operator allows reading or writing to the variable pointed to by a
pointed variable.

Example:

```
int main()
{
  int var = 1234;
  int *ptr = &var;

  cout << *ptr; //prints 1234

  *ptr = 5;     //stores a value of 5 in var
}
```

Using * to access variables is called indirection.

Passing by reference is simpler notation for passing by pointers.

```
void set(&val) //val is a really pointer
{
  val = 5;
}

int main()
{
  int x = 1;

  set (x)     //passes address, not variable
}
```

A pointer must point to something before it can be used. If a pointer is not
initialized to a variable's address, it is recommended to set it to the null
pointer (NULL or nullptr).

**Challenge:**

Create a function swap() that accepts two pointers to integers and swaps two
values pointed to by pointers.

```
void swap(int *pa, int *pb)
{
  int temp;
  temp = *pa;
  *pa = *pb;
  *pb = temp;
}

int main()
{
  int a = 5, b = 6;

  swap (&a, &b)
  cout << a;
  cout << b
}
```

---
## Arrays, Addresses and Pointers

Like any other variable, every array has an address in memory that can be
accessed with the & operator. _But_, in C++, the & operator is not needed to
access the address, the name of the array (without brackets) can be used to
access the address of the start of the array.

Example:
```
int main()
{
  int nums[3] = {10,20,30};
  cout << nums;     //prints &nums
  int *ptr = nums;   //points to array
}
```

In the example above, nums is _not_ a pointer, it is replaced by the address
of the array during compilation, as such, it is immutable.

A pointer to an array can be used with brackets as if it were the array itself.

Example:
```
int main()
{
  int nums[3] = {10,20,30};
  int *ptr = nums;
  cout << ptr[2]; //prints 30
}
```

The * operator can also be used to access the arrays contents. The following
code is equivalent the the previous example.
```
int main()
{
  int nums[3] = {10,20,30};
  int *ptr = nums;
  cout << *(ptr+2); //prints 30
}
```

The following statements are all equivalent:
```
ptr[2] <==> *(ptr+2) <==> *(2+ptr) <==> 2[ptr]
```

When passing an array to a function, what is really being passed is the address
of the array.
```
void(int array[])
{
  cout << array[0];
}
```
is equivalent to:
```
  void(int *array)
  {
    cout << array[0];
  }
```

In order to pass an array to a function by value, you must contain the array
in a struct and pass the struct by value.
