# Sorting

Sorting is the process of ordering items based on one or more rules, subject to
one or more constraints.

The optimal sorting algorithm varies with requirements, sometimes, it is better
to use a simpler but less efficient one.

A sort is _stable_ if it maintains the initial relative positions of items.

A sort is _unstable_ if it does not maintain the initial relative positions of the items.

### Selection Sort
1. Look at all N items, select the smallest items
2. Swap with the first items
3. Repeat until sorted

The selection sort is O(n<sup>2</sup>). It is very slow. The selection sort is
not any more efficient on a mostly ordered set.

Example:
```
void selectionSort(int A[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
            if (A[j] < A[minIndex]) minIndex = j;
        swap(A[i], A[minIndex]);
    }
}
```

Selection sort is unstable.

### Insertion Sort
1. Consider the first 2 items of a set.
1. If the last item in the is in the wrong order,  store the value and remove it.
2. Shift all other items right
3. Insert the removed item into the appropriate slot
5. Repeat on the first n + 1 items

Insertion sort is O(n<sup>2</sup>), so it is still slow, but it is more efficient
on a set of items that is already ordered. If the items are sorted already, the
algorithm is O(n). For a set of items in reverse order, it is O(n<sup>2</sup>).

Example:
```
void insertionSort(int A[], int n)
{
    for (int s = 2; s <= n; s++)
    {
        int sortMe = A[s - 1];
        int i = s - 2;
        while (i >= 0 && sortMe < A[i])
        {
            A[i + 1] = A[i];
            --i;
        }
        A[i + 1] = sortMe;
    }
}
```

Insertion sort cannot be used on a singly linked list, since the algorithm needs
to examine previous values.

Insertion sort is stable.

### Bubble Sort

1. Start at first element
2. Compare first two elements, if they are not in order, swap them
3. Advance one element and repeat
4. Repeat until the end is reached
5. If at least one swap was made, repeat the entire process

Bubble sort is O(n<sup>2</sup>) in the worst case (smallest item at the end).

Example:
```
void bubbleSort(int Arr[], int n)
{
    bool atLeastOneSwap;
    do
    {
        atLeastOneSwap = false;
        for (int j = 0; j < (n - 1); j++)
        {
            if (Arr[j] > Arr[j + 1])
            {
                Swap(Arr[j], Arr[j + 1]);
                atLeastOneSwap = true;
            }
        }
    }
    while (atLeastOneSwap == true);
}
```

Bubble sort is stable.

### Shell Sort

Shell sort is based on an underlying sort called _h-sort_

---
**h-sort**

  - Pick a value _h_
  - For each element in the array (stop if a[i+h] is out of bounds)
    - if a[i] and a[i+h] are out of order
      - swap the two elements
  - If at least one swap has been made, repeat the entire process (same _h_)

  Note: bubble sort is an h-sort of h = 1

  At the end, the array is not necessarily sorted, it is just h-sorted
---

1. Select a sequence of decreasing _h_ values, ending in _h_ = 1
2. Perform an h-sort for each _h_ value, ending in bubble sort

Shell sort is very low memory. Empirically, shell sort has been found to be
O(n<sup>1.25</sup>), an analytic solution for the Big-O is not known.
It is unstable.
