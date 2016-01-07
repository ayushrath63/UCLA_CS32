# Constructors:

  Every class should have an initialization function that can be used to reset
  new variables before they are used.

  Example:
```
    class CSNerd
    {
    public:
      void Init(int PCs, bool usesMacs)
      {
        m_numPCs = PCs; // # of PCs owned
        m_macUser = usesMac;
      }
      int getNerdScore(void)
      {
        if(m_macUser == true)
          return(0); //not nerdy;
        return(10 * m_numPCs);
      }
    private:
      int m_numPCs;
      bool m_macUser;
    }
```
  Problem - User has to call init

  A constructor is a member function that is called when an object is created
  and initializes all member variables, this solves the problem introduced in
  the program above:
```
    class CSNerd
    {
    public:
      CSNerd(int PCs, bool usesMacs)
      {
        m_numPCs = PCs; // # of PCs owned
        _macUser = usesMac;
      }
      int getNerdScore(void)
      {
        if(_macUser == true)
          return(0); //not nerdy;
        return(10 * m_numPCs);
      }
    private:
      int m_numPCs;
      bool _macUser;
    }
```
  The constructor will be called when a new CSNerd object is created, so the
  programmer does not have to call an init function.

  The constructor always has the same name as the class and there is no
  return type. To terminate the constructor with a return statement, you can use
  'return;'.

  If a constructor requires parameters, the parameters must be passed when an
  object is created. A constructor can have one or more default parameters (As
  can any C++ function). Default parameters must be right justified.

  Example:
```
  class CSNerd{
    //...
    //Good
    CSNerd(int PCs, bool usesMacs = true)
    {
      m_numPCs = PCs; // # of PCs owned
      _macUser = usesMac;
    }
    /*Bad
    CSNerd(int PCs = 5, bool usesMacs)
    {
      m_numPCs = PCs; // # of PCs owned
      _macUser = usesMac;
    }*/
    //...
  }

  int main() {
    CSNerd lyn(1,false);
    CSNerd ned(5); // OK!
    CSNerd dave; //invalid!
  }
```
 Constructors can be overloaded, the default constructor takes no parameters.

  Example:
```
    class CSNerd{
    ...
      CSNerd(int PCs, bool usesMacs = true)
      {
        m_numPCs = PCs; // # of PCs owned
        _macUser = usesMac;
      }

      CSNerd() {
        m_numPCs = 1;
        m_macUser = false;
      }
    ...
    }

    int main() {
      CSNerd lyn(1,false);
      CSNerd ned(5); //OK
      CSNerd dave; //OK
    }
```
  Note: Overloaded functions must not have the exact same parameter types

  If a constructor is not declared, C++ generates an implicit default
  constructor that does not initialize scalar member variables

  To have an array of objects of a class, the class must have a constructor that
  takes no parameters.

  When are constructors called?
```
    int main()
      {
        CSNerd carey(3,false), bill; //Constructor called
        CSNerd arr[52]; //Constructor called
        CSNerd *ptr = new CSNerd(1,3); //Constructor called
        CSNerd *justAPtr; //Constructor not called
      }
```
