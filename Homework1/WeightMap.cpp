#include "Map.h"
#include "WeightMap.h"
#include <iostream>

WeightMap::WeightMap()
{

}

bool WeightMap::enroll(std::string name, double startWeight)
{
	if (startWeight >= 0 && 
		size() < DEFAULT_MAX_ITEMS && 
		_weightNamePairs.insert(name, startWeight)
		)
		return true;

	else return false;
}

double WeightMap::weight(std::string name) const
{
	double returnWeight = -1;
	_weightNamePairs.get(name, returnWeight);
	return returnWeight;
}

bool WeightMap::adjustWeight(std::string name, double amt)
{
	if (!_weightNamePairs.contains(name) || (weight(name) + amt < 0))
		return false;
	else
	{
		_weightNamePairs.update(name, weight(name) + amt);
		return true;
	}
}

int WeightMap::size() const
{
	return _weightNamePairs.size();
}

void WeightMap::print() const
{
	for (int i = 0; i < size(); i++)
	{
		std::string name;
		double		weight;
		_weightNamePairs.get(i, name, weight);
		std::cout << name << " " << weight << std::endl;
	}
}