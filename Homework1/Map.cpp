#include "Map.h"

Map::Map()
{
	_nPairs = 0;
}

bool Map::empty() const
{
	return !_nPairs; //returns true if the map is empty, false otherwise
}

int Map::size() const  
{
	return _nPairs; //returns the number of key/value pairs in the map
}

bool Map::contains(const KeyType& key) const
{
	if (empty())
		return false;			//return false if map is empty
	else
		for (int i = 0; i < size(); i++)
			if (key == _pairs[i]._key)
				return true;	//return true if the key is in the map
	return false;				//return false if it is not in the map
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (size() == DEFAULT_MAX_ITEMS)
		return false;	//return false if th map is full

	//check that the map does not contain the key provided
	bool keyInMap = false;
	if (contains(key))
		keyInMap = true;

	//add key/value pair if not in map, return true
	if(!keyInMap)
	{
		_pairs[size()]._key	= key;
		_pairs[size()]._val	= value;
		_nPairs++;
		return true;
	}
	else return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i < size(); i++)
		if (key == _pairs[i]._key)
		{
			_pairs[i]._val = value;
			return true;
		}
	return false;	//return false if key not in map
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (update(key, value))
		return true;
	else if (insert(key, value))
		return true;
	else return false;
}

bool Map::erase(const KeyType& key)
{
	//search for key in the map, if found, shift every pair right by 1 and return true
	for (KeyValPair *p = _pairs; p < (_pairs + size()); p++)
		if ((*p)._key == key)
		{
			for (; p < (_pairs + size() - 1); p++)
			{
				*p = *(p + 1);
			}
			_nPairs--; //decrement number of pairs
			return true;
		}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	//search for matching key in map, if found, set the value parameter to the value mapped to 
	//by the key parameter and return true
	for (const KeyValPair *p = _pairs; p < (_pairs + size()); p++)
		if ((*p)._key == key)
		{
			value = (*p)._val;
			return true;
		}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	//if 0<=i<size(), set the key and value parameters to those at
	//index i of the array of key/value pairs
	if (0 <= i && i < size())
	{
		key		= _pairs[i]._key;
		value	= _pairs[i]._val;
		return true;
	}
	return false;
}

void Map::swap(Map& other)
{
	int tempPairCount = _nPairs;
	_nPairs = other._nPairs;
	other._nPairs = tempPairCount;
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		KeyValPair tempPairStruct = *(_pairs + i);
		*(_pairs + i) = *(other._pairs + i);
		*(other._pairs + i) = tempPairStruct;
	}
}
