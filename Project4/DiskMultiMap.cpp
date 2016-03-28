#include "DiskMultiMap.h"
#include "MultiMapTuple.h"
#include "BinaryFile.h"

const int MAX_DATA_SIZE = 120;

DiskMultiMap::Iterator::Iterator()
{
	_filename = "";
	_valid = false;
}

DiskMultiMap::Iterator::Iterator(const std::string filename, int startOffset)
{
	if(startOffset != 0)
		_valid = true;
	else _valid = false;

	_offset = startOffset;
	_filename = filename;
	BinaryFile bf;
	if (bf.openExisting(_filename))
		bf.read(_next, _offset + MAX_DATA_SIZE * 3);
}

bool DiskMultiMap::Iterator::isValid() const
{
	return _valid;
}

DiskMultiMap::Iterator & DiskMultiMap::Iterator::operator++()
{
	//sets invalid if cannot advance or is already invalid
	if (!isValid() || _next == 0)
	{
		_valid = false;
		return *this;
	}

	//advance to next node in list
	_offset = _next;
	BinaryFile bf;
	if(bf.openExisting(_filename))
		bf.read(_next, _offset + MAX_DATA_SIZE * 3);	
	return *this;
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
	//create empty tuple to return if invalid iterator
	MultiMapTuple returnTuple;
	returnTuple.key = returnTuple.value = returnTuple.context = "";
	if (!isValid())
		return returnTuple;

	//read file and return tuple with values
	BinaryFile bf;
	if (bf.openExisting(_filename))
	{
		char tmp[MAX_DATA_SIZE];

		//read key
		bf.read(tmp,_offset);
		returnTuple.key = tmp;

		//read value
		bf.read(tmp, _offset+MAX_DATA_SIZE);
		returnTuple.value = tmp;

		//read context
		bf.read(tmp, _offset+MAX_DATA_SIZE*2);
		returnTuple.context = tmp;

		//read offset of next node
		bf.read(_next, _offset + MAX_DATA_SIZE * 3);
	}
	if (bf.isOpen()) bf.close();
	return returnTuple;
}

DiskMultiMap::DiskMultiMap()
{
}

DiskMultiMap::~DiskMultiMap()
{
	close();
}

bool DiskMultiMap::createNew(const std::string & filename, unsigned int numBuckets)
{
	bool returnVal = false;
	BinaryFile bf;
	if (bf.createNew(filename))
	{
		_filename = filename;
		//create header in the file with the number of buckets
		header.numBuckets = numBuckets;
		header.freeList = 0;
		bf.write(header, 0);
		_hashTableStart = bf.fileLength();
		//initialize buckets

		//write offsets storing start for each bucket
		for (int i = 0; i < (int)header.numBuckets; i++) //O(B)
			bf.write(0, bf.fileLength());

		returnVal = true;
	}
	if (bf.isOpen()) bf.close();
	return returnVal;
}

bool DiskMultiMap::openExisting(const std::string & filename)
{
	close();
	bool returnVal = false;
	BinaryFile bf;
	if (bf.openExisting(filename))
	{
		bf.read(header.numBuckets, 0);
		_filename = filename;
		returnVal = true;
	}
	if(bf.isOpen()) bf.close();
	return returnVal;
}

void DiskMultiMap::close()
{
	_filename = "";
}

