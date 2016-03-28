#include "Map.h"

Map::Map()
{
	_nPairs = 0;
	_head = nullptr;
	_tail = nullptr;
}

Map::Map(const Map &other)
{
	//copy values from all nodes into new linked list
	_nPairs = 0;
	while (size() < other.size())
	{
		KeyType		tempKey;
		ValueType	tempValue;
		int otherLastNode = other.size() - size() - 1;
		other.get(otherLastNode, tempKey, tempValue);
		insert(tempKey, tempValue);
	}
}

Map::~Map()
{
	MapNode *p = _tail;
	while (p != nullptr)
	{
		MapNode *q = p;
		p = p->previous;
		erase(q->key);
	}
	_head = nullptr;
	_tail = nullptr;
}

Map& Map::operator=(const Map &other)
{
	//if the map passed by reference is the same as the current map, return *this
	if(&other == this)
		return *this;

	//delete all nodes in list
	MapNode *p = _tail;
	while (p != nullptr)
	{
		MapNode *q = p;
		p = p->previous;
		erase(q->key);
	}
	_head = nullptr;
	_tail = nullptr;

	//copy node values into this object
	while (size() < other.size())
	{
		KeyType		tempKey;
		ValueType	tempValue;
		int otherLastNode = other.size() - size() - 1;
		other.get(otherLastNode, tempKey, tempValue);
		insert(tempKey, tempValue);
	}
	return *this;
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
	MapNode *p = _head;
	while (p != nullptr)
	{
		if (p->key == key)
			return true;	//returns true if map contains a key matching the parameter
		p = p->next;
	}
	return false;			//returns false if no matching key is found
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	//returns false if the map already contains key
	if(contains(key))
		return false;

	else
	{
		//add node to front of linked list
		MapNode *p = new MapNode;
		p->key = key;
		p->val = value;
		p->next = _head;
		p->previous = nullptr;
		_head = p;
		//set previous pointer for old head (if it exists)
		if(_head->next != nullptr)
			_head->next->previous = p;

		//determine new tail pointer
		MapNode *q = _head;
		while (q->next != nullptr)
		{
			q = q->next;
		}
		_tail = q;

		//increment count of number of mapped pairs
		_nPairs++;
		return true;
	}
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	//returns false if the key is not in the map
	if(!contains(key))
		return false;

	//locate key in map and change corresponding value, then return true
	else
	{
		MapNode *p = _head;
		while (p->key != key)
		{
			p = p->next;
		}
		p->val = value;
		return true;
	}
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	//returns true if key/value pair is updated or inserted
	if (update(key, value) || insert(key, value))
		return true;

	else return false;
}

bool Map::erase(const KeyType& key)
{
	if (!contains(key))
		return false;
	else
	{
		//create dummy node to make doubly linked list circular
		MapNode *circLink = new MapNode;
		circLink->next = _head;
		circLink->previous = _tail;
		_head->previous = circLink;
		_tail->next = circLink;

		//find node before node to be deleted
		MapNode *temp = circLink;
		while (temp->next->key != key)
		{
			temp = temp->next;
		}

		//set previous and next pointers of nodes around node to be deleted
		MapNode *deleteThis = temp->next;
		temp->next = deleteThis->next;
		temp->next->previous = temp;
		delete deleteThis;
		_nPairs--;

		//reset head and tail pointers, delete dummy node so list isn't circular
		_head = circLink->next;
		_tail = circLink->previous;
		_head->previous = _tail->next = nullptr;
		delete circLink;

		return true;
	}
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	//return false if key not in map
	if (!contains(key))
		return false;

	//find key and set value parameter to value mapped to by key
	else
	{
		MapNode *p = _head;
		while (p != nullptr)
		{
			if (p->key == key)
				break;
			p = p->next;
		}
		value = p->val;
	}

	return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	//return false if i < 0 or i >= number of mapped pairs
	if ((i < 0) || !(i < size()))
		return false;

	//set parameters to key and value from i+1th node
	else
	{
		MapNode *p = _head;
		int j;
		for (j = 0; j < i; j++)
		{
			p = p->next;
		}
		key = p->key;
		value = p->val;
	}
	return true;
}

void Map::swap(Map& other)
{
	//create temporary variables to store pointers + _nPairs
	Map::MapNode *tempHead	= _head;
	Map::MapNode *tempTail	= _tail;
	int tempPairCount		= _nPairs;

	//swap the pointers and pair count of each map
	_head					= other._head;
	_tail					= other._tail;
	_nPairs					= other._nPairs;
	
	other._head				= tempHead;
	other._tail				= tempTail;
	other._nPairs			= tempPairCount;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	//create temporary map, copy of m1;
	result = m1;
	bool returnBool = true;

	//iterate along every node in m2
	for (int i = 0; i < m2.size(); i++)
	{
		KeyType tempKey;
		ValueType tempValue;
		ValueType inResult;
		m2.get(i, tempKey, tempValue);
		bool keyInBoth = m1.get(tempKey, inResult);

		//if key in both maps, but values don't match, remove from temp, function will return false when done
		if (keyInBoth && (tempValue != inResult))
		{
			result.erase(tempKey);
			returnBool = false;
		}

		//if key in both maps with matching values do nothing
		else if (keyInBoth && tempValue == inResult);

		//if key is not in both maps, insert into temp
		else
			result.insert(tempKey, tempValue);
	}
	return returnBool;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	//result is set to an empty map if m1 and m2 are the same map
	if(&m1 == &m2)
	{
		Map temp;
		result = temp;
		return;
	}

	result = m1;
	for (int i = 0; i < m1.size(); i++)
	{
		KeyType tempKey;
		ValueType tempValue;
		m1.get(i, tempKey, tempValue);
		if (m2.contains(tempKey))
		{
			result.erase(tempKey);
		}
	}
	return;
}
