// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
	//return false if array empty
	if (n == 0)
		return false;

	else
	{
		//if first element returns true, return true
		if (somePredicate(*a))
			return true;

		//else call any true on all elements after
		else
			return anyTrue(a + 1, n - 1);
	}
    return false;
}

  // Return the number of elements in the array for which the
  // somePredicate function returns true.
int countTrue(const double a[], int n)
{
	//return 0 if end of array reached
	if (n == 0)
		return 0;

	//if first element returns true, add 1 to value returned by recurive calls
	if (somePredicate(*a))
		return 1 + countTrue(a + 1, n - 1);

	//else call countTrue on remaining members of array
	else
		return countTrue(a + 1, n - 1);
}

  // Return the subscript of the first element in the array for which
  // the somePredicate function returns true.  If there is no such
  // element, return -1.
int firstTrue(const double a[], int n)
{
	//return -1 if end of array reached
	if (n == 0)
		return -1;

	//return 0 if first true found
	if (somePredicate(*a))
		return 0;

	else
	{
		//if end of array not reached, increment index by 1
		if (firstTrue(a + 1, n - 1) != -1)
			return 1 + firstTrue(a + 1, n - 1);

		//return -1 if end of array reached;
		else return -1;
	}
}

  // Return the subscript of the smallest element in the array (i.e.,
  // the one whose value is <= the value of all elements).  If more
  // than one element has the same smallest value, return the smallest
  // subscript of such an element.  If the array has no elements to
  // examine, return -1.
int indexOfMin(const double a[], int n)
{
	//return -1 if no elements
	if(n == 0)
		return -1;

	//if only one element, it is the min
	if (n == 1)
		return 0;

	//if last element is less than the min of the rest, return index of the last one
	if (*(a + n - 1) < *(a + indexOfMin(a, n - 1)))
		return n - 1;

	//otherwise return index of min of first n-1 elements
	else
		return indexOfMin(a, n - 1);
}

  // If all n2 elements of a2 appear in the n1 element array a1, in
  // the same order (though not necessarily consecutively), then
  // return true; otherwise (i.e., if the array a1 does not include
  // a2 as a not-necessarily-contiguous subsequence), return false.
  // (Of course, if a2 is empty (i.e., n2 is 0), return true.)
  // For example, if a1 is the 7 element array
  //    10 50 40 20 50 40 30
  // then the function should return true if a2 is
  //    50 20 30
  // or
  //    50 40 40
  // and it should return false if a2 is
  //    50 30 20
  // or
  //    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	//return true if second array empty
	if (n2 == 0)
		return true;

	//return false if not enough elements in first array
	if (n1 < n2)
		return false;

	if (*a1 != *a2)
		return includes(a1 + 1, n1 - 1, a2, n2);

	else
		return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
}
