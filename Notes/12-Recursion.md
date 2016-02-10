# Recursion

Recursion is the use of a function that calls itself in order to implement an
algorithm.

Merge sort is an example of a recursive algorithm.

Merge Sort:
```
void MergeSort(array)
{
  if (array's size == 1)
    return;

  MergeSort(first half of array);
  MergeSort(second half of array);

  Merge(two array halves);
}
```

Rules of Recursion:

1. Every recursive function must have a stopping condition, or _base case_ that
   is solvable without recursion.

2. Every recursive function must have a simplifying step, which breaks the
   into smaller sub-problems, so that the stopping condition is eventually reached.
   Most recursive algorithms divide the input in half or reduce the input by 1.

3. Recursive functions should not use global, static or member variables. They should
   only use local variables and parameters.

### Examples of Recursive Functions:

Factorial Using Recursion:
```
int factorial(int n)
{
  if(n == 0)
    return 1;
  return (n*factorial(n-1));
}
```

Summing Elements of an Array using Recursion:
```
int sumArr(int *arr, int n)
{
  if(n == 0) return 0;
  if(n == 1) return *arr;

  return (sumArr(arr, n/2) + sumArr(arr+n/2, n-n/2));
}
```

Printing an Array in Reverse:
```
void reversePrint(string *arr, int size)
{
  if(size == 0)
    return;
  else
  {
    reversePrint(arr + 1, size - 1);
    cout << *arr << "\n";
  }
}
```

Finding the Largest Item in a Non-Empty Linked List:
```
struct Node
{
  int val;
  Node* next;
}

int biggest(Node *current)
{
  if(!(current->next))
    return current->val;
  int thisValue = current->val;
  int other = biggest(current->next);
  return (thisValue > other ? thisValue : other);
}
```
When using recursion on a linked list with n items, the function should call
itself on the remaining n-1 elements in the linked list.

A recursive function on an array or linked list should generally only access
the current node or element. Generally the base case should refer to the first
or last item.

### Useful Recursive Functions

Binary Search Using Recursion:
```
//helper function
int BS(string A[], int top, int bot, string f)
{
  if (top > bot)
    return (-1); // Value not found
  else
  {
    int Mid = (top + bot) / 2;
    if (f == A[Mid])
      return(Mid); // found – return where!
    else if (f < A[Mid])
      return(BS(A,top,Mid - 1,f));
    else if (f > A[Mid])
      return(BS(A, Mid + 1,bot,f));
  }
}

//user calls this
int SimpleBinarySearch(string A[], int size, string findMe)
{
  return BS(A , 0 , size-1 , findMe);
}
```

Maze Solving using Backtracking:
```
//global variables are still bad, don't do this
bool solvable = false;
char m[11][11] =
{
//maze here  
}

void solve(int sx, int sy)
{
  m[sy][sx] = ‘#’;
  // drop crumb
  if (sx == dx && sy == dy)
    solveable = true;
    // done!
  if (m[sy-1][sx] == ' ')
    solve(sx,sy-1);
  if (m[sy+1][sx] == ' ')
    solve(sx,sy+1);
  if (m[sy][sx-1] == ' ')
    solve(sx-1,sy);
  if (m[sy][sx+1] == ' ')
    solve(sx+1,sy);
}
```

Tic-Tac-Toe Player:
```
bool gameIsOver(board) {
  if (X has three in a row) // X wins
    return true;
  if (O has three in a row) // O wins
    return true;
  if (all squares are filled) // tie game
    return true;
  return false;
}

//Co-recursive functions
FindBestOMove(board)
{
for each legal move j the computer can make
  duplicate the current board and apply move j
  if (theComputerJustWon() == true)
    outcome[j] = O_wins;
  else if (itsATieGame() == true)
    outcome[j] = Tie_game;
  else // not sure yet what the result will be…
    outcome[j] = FindBestXMove(board);
    return the best {outcome,move} for O
}

FindBestXMove(board)
{
for each legal move j the computer can make
  duplicate the current board and apply move j
  if (theComputerJustWon() == true)
    outcome[j] = O_wins;
  else if (itsATieGame() == true)
    outcome[j] = Tie_game;
  else // not sure yet what the result will be…
    outcome[j] = FindBestOMove(board);
    return the best {outcome,move} for O
}

GameBoard b;
while (!gameIsOver(b))
{
  move = GetHumanMove(b);
  b.applyMove(move);
  move = FindBestOMove(b);
  b.applyMove(move);
}
```
