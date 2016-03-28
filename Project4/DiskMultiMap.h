#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "DiskMultiMap.h"

class BinaryFile;

class DiskMultiMap
{
public:

	class Iterator
	{
	public:
		Iterator();
		Iterator(const std::string filename, int startOffset);
		// You may add additional constructors
		bool isValid() const;
		Iterator& operator++();
		MultiMapTuple operator*();

	private:
		// Your private member declarations will go here
		std::string _filename;
		bool _valid;
		int _offset;
		int _next;
	};

	DiskMultiMap();
	~DiskMultiMap();
	bool createNew(const std::string& filename, unsigned int numBuckets);
	bool openExisting(const std::string& filename);
	void close();
	bool insert(const std::string& key, const std::string& value, const std::string& context);
	Iterator search(const std::string& key);
	int erase(const std::string& key, const std::string& value, const std::string& context);

private:
	// Your private member declarations will go here

	int generateHash(const std::string key);
	std::string getFilename(){ return _filename; }

	std::string _filename;
	int _hashTableStart;

	struct Header
	{
		unsigned int numBuckets;
		int freeList;
	};
	Header header;
};

#endif // DISKMULTIMAP_H_