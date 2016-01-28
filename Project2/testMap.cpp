#include "Map.h"
#include <string>

using namespace std;

int main()
{
	Map a;
	Map q(a);
	a.insertOrUpdate("armada", 0);
	a.insert("mango", 17);
	a.insert("pi", 3.1415);
	a.insertOrUpdate("armada", 1);
	a.erase("pi");
	a.insert("pi", 3.2);
	a.update("pi", 3.1415);
	a = a;
	Map *b = new Map;
	b->insert("a", 1);
	b->insert("b", 2);
	b->insert("c", 3);
	b->insert("d", 4);
	Map c(*b);
	a.swap(*b);
	delete b;
	
	Map m1, m2, result;

	m1.insert("Fred", 123);
	m1.insert("Ethel", 456);
	m1.insert("Lucy", 789);

	m2.insert("Lucy", 654);
	m2.insert("Ricky", 321);

	combine(m1, m1, result);
	combine(m1, m2, result);

	m2.insert("Ethel", 654);
	subtract(m1, m2, result);

	subtract(m1, m1, result);
}