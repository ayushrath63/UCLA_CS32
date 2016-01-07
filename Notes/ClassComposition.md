# Class Composition:

  Class composition is when a class contains one or more member variables
  that are objects.

  Example:
```
    class GasTank
    {
    ...
    };

    class Car
    {
    ...
    private:
      GasTank myTank;
      int horsepower;
      string modelName;
    }
```
  Embedded member variables must be constructed before the outer object's
  constructor can be called. The compiler automatically adds code to call
  the default constructors for the member variables.

  Destruction occurs in the reverse order of construction (i.e. bottom-up).

  C++ automatically adds constructors and destructors in order for member
  variables to be properly constructed and destructed implicitly.

  If an embedded member variable requires a parameter for construction, the
  constructor for it must explicitly be called using an initializer list.

  Example:
```
    class Stomach
    {
    public:
      Stomach(int startGas)
        {
        myGas = startGas;
        }
      ~Stomach() { cout << “Fart!\n”; }
      void eat() { myGas ++; }
    private:
      int myGas;
    };

    class HungryNerd
    {
    public:
      HungryNerd()
        :myBelly(10) //initializer list --- !!!
      {
        myBelly.eat();
        myBrain.think();
      }
    private:
      Stomach myBelly;
      Brain myBrain;
    };
```
  Any time you have a member variable (e.g., myBelly) that requires a parameter
  for construction you must add an initializer list to all of your outer class's
  constructors.

  Items in an initializer list are separated by commas and must be in order of
  construction, no semicolon at the end.

  Initializer list must be added to the constructor definition, it cannot be
  added to a prototype.

  The initializer list can also be used to declare scalar member variables.
M
