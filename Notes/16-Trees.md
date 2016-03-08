# Trees

A tree is a linked-list based data structure with many uses:
  - Organizing hierarchical data
  - Make information easily searchable
  - Simplify evaluation of expressions
  - Decision making

Trees are made of nodes, each containing pointers to child nodes and a root
pointer. The top of a tree is called it's root node. Two root nodes cannot point
to the same child node, a child node cannot point to a root node.
A tree with no nodes is an empty tree.

A _binary tree_ is a special kind of trees where each node has two child nodes.
This is the most common kind of tree.

Starting at any node in a tree will give you a sub-tree, this is useful for
recursion.

A _full binary tree_ is one in which every leaf node has the same depth and every
non-leaf has exactly two children.

Common operations on binary trees:
 - enumerating items
 - searching for an item
 - adding a new item at a certain position on the trees
 - deleting an items
 - deleting the entire tree (destruction)
 - removing a whole section of the tree (pruning)
 - adding a section to the tree (grafting)

 Simple tree example:

```
struct BTNODE // node
{
  int value; // data
  BTNODE *left, *right;
};

main()
{
  BTNODE *temp, *pRoot;
  pRoot = new BTNODE;
  pRoot->value = 5;
  temp = new BTNODE;
  temp->value = 7;
  temp->left = NULL;
  temp->right = NULL;
  pRoot->left = temp;
  temp = new BTNODE;
  temp->value = -3;
  temp->left = NULL;
  temp->right = NULL;
  pRoot->right = temp;
  // etc…
}
```

Iterating through nodes in a tree is called traversal. When traversing at a tree,
always start as a root node.

There are 4 ways to traverse through a binary tree:
1. Pre-order traversal
2. In-order traversal
3. Post-order traversal
4. Level-order traversal

### Pre-Order traversal

1. Process current node
2. Process left sub-tree
3. Process right sub-tree

This is a variation of a depth-first search.

Example:
```
void PreOrder(Node *cur)
{
  if (cur == NULL) // if empty, return…
    return;
  cout << cur->value; // Process the current node.
  PreOrder(cur->left); // Process nodes in left sub-tree.
  PreOrder(cur-> right); // Process nodes in left sub-tree.
}
```

### In-order traversal

1. Process nodes in left sub-tree
2. Process the current node
3. Process the nodes in the right sub-tree

Example:
```
void InOrder(Node *cur)
{
  if (cur == NULL) // if empty, return…
    return;
  InOrder(cur->left); // Process nodes in left sub-tree.
  cout << cur->value; // Process the current node.
  InOrder(cur-> right); // Process nodes in left sub-tree.
}
```

### Post-order traversal

1. Process nodes in left sub-tree
2. Process the nodes in the right sub-tree
3. Process the current node

Example:
```
void PostOrder(Node *cur)
{
  if (cur == NULL) // if empty, return…
    return;
  PostOrder(cur->left); // Process nodes in left sub-tree.
  PostOrder(cur-> right); // Process nodes in left sub-tree.
  cout << cur->value; // Process the current node.
}
```

### Level order traversal

1. Use a temp pointer variable and a queue of node pointers
2. Insert root node pointer into queue
3. While queue not empty:  
4. Dequeue the top node pointer and put in temp
5. Process the node
6. Add the node's children to queue if not NULL
7. End While

The Big-O of all of these traversals is O(n).

## Binary Search Trees (BST)

A binary search tree is a binary tree with the following properties:
1. All nodes in a node's left sub-tree must be less than the node's value
2. All nodes in a node's right sub-tree must be greater than the node's value

### Operations on a BST

#### Searching

Pseudocode:

```
Start at the root of the tree
Keep going until we hit the NULL pointer
  If V is equal to current node’s value, then found!
  If V is less than current node’s value, go left
  If V is greater than current node’s value, go right
If we hit a NULL pointer, not found.
```

Iterative implementation:

