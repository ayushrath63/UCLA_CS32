# Classes and the "this" Pointer

Example of a program without classes (C style):
```
struct Wallet
{
  int num1s, num5s;
};

void Init(Wallet *ptr)
{
  ptr->num1s = 0;
  ptr->num5s = 0;
}

void AddBill(Wallet *ptr, int amt)
{
  if (amt == 1) ptr->num1s++; else if (amt == 5) ptr->num5s++;
}

void main(void)
{
  Wallet w;
  Init(&w);
  AddBill(&w , 5);
}
```

C++ classes function in an almost identical fashion.

Equivalent in C++:
```
class Wallet
{
public:
  void Init();
  void AddBill(int amt);
…
private:
  int num1s, num5s;
};

void Wallet::Init()
{
  num1s = num5s = 0;
}

void Wallet::AddBill(int amt)
{
  if (amt == 1) num1s++;
  else if (amt == 5)num5s++;
}

int main()
{
  Wallet a;
  a.Init();
  a.AddBill(5);
}
```

Every time a member function of a class is called, the compiler rewrites the
function call with the address of the class instance as a parameter, called this.

So
```
void Wallet::Init()
{
  num1s = num5s = 0;
}

void Wallet::AddBill(int amt)
{
  if (amt == 1) num1s++;
  else if (amt == 5)num5s++;
}
```
is interpreted as implicitly
```
void Init(Wallet *this)
{
  this->num1s = this->num5s = 0;
}

void AddBill(Wallet *this, int amt)
{
  if (amt == 1) this->num1s++;
  else if (amt==5) this->num5s++;
}
```

The this pointer can be used to access a member variable with the same name as
a member variable.


The 'this' pointer can be called explicitly in methods.

Example:

```
void Wallet::Init()
{
  num1s = num5s = 0;
  cout << “I am at address: “ << this;
}
```