bool DiskMultiMap::insert(const std::string & key, const std::string & value, const std::string & context)
{
	//return false if key, value or context is too large
	if (key.size() > MAX_DATA_SIZE || value.size() > MAX_DATA_SIZE || context.size() > MAX_DATA_SIZE)
		return false;

	int hash = generateHash(key);
	int llheadOffset = sizeof(header) + hash*sizeof(int);
	int	itemOffset = 0;
	BinaryFile bf;
	if (bf.openExisting(_filename))
	{
		bf.read(itemOffset, llheadOffset);
		//insert into disk based linked list with head at llheadOffset
		int next = itemOffset;
		int prev = llheadOffset - (MAX_DATA_SIZE * 3);
		while (next != 0)
		{
			prev = next;
			bf.read(next, next + (MAX_DATA_SIZE * 3));
		}

		//temporary character array to copy std::strings into
		char tmp[MAX_DATA_SIZE];
		//write offset of current node to end of last node
		bf.write(bf.fileLength(), prev + (MAX_DATA_SIZE * 3));

		//write key
		//strcpy(tmp, key.c_str()); Visual Studio complaining about strcpy, so manually re-implementing
		for (int i = 0; i <= (int)key.size(); i++)
			tmp[i] = (key.c_str())[i];
		bf.write(tmp, bf.fileLength());

		//write value
		for (int i = 0; i <= (int)value.size(); i++)
			tmp[i] = (value.c_str())[i];
		bf.write(tmp, bf.fileLength());

		//write context
		for (int i = 0; i <= (int)context.size(); i++)
			tmp[i] = (context.c_str())[i];
		bf.write(tmp, bf.fileLength());

		//write offset of next, set to 0
		bf.write(0, bf.fileLength());
		
		return true;
	}
	else return false;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string & key)
{
	if (key.size() > MAX_DATA_SIZE)
		return Iterator();

	//generate hash form key and check if any items exist with the hash
	int hash = generateHash(key);
	int llheadOffset = sizeof(header) + hash*sizeof(int);
	int	itemOffset = 0;
	BinaryFile bf;
	if(bf.openExisting(_filename))
		bf.read(itemOffset, llheadOffset);
	else return Iterator();

	return Iterator(_filename, itemOffset);
}

int DiskMultiMap::erase(const std::string & key, const std::string & value, const std::string & context)
{
	int delCount = 0;
	//if any parameter too large, no items deleted
	if(key.size() > MAX_DATA_SIZE || value.size() > MAX_DATA_SIZE || context.size() > MAX_DATA_SIZE)
		return 0;
	
	int hash = generateHash(key);
	BinaryFile bf;
	if (bf.openExisting(_filename))
	{
		DiskMultiMap::Iterator it = search(key);
		int llheadOffset = sizeof(header) + hash*sizeof(int);
		int itemOffset = 0;
		bf.read(itemOffset, llheadOffset);

		//return 0 if no items with corresponding key found
		if (itemOffset == 0)
			return 0;

		else
		{
			int next = itemOffset;
			int prev = llheadOffset;
			while (next != 0)
			{
				//stores whether or not iterator has advanced
				bool advanced = false;
				//find and delete nodes, add freed nodes to free list
				if (((*it).key == key) && ((*it).value == value) && ((*it).context == context))
				{
					int temp = prev;
					//if deleting first node	
					if (prev == llheadOffset)
					{
						bf.read(temp, next + (MAX_DATA_SIZE * 3));
						bf.write(temp, prev);
						delCount++;
					}
					else
					{
						//advance next by 1, set prev to next
						temp = next;
						if (next != 0)
						{
							bf.read(next, next + (MAX_DATA_SIZE * 3));
							++it;
						}

						//write location after current node to next of previous node
						bf.write(next, prev);
						advanced = true;
						delCount++;
					}
					
				}
				//if have not advanced through array
				if (!advanced)
				{
					++it;
					prev = next + (MAX_DATA_SIZE * 3);
					if (next != 0)
						bf.read(next, next + (MAX_DATA_SIZE * 3));
				}
			}
		}
		return delCount;
	}
	else return 0;
}

int DiskMultiMap::generateHash(const std::string key)
{
	//adds bytes for every character in key 
	int hash = 0;
	for (int i = 0; i < (int)key.size(); i++)
		hash += key.c_str()[i] * i;
	
	hash <<= hash;
	
	//modulo to make sure fits within number of buckets
	hash %= header.numBuckets;

	return hash;
}
