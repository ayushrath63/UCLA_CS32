/*
#include "Map.h"
#include <cassert>
#include <string>
#include <iostream>

using namespace std;

void test()
{
	Map<int, double> mid;
	Map<string, int> msi;
	assert(msi.empty());
	assert(msi.size() == 0);
	assert(msi.insert("Hello", 10));
	assert(mid.insert(10, 3.5));
	assert(msi.update("Hello", 20));
	assert(mid.update(10, 4.75));
	assert(msi.insertOrUpdate("Goodbye", 30));
	assert(msi.erase("Goodbye"));
	assert(mid.contains(10));
	int k;
	assert(msi.get("Hello", k));
	string s;
	assert(msi.get(0, s, k));
	Map<string, int> msi2(msi);
	msi2.swap(msi);
	msi2 = msi;
	combine(msi, msi2, msi);
	combine(mid, mid, mid);
	subtract(msi, msi2, msi);
	subtract(mid, mid, mid);
}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}
*/
#include "Map.h"  // class template from problem 1

class Coord
{
public:
	Coord(int r, int c) : m_r(r), m_c(c) {}
	Coord() : m_r(0), m_c(0) {}
	double r() const { return m_r; }
	double c() const { return m_c; }
private:
	double m_r;
	double m_c;
};

int main()
{
	Map<int, double> mid;
	mid.insert(42, -1.25);         // OK
	Map<Coord, int> mpi;
	mpi.insert(Coord(40, 10), 32);  // error!
}