```
bool Search(int V,Node *ptr)
{
  while (ptr != NULL)
  {
    if (V == ptr->value) return(true);
    else if (V < ptr->value) ptr = ptr->left;
    else ptr = ptr->right;
  } return(false); // nope
}
```

Recursive implementation:

```
bool Search(int V, Node *ptr)
{
  if (ptr == NULL) return(false); // nope
  else if (V == ptr->value) return(true); // found!!!
  else if (V < ptr->value) return(Search(V,ptr->left));
  else return(Search(V,ptr->right));
}
```

Searching a BST is O(log<sub>2</sub>n) in the best case, O(n) in the worst.

#### Inserting

Pseudocode:

```
Input: A value V to insert

If the tree is empty
  Allocate a new node and put V into it
  Point the root pointer to our new node. DONE!

Start at the root of the tree
While we’re not done…  
  If V is equal to current node’s value, DONE! (nothing to do...)
  If V is less than current node’s value
    If there is a left child, then go left
    ELSE allocate a new node and put V into it, and
      set current node’s left pointer to new node. DONE!
  If V is greater than current node’s value
    If there is a right child, then go right
    ELSE allocate a new node and put V into it,
      set current node’s right pointer to new node. DONE!
```

```
struct Node
{
  Node(const std::string &myVal)
  {
    value = myVal;
    left = right = NULL;
  }
  std::string value;
  Node *left,*right;
};

class BinarySearchTree
{
public:
  BinarySearchTree()
  {
    m_root = NULL;
  }
  void insert(const std::string &value);
private:
  Node *m_root;
};

void BinarySearchTree::insert(const std::string &value)
{
if (m_root == NULL)
  {
    m_root = new Node(value);
    return;
  }
Node *cur = m_root;
for (;;) {
  if (value == cur->value)
    return;
  if (value < cur->value)
  {
    if (cur->left != NULL)
      cur = cur->left;
    else
    {
      cur->left = new Node(value);
      return;
    }
  }
  else if (value > cur->value)
  {
    if (cur->right != NULL)
      cur = cur->right;
    else
    {
      cur->right = new Node(value);
      return;
    }
  }
}
```

Do not insert ordered numbers into a non-balanced binary search tree, it will
be slow.

Insertion is O(log<sub>2</sub>n) in the best case, O(n) in the worst.

**Finding the max and min of a BST**
- the min is the leftmost value
- the max is the rightmost value  

### Freeing BST

```
void FreeTree(Node *cur)
{
if (cur == NULL) // if empty, return…
  return;
FreeTree(cur->left); // Delete nodes in left sub-tree.
FreeTree (cur-> right); // Delete nodes in left sub-tree.
delete cur; // Free the current node
}
```

### Deleting a Node

```
Given a Value V for delete
//find V in tree with modified BST search using current and parent pointers
parent = NULL
cur = root
While (cur != NULL)
  If (V == cur->value) then we’re done.
  If (V < cur->value) parent = cur; cur = cur->left;
  Else if (V > cur->value) parent = cur; cur = cur->right;
//If the node was found, delete it from the tree, making sure to preserve its ordering (3 Cases)!
If node is leaf:
  If target node is not the root node
    Unlink the parent node from the target node (cur) by setting the parent’s appropriate link to NULL
    Delete target (cur) node
  If target is root node
    Set root pointer to NULL
    Delete target (cur) node
if node has one child:
  If target node is not the root node
    Relink the parent node to the target (cur) node’s only child.
    Delete target (cur) node
  If target node is the root node
    Relink the root pointer to the target (cur) node’s only child.
    Delete target (cur) node
if node has 2 children:
  Replace value in target with either left sub-tree's largest child or right sub-tree's smallest child
  //Guaranteed to be a leaf or have 1 child
  Delete node that held replacement value
```

The STL ```map``` and ```set``` use a BST.

Huffman compression is tree based.

## Balanced Binary Search Trees

A Balanced BST is a BST that is reorganized so that it always has log<sub>2</sub>
levels.
