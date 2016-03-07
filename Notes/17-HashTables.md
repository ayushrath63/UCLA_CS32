# Hash Tables


A _hash table_ is a data structure that that can perform searching and insertion
in O(1) time.


Suppose we need a data structure to store and search for 50,000 ID numbers in O(1)
Start with a VERY large array.

```
class AlmostHashTable
{
public:
  void addItem(int n)
  {
    m_array[n] = true;
  }
  bool holdsItem(int q)
  {
    return m_array[q] == true;
  }
private:
  bool m_array[100000000]; // big!
}

int main()
{
  AlmostHashTable x;
  x.addItem(400683948);
  if (x.holdsItem(1234) != true)
    cout << “Couldn’t find it!”;
}
```

The above example is an O(1) search, but it uses a wastes amount of memory that.
Now suppose we want to store the IDs in a 100,000 element array, so we need to
convert from 0 - 999999999 to a number in a range from 0 - 99999. This can
done using a modulo operator.

```
class AlmostHashTable2
{
public:
  void addItem(int n)
  {
    int slot = hashFunc(n);
    m_array[slot] = true;
  }
  bool containsItem(int q)
  {
    int slot = hashFunc(q);
    return m_array[slot] == true;
  }
private:
  int hashFunc(int idNum)
  {
    const int ARRAY_SIZE = 100000;
    int bucket = idNum % ARRAY_SIZE;
    return bucket;
  }
  bool m_array[100000]; // not so big!
};

```

The method ```hashFunc()``` converts an ID number into a _bucket number_ where
the boolean is stored. This implementation will fail in instances when there are
two numbers map to the same bucket. This is called a collision.

There are two common methods to solve this:

#### The _Closed Hash Table_ (aka _Linear Probing_):

As before, use the hash function to locate the right bucket in the array,
however, store the original value in the bucket instead of a boolean. If that
bucket is occupied, scan down from that bucket and put the value in the next
open bucket.

This is between O(1) and O(n).

To search, use linear probing. Go to the target bucket, and probe downwards if
it is not there, until the value is found or an empty bucket is found.

In a Linear Probing Hash Table, each bucket is a struct:

```
struct BUCKET
{
  int idNum;
  bool used;
}
```

Example:

```
#define NUM_BUCK 10
class HashTable {
public:
  void insert(int idNum)
  {
    int bucket = hashFunc(idNum);
    for (int tries=0;tries<NUM_BUCK;tries++)
    {
      if (m_buckets[bucket].used == false)
      {
        m_buckets[bucket].idNum = idNum;
        m_buckets[bucket].used = true;
        return;
      }
      bucket = (bucket + 1) % NUM_BUCK; //wraps around when end reached
    }
    // no room left in hash table!!!
  }
private:
  int hashFunc(int idNum) const { return idNum % NUM_BUCK; }
  BUCKET m_buckets[NUM_BUCK];
};
```

The problem with this is the limited size, and that issues arise when the hash
table if filled. This is not a problem with linked lists or binary search trees

Additional values can be stored in each bucket as well.

Deleting from a closed hash table is slightly more complicated. If you just set
the used flag to false, linear probing for other results will false. This can
circumvented using a tombstone, but, in general deletion should not be done on a
closed hash table.

#### The _Open Hash Table_

Instead of storing values directly, have each bucket point to a linked list of
values.

Searching for an item in such a list is O(n/b) where n is the number of items
and b is the number of buckets. For n < b O(n/b) is approximately O(1).

A BST can also be used instead of a linked list for slightly better performance.

Initializing a hash table is O(n).

Hash table efficiency is dependent on the type (closed/open), how full it is,
how many collisions there are.

Hash tables that are mostly full are inefficient,

The _load_ of a hash table is the number of values divided by the number of
buckets, n/b.

For a closed hash table, the average number of searches is ½(1+ 1/(1-L)) for L < 1.0

For an open hash table, the average number of searches is 1 + L/2

The STL containers ```unordered_set``` and ```unordered_map``` are based on
hash tables.

Hash tables have a tradeoff between memory usage and search time.

When choosing the number of buckets for a hash table, you should choose a prime
number.

Choosing a hash function:

The hash function should always return the same value for the same input.
The hash function should produce a roughly even distribution of items.

**Hash Tables vs. Binary Search Trees**

Speed: Hash Table > BST

Simplicity: Hash Table > BST

Max Size: Hast Table < BST

Space Efficiency: Hash Table < BST

Ordering: Hash Table < BST
