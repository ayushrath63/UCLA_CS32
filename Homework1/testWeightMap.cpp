#include "Map.h"
#include "WeightMap.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
	WeightMap wMap;
	assert(wMap.size() == 0);

	wMap.enroll("testName", 2.0);
	assert(wMap.weight("testName") == 2.0);
	assert(wMap.size() == 1);
	assert(wMap.adjustWeight("testName", -3) == false);
	
	wMap.adjustWeight("testName", 5);
	assert(wMap.weight("testName") == 7);

	wMap.enroll("testName2", 2.1);
	wMap.enroll("testName3", 2.4);
	wMap.enroll("testName4", 5.0);
	wMap.print();
}