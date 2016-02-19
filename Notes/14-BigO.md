# Big-O

Some algorithms are faster than others. You usually want to use a faster algorithm.
Just measuring the algorithm's run time is not sufficient, since that varies with
computer and input.

A better way is to measure the number of computer instructions to solve the problem
with a given input size. Algorithms might be more or less efficient than others
depending on input size.

The number of instructions used by the algorithm as a function of input data size
can be used to compare function efficiency for all input sizes. This is the
idea behind ```Big-O```.

The ```Big-O``` approach measures the gross number of steps required to process
an input of size *N* in the worst case scenario. Coefficients and lower order
terms are ignored (i.e. 5N<sup>2</sup>+3N -> N<sup>2</sup>).
So, an O(n<sup.2</sup>) sorting algorithm requires roughly 1,000,000 operations
to sort n=1000 items.

An _operation_ can be any of the following:
1. Accessing an item in an array
2. Evaluating an expression
3. Traversing a single linked list node
4. etc.

Example:
```
int arr[n][n];
for ( int i = 0; i < n; i++ )
  for ( int j = 0; j < n; j++ )
    arr[i][j] = 0;
```
1. The variable ```i``` is initialized once.
2. There are ```n``` comparisons between ```i``` and ```n```
3. ```i``` is incremented ```n``` times
4. ```j``` is initialized ```n``` times
5. ```n```<sup>2</sup> comparisons between ```j``` and ```n```
6. ```j``` is incremented ```n```<sup>2</sup> times
7. ```arr[i][j]``` is set ```n```<sup>2</sup> times

This gives us f(n) = 3n<sup>2</sup> + 3n + 1 -> O(n<sup>2</sup>)

There is no need to look at every operation, we only need to look at frequently
occurring operations. for the previous example, there are 2 nested for loops, each
called ```n``` times, so the algorithm is O(n<sup>2</sup>)

Examples:
```
for ( int i = 0; i < n; i+=2 )
  sum++;
```
is O(n)

```
for ( int i = 0; i < q; i++ )
  for ( int j = 0; j < q; j++ )
    sum++;
```
is O(q<sup>2</sup>)

```
k = 2;
while (k>1)
{
  sum++;
  k /= 2;
}
```
is O(log<sub>2</sub>n)

```
for(int i = 0l i < n; i++)
  k = 2;
  while (k>1)
  {
    sum++;
    k /= 2;
  }
}
```
is O(nlog<sub>2</sub>n)

```
for ( int i = 0; i < q; i++ )
  for ( int j = 0; j < i; j++ )
    sum++;
```
is O(n<sup>2</sup>)

Method for Determining ```Big-O```:
1. Locate all loops that don't run a fixed number of times
2. Determine the maximum number of iterations for each loop
3. Turn each loop into a loop with a fixed number of iterations equal to the maximum
4. Do analysis

For algorithms with multiple inputs, each one must be considered.

### STL and Big-O

Algorithms associated with STL containers have Big-Os

The ```find()``` method for a ```set``` is O(log<sub>2</sub>n).

For a ```vector```, adding an item is O(1).

Deleting the first item from a ```vector``` is O(n).
