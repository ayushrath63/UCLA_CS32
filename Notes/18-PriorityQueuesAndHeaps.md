# Priority Queues and Heaps

A _priority queue_  is a special type of queue that holds a _priority rating_
for each item, when dequeueing, the highest priority item is dequeued.

A priority queue supports 3 operations:
  - Insertion
  - Getting the value of the highest priority item
  - Removing the highest priority item

When defining a priority queue, you must specify how to determine the priority
of each item in the data structure.

The best way to implement a priority queue is using a data structure known as
a _heap_.

A heap is a special type of binary tree (not a BST).

A heap uses a _complete binary tree_, a tree with N levels  with the top
N-1 levels completely filled and the nodes in the bottom level arranged from
left to right.

Heaps are special complete binary trees. There are two types of heaps: _maxheaps_
and _minheaps_.

**Maxheaps:**
  - Value of a node is greater than or equal to the value of its children
  - Complete binary tree

**Minheaps:**
- Value of a node is less than or equal to the value of its children
- Complete binary tree

In a maxheap, the highest value item is located at the top. This is ideal for a
priority queue, since the highest priority item should be easily retrievable.

#### Maxheap Operations

To extract the largest item from a maxheap:
1. if the tree is empty, return error
2. Otherwise the top item is the largest value, store it
3. If the heap only has one node, delete it and return the saved value
4. Otherwise, copy the value from the bottom-most right-most node into the root
node.
5. Delete the bottom-most right-most node
6. Repeatedly swap the top value with the larger of its two children until the
value is greater than or equal to both of its children
7. Return the saved value.

Insertion:
1. If the tree is empty, create a new root node and return
2. otherwise, insert the new node at the bottom-most left-most position of the
tree (to maintain completeness)
3. Compare new value with the parent value
4. If the new value is larger, swap with parent
5. repeat 3-4 until the new value is no longer larger than its parent

Operations for a minheap are similar.

#### Heap Implementation

A heap cannot easily be implemented using a classical binary tree, since there is
no easy way to find a parent node.

An easy way to implement a heap is to use an array and copy values into nodes
a level at a time.

The root node can be initialized to ```heap[0]```, its children in the next two
slots, their children in the next 4, etc.

So ```{12,7,10,3,2,8,4,2}``` can be used to represent
```
12
  7
    3
      2
    2
  10
    8
    4
```
without a complex data structure.

An array-based tree has the following properties
  - The root is at ```heap[0]```
  - The bottom-most right-most node is at ``heap[size-1]```
  - The bottom-most left-most node is at ``heap[size]```
  - A node can easily be added or removed by setting ```heap[size] = value```
  and updating the size.
  - The left and right children of the item at slot ```p``` can be given
  by ```leftChild(p) = 2*p + 1``` and ```rightChild(p) = 2*p + 2```
  - The parent of a node at position ```c``` can be found using ```parent (c-1)/2```
  (note must be integer division).

A Heap Helper Class Implementation:
```
class HeapHelper
{
public:
  HeapHelper() { num = 0; }
  int GetRootIndex() { return(0); }
  int LeftChildLoc(int i) { return(2*i+1); }
  int RightChildLoc(int i) { return(2*i+2); }
  int ParentLoc(int i) { return((i-1)/2); }
  int PrintVal(int i) { cout << a[i]; }
  void AddNode(int v);
private:
  int a[MAX_ITEMS];
  int num;
};
```

To insert a node:
```
void HeapHelper::AddNode(int v)
{
  a[num] = value;
  num++;
  for(int i = 0; i < num; i++)
  {
    while(a[i] > a[(i-1)/2])
      swap(a[i],a[(i-1)/2]);
  }
}
```

To extract the max value:
1. If the ```count == 0``` (it’s an empty tree), return error.
2. Otherwise, ```heap[0]``` holds the biggest value. Remember it for later.
3. If the ```count == 1``` (that was the only node) then set ```count = 0``` and return the saved value.
4. Copy the value from the right-most, bottom-most node to the root node: ```heap[0] = heap[count-1]```
5. Delete the right-most node in the bottom-most row: ```count = count - 1```
6. Repeatedly swap the just-moved value with the larger of its two children: Starting with ```i=0```, compare and swap: ```heap[i]``` with ```heap[2*i+1]``` and ```heap[2*i+2]```
7. Return the saved value to the user.

**Heap Time Complexity***
Since a heap is a complete binary tree, it will always have log<sub>2</sub>N levels if it has N entries, thus:
- Insertion is O(log<sub>2</sub>N)
- Extraction is O(log<sub>2</sub>N)

#### Heapsort
Simple implementation:
1. Given an array of N items to sort
2. Insert all N items into a new maxheap and empty the array
3. while there are numbers left in the heap:
  - Remove the largest value
  - Place it in the last open slot of the array

This algorithm is O(N*log<sub>2</sub>N). This implementation is inefficient
since a new data structure for the heap must be created, it is more efficient
to turn the existing array into a maxheap:
1. Convert input array into a maxheap
2. While there are numbers left in the heap:
  - Remove the biggest value from the heap
  - Place it in the last open slot of the array
This algorithm is O(N*log<sub>2</sub>N), but it is a faster O(N*log<sub>2</sub>N)

Implementation:
```
void bubbleDown( int a[N], int i, int N){     
	while ( i*2 + 1 < N ) {
 		int child = 2*i + 1;    

		//get bigger child if there are two
		if ((child + 1 < N) && (a[child] < a[child+1])) child++;        

    //reorder into maxheap
		if (a[i] < a[child]) {    
			std::swap( a[child], a[i] );           
			k = child;  
		}        
		else            
			return;    
	}
}  

void heapsort(int a[], int N){       
	// heapify
	for (int k = N/2; k >= 0; k--) {
		bubbleDown( a, k, N);    
	}     

	while (N-1 > 0) {  
		std::swap(a[N-1], a[0]); //swap root with last
		bubbleDown(a, 0, N-1);  //re-heapify
		N--; //decrement heap size
	}
}  
```
