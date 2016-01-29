# Stacks

A stack is an ADT that holds a collection of items where elements are always
added to one end.

The last item pushed onto the top of a stack is the first item to be removed

Stack operations:
- Put something on top: ```push```
- Remove the top item: ```pop```
- look at top item
- check if stack is empty

This is called a Last-In-First-Out (LIFO) data structure.

Example of stack implementation:
```
const int SIZE = 100;

class Stack
{
public:
  Stack() { _top = 0 }
  void push(int val)
  {
    if(_top >= SIZE) return;
    _stack[_top] = val;
    if(_top == 0)
      empty = false;
    _top++;
  }
  int pop()
  {
    if(empty) return -1; //underflow NOTE: poor implementation
    top--;
    if(_top == 0)
      empty = true;
    return _stack[_top];
  }
  bool isEmpty()
  {
    return empty;
  }
  int peekTop()
  {
    if(!empty)
      return _stack[_top-1];
    else return -1; //NOTE: poor implementation
  }
private:
  bool empty;
  int _stack[SIZE];
  int _top;
}
```
Note: top does not identify the last item, it indicates the location above it.
Top is the number of items in the stack

C++ contains a stack implementation in the Standard Template Library (STL).
A stack can be of any type, including structs or classes.

Using the STL stack implementation:
```
#include <iostream>
#include <stack>

using namespace std;

int main()
{
  stack<int> intStack; //stack of ints
  stack<string> stringStack //stack of strings

  cout << intStack.top();  //get top value
  intStack.pop();          //removes top value

  if(intStack.empty() == false)
    cout << intStack.size();
}
```

Uses for stacks:
- storing undo items in word processor
- evaluating mathematical expressions
- converting from infix to postfix
- solving mazes
- etc.

Every CPU has a built in stack to hold local variables and function parameters.

**Postfix Expression Evaluation**:
- 15 + 6 -> 15 6 +
- 9 - 4  ->  9 4 -
- (14 + 6) * 5 -> 15 6 + 5 *
- 7 * 6 + 5 -> 7 6 * 5 +
- 3 + (4 + 5) -> 3 4 5 * +

postfix notation is unambiguous, unlike infix

Input: postfix expression

Output: answer

Private Data: a stack
Steps:
- Start at leftmost token
- If token is a number, push it onto the stack
- Else if the token is an operator:
  - Pop top value into variable v2, second-to-top item into variable v1
  - Apply operator to v1 and v2 (e.g. v1/v2)
  - Push result to the stack
- If there are more tokens, advance to next token and go back to step 2
- After all tokens have been processed, top item of stack is the answer

**Solving Mazes with a Stack and Depth-First-Search:**

Input: 10x10 maze in 2D array

Output: true if solvable, false if not

Private Data: a queue

Steps:
- Push starting point onto stack
- Mark starting point as discovered
- if stack is empty, there is no solution
- Pop the top of stack
- If at endpoint, done
- Otherwise if slot to west is open:
  - Mark (curx-1, cury) as discovered
  - Push (curx-1, cury) on stack
- Otherwise if slot to east is open:
  - Mark (curx+1, cury) as discovered
  - Push (curx+1, cury) on stack
- Otherwise if slot to north is open:
  - Mark (curx, cury-1) as discovered
  - Push (curx, cury-1) on stack
- Otherwise if slot to south is open:
  - Mark (curx, cury+1) as discovered
  - Push (curx, cury+1) on stack
- GOTO step 3

---

# Queues

A queue is a First-In-First-Out (FIFO) data structure. Items are enqueued at the
rear and dequeued at the front.

Queue interface:
```
enqueue(int a);
int dequeue();
...
FINISH THIS LATER
...
```

Common Uses for Queues:
  - Storing data from the internet
  - Searching through mazes (breadth-first)

**Solving a Maze with a Queue and Breadth-First-Search:**

Input: 10x10 maze in 2D array
Output: true if solvable, false if not
Steps:
- Insert starting point at onto queue
- Mark starting point as discovered
- If queue is empty, no solution, done
- Remove top point from queue
- If point to west is open and undiscovered
  - Mark (curx-1, cury) as discovered
  - Insert (curx-1, cury) on queue
- If point to east is open and undiscovered
  - Mark (curx+1, cury) as discovered
  - Insert (curx+1, cury) on queue
- If point to north is open and undiscovered
  - Mark (curx, cury-1) as discovered
  - Insert (curx, cury-1) on queue
- If point to south is open and undiscovered
  - Mark (curx, cury+1) as discovered
  - Insert (curx, cury+1) on queue
- GOTO step 3

Queue Implementations:

Circular Queues:

A circular queue is an array-based queue. Items do not need to be shifted with
a circular queue.

```
private data:
  an array: array-based
  an integer: head
  an integer: tail
  an integer: count

To initialize the queue:
head = tail = count = 0

to enqueue a new item, place it in arr[tail], increment tail and count

to dequeue an item fetch head[arr], decrement head, count

if head or tail go past the end of the array, set them to 0
```

STL Queue Implementation:
```
#include <iostream>
#include <queue>
```
