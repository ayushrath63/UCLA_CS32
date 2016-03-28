# Graphs

A graph is an ADT that stores a set of items and tracks the relationships between
them.

Graphs contain two types of elements _vertices_ and _edges_.

There are two main types of graphs: Directed and Undirected

In a directed graph, an edge can only be followed from a vertex in one direction

In an undirected graph, edges are bidirectional.

The easiest way to represent a graph is a 2D array: ```bool graph[N][M]```
where ```graph[i][j]``` is true if there is an edge between vertex ```i``` and
vertex ```j```. This is called an _adjacency matrix_.

For example:
```
bool graph[5][5];
graph[0][3] = true;
graph[1][2] = true;
graph[3][0] = true;
```
can represent:
```
{4}
{0} <--> {3}
{2} <--- {1}
```

To represent an undirected graph with an adjacency matrix, if ```graph[i][j]```
is true ```graph[j][i]``` must also be true.

Squaring the adjacency matrix of a graph results in a matrix showing which
vertices are two elements apart.

Example:
```
{A}---->{B}
{B}---->{C}
{C}---->{D}
{D}---->{B}
```
Is given by the adjacency matrix:
```
adj[4][4]=
{
  {0,1,0,0},
  {0,0,1,0},
  {0,0,0,1},
  {0,1,0,0}
};
```
And ```adj```<sup>2</sup> is:
```
{
  {0,0,1,0},
  {0,0,0,1},
  {0,1,0,0},
  {0,0,1,0}
}
```

A directed graph with _n_ vertices can also be represented by an array of _n_
linked lists.

Example:
```
list<int> graph[n];
```
If _j_ is added to ```graph[i]``` there is an edge from i to j;

This is called an _adjacency list_.

An adjacency list should be used for sparse graphs(few edges, many vertices), while
and adjacency matrix should be used with dense matrices that have few vertices (many edges).

A graph can be traversed using a DFS or a BFS, just like a binary tree:

DFS for graphs (recursive):
```
Depth-First-Traversal(curVertex)
  If we’ve already visited the current vertex
    Return
  Otherwise
    Mark the current vertex as visited
    Process the current vertex (e.g., print it out)
    For each edge leaving the current vertex
      Determine which vertex the edge takes us to
      Call Depth-First-Traversal on that vertex
```

DFS for graphs (stack based):
```
Depth-First-Search-With-Stack(startVertex)
  Push startVertex on the stack
  While the stack is not empty
    Pop the top item off the stack and put it in variable c
    If c hasn’t been visited yet
      Drop a breadcrumb (we’ve visited the current vertex)
      For each edge e leaving the room
      If the vertex v behind e hasn’t been visited
        Push v onto the stack.
```

BFS for graphs:
```
Breadth-First-Search (startVertex)
Add the starting vertex to our queue
Mark the starting vertex as “discovered”
While the queue is not empty
  Dequeue the top vertex from the queue and place in c
  Process vertex c (e.g., print its contents out)
  For each vertex v directly reachable from c
    If v has not yet been “discovered”
      Mark v as “discovered”
      Insert vertex v into the queue
```
Note the similarity to the queue based maze solving algorithm from earlier

#### Graphs with Weighted Edges

A weighted graph is one that has a weight or cost associated with each edge.
A path is considered longer the higher its weight is.

#### Dijkstra's algorithm

Dijkstra's Algorithm is an algorithm that determines the shortest path between
any two vertices in a weighted graph without negative weights.

The algorithm takes a graph and a starting  vertex as input and produces an array
of optimal distances to every other vertex.

The algorithm splits vertices into two sets:
- Unsettled Vertex: A vertex _v_ is unsettled if we don’t know the optimal distance to it from the starting vertex _s_.
- Settled Vertex: A vertex _v_ is settled if we do know the optimal distance to it from the starting vertex _s_.

Initially all vertices are unsettled, and the algorithm ends once they are all
settled.

Dijkstra's Algorithm uses two data structures:
1. An array _Dist_ that contains the best known cost to get from the starting vertex to
every other vertex.
  - initially, for each vertex _i_, ```Dist[i]``` is infinite, unless _i_ is the starting vertex in which case it is 0
2. An boolean array _Done_ that holds true for every settled vertex and false for every unsettled vertex.
  - initially, every element is false

Pseudocode for Dijkstra's Algorithm:
```
While there are still unprocessed vertices:
  Set u = the closest unprocessed vertex to the start vertex s
  Mark vertex u as processed: Done[u] = true.
  We now know how to reach u optimally from s
  Loop through all unprocessed vertices:
    Set v = the next unprocessed vertex
    If there’s an edge from u to v then compare:
      a. the previously computed path from s to v (i.e. Dist[v])
      OR
      b. the path from s to u, and then from u to v (I.e. Dist[u] + weight(u,v))
      If the new cost is less than old cost then:
        Set Dist[v] = Dist[u] + weight(u,v)
```
