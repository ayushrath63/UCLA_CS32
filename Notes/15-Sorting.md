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

### Quicksort

Quicksort is a divide and conquer search.

1. If the array contains 0 or 1 element, return
2. Select and arbitrary element P (pivot)
3. Move all elements <= P to the left, all elements > P to the right
4. Recursively repeat on left sub-array and right sub-array

Example:
```
int Partition(int a[], int low, int high)
{
  int pi = low;
  int pivot = a[low];
  do
  {
      while (low <= high && a[low] <= pivot)
        low++;
      while (a[high] > pivot)
        high--;
      if (low < high)
        swap(a[low], a[high]);
  }
  while (low < high);
    swap(a[pi], a[high]);
  pi = high;
  return (pi);
}

void QuickSort(int Array[], int First, int Last)
{
  if (Last– First >= 1) {
    int PivotIndex;
    PivotIndex = Partition(Array, First, Last);
    QuickSort(Array, First, PivotIndex - 1); // left
    QuickSort(Array,PivotIndex+1,Last); // right
  }
}
```

The ```Partiton()``` function is O(n).

Quicksort is unstable. Quicksort is O(nlog<sub>2</sub>n) in the best case, in
the worst case (mostly sorted, mostly reverse sorted, many identical elements),
it is O(n<sup>2</sup>). Quicksort uses a variable amount of memory, it varies
significantly. Quicksort is easily parallelizable with multiple cores.

### Mergesort

Mergesort is a divide and conquer search.

Mergesort is based on a simpler ```merge()``` function.
This function takes two pre-sorted arrays and outputs a third sorted array.

1. If array has 0 or 1 element, return
2. Split array into two equal sections
3. Recursively call Mergesort on the left half
4. Recursively call Mergesort on the right half
5. Marge halves

Example:
```
void merge(int data[], int n1, int n2)
{
  int i = 0, j = 0, k = 0;
  int * temp = new int[n1 + n2];
  int * sechalf = data + n1;
  while (i < n1 || j < n2)
  {
    if (i == n1) temp[k++] = sechalf[j++];
    else if (j == n2) temp[k++] = data[i++];
    else if (data[i] < sechalf[j]) temp[k++] = data[i++];
    else temp[k++] = sechalf[j++];
  }
  for (i = 0; i < n1 + n2; i++)
    data[i] = temp[i];
  delete[] temp;
}

void Mergesort(int data[], int n)
{
  if(n <= 1)
    return;
  int left[n/2];
  for(int i = 0; i < n/2; i++)
    left[i] = data[i];
  int right[n-n/2];
  for(int i = n/2; i < n; i++)
    right[i] = data[i];
  Mergesort(left[], n/2);
  Mergesort(right[], n-n/2);
  for(int i = 0; i < n; i++)
  {
    if(i < n/2)
      data[i] = left[i];
    if(i >= n/2)
      data[i] = right[i-n/2];
  }
  merge(data[], n/2, n-n/2)
}
```

Mergesort is always O(nlog<sub>2</sub>n), but uses a lot of memory. Mergesort
cannot be used easily on a linked list, and is stable or unstable depending on
the implementation of ```merge()```. Mergesort is easily parallelizable.
