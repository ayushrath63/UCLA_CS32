#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include "IntelWeb.h"
#include "DiskMultiMap.h"
#include "InteractionTuple.h"

const double LOAD = .75;

IntelWeb::IntelWeb()
{
}

IntelWeb::~IntelWeb()
{
	//close all hashtables
	_fw->close();
	_rev->close();

	//delete pointers
	delete _fw;
	delete _rev;
}

bool IntelWeb::createNew(const std::string & filePrefix, unsigned int maxDataItems)
{
	//if any of the 3 hashtables cannot be created, return false
	_fw = new DiskMultiMap();
	_rev = new DiskMultiMap();;
	if( (_fw->createNew(filePrefix + "FW.dat", (int)(maxDataItems / LOAD))) &&
		(_rev->createNew(filePrefix + "REV.dat", (int)(maxDataItems / LOAD))))
		return true;
	else return false;
}

bool IntelWeb::openExisting(const std::string & filePrefix)
{
	//close any open hashtables
	close();

	//if any of the 3 hashtables cannot be opened, return false
	if ((_fw->openExisting(filePrefix + "FW.dat")) &&
		(_rev->openExisting(filePrefix + "REV.dat")))
		return true;
	else return false;
}

void IntelWeb::close()
{
	//close all hashtables
	_fw->close();
	_rev->close();
}

bool IntelWeb::ingest(const std::string & telemetryFile)
{
	//open telemetry log, return false if cannot be opened
	std::ifstream tFile(telemetryFile);
	if (!tFile.is_open())
	{
		return false;
	}

	//process each line of the file
	std::string line;
	while (std::getline(tFile,line))
	{
		//split line into 3 strings at spaces
		std::string context, key, value;
		std::string delimiter = " ";

		//find 1st item (key)
		context = line.substr(0, line.find(delimiter));
		line.erase(0, line.find(delimiter) + delimiter.length());
		
		//find 2nd item (value)
		key = line.substr(0, line.find(delimiter));
		line.erase(0, line.find(delimiter) + delimiter.length());

		//find 3rd item (context)
		value = line.substr(0, line.find(delimiter));
		line.erase(0, line.find(delimiter) + delimiter.length());

		_fw->insert(key, value, context);
		_rev->insert(value, key, context);
	}

	return true;
}

unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators, 
							 unsigned int minPrevalenceToBeGood, 
							 std::vector<std::string>& badEntitiesFound, 
							 std::vector<InteractionTuple>& interactions)
{
	//MALICIOUS ENTITY DISCOVERY RULES
	//1. Downloading a file from a malicious website, file has prevalence < P_good -> file malicious
	//2. Downloading a malicious file from a website, website has prevalence < P_good -> website malicious
	//3. A known malicious file F creates a  file G, if G < P_good -> F is malicious
	//4. A file F creates a known malicious file G, if F < P_good -> F is malicious
	//5. If a file F connects to a known malicious website W, F < P_good -> F is malicious
	//6. If a known malicious file F connects to a website W, W < P_good -> W is malicious
	//1,3,5 similar for all interaction types
	//2,4,6 similar for all interaction types

	//create data structure to store possible malicious entities
	std::map<std::string, int> candidates;
	std::map<std::string, int> malicious;
	
	//iterate through all given indicators (O(T)) to find other malicious entities with reverse interactions
	for (int i = 0; i < (int)indicators.size(); i++)
	{
		//queue used to store indicators/malicious entities while performing BFS
		std::queue<std::string> bfsQrev;
		
		//BFS to find possible malicious entities in reverse interactions
		bfsQrev.push(indicators[i]);
		while (!bfsQrev.empty())
		{
			std::string current = bfsQrev.front();
			bfsQrev.pop();

			DiskMultiMap::Iterator rev_it = _rev->search(current);
			while (rev_it.isValid())
			{
				if(candidates.find(current) == candidates.end() && _fw->search(current).isValid())
					bfsQrev.push((*rev_it).value);
				++rev_it;
			}
			candidates[current] += 1;
		}
	}

	//get every object that has a reverse interaction with the indicators, directly or indirectly
	for (std::map<std::string, int>::iterator it = candidates.begin(); it != candidates.end(); ++it)
	{
		//queue used to store indicators/malicious entities while performing BFS
		std::queue<std::string> bfsQfw;

		//BFS to find possible malicious entities in forward interactions
		bfsQfw.push(it->first);

		while (!bfsQfw.empty())
		{
			std::string current = bfsQfw.front();
			bfsQfw.pop();
			//Push current interaction onto end of interactions vector
			DiskMultiMap::Iterator fw_it = _fw->search(current);
			while (fw_it.isValid())
			{
				if (malicious.find(current) == malicious.end())
				{
					bfsQfw.push((*fw_it).key);
					bfsQfw.push((*fw_it).value);
				}
				++fw_it;
			}
			malicious[current] += 1;
		}
	}

	//find all entities from those marked malicious that meet p_good, find objects they forward interacts with and mark them good too
	for (std::map<std::string, int>::iterator it = malicious.begin(); it != malicious.end(); ++it)
	{
		if((int)it->second >= minPrevalenceToBeGood && find(indicators.begin(),indicators.end(),it->first) == indicators.end())
		{
			DiskMultiMap::Iterator fw_it = _fw->search(it->first);
			while (fw_it.isValid())
			{
				malicious[(*fw_it).value] = minPrevalenceToBeGood+1;
				++fw_it;
			}
		}
	}


	for (int i = 0; i < indicators.size(); i++)
		if(_fw->search(indicators[i]).isValid())
			badEntitiesFound.push_back(indicators[i]);

	for (std::map<std::string, int>::iterator it = malicious.begin(); it != malicious.end(); ++it)
	{
		if ((int)it->second < minPrevalenceToBeGood && 
			find(badEntitiesFound.begin(),badEntitiesFound.end(), it->first) == badEntitiesFound.end()
			)
		{
			badEntitiesFound.push_back(it->first);
		}
	}

	for (int i = 0; i < badEntitiesFound.size(); i++)
	{
		DiskMultiMap::Iterator fw_it = _fw->search(badEntitiesFound[i]);
		while (fw_it.isValid())
		{
			InteractionTuple currentInteraction;
			currentInteraction.context = (*fw_it).context;
			currentInteraction.to = (*fw_it).value;
			currentInteraction.from = (*fw_it).key;
			interactions.push_back(currentInteraction);
			++fw_it;
		}

		DiskMultiMap::Iterator rev_it = _rev->search(badEntitiesFound[i]);
		while (rev_it.isValid())
		{
			InteractionTuple currentInteraction;
			currentInteraction.context = (*rev_it).context;
			currentInteraction.to = (*rev_it).key;
			currentInteraction.from = (*rev_it).value;
			interactions.push_back(currentInteraction);
			++rev_it;
		}
	}

	std::sort(badEntitiesFound.begin(), badEntitiesFound.end());

	return badEntitiesFound.size();
}

bool IntelWeb::purge(const std::string & entity)
{
	bool returnThis = false;
	//Erases all entity appearances from _fw and _rev
	DiskMultiMap::Iterator fw_it = _fw->search(entity);
	while (fw_it.isValid())
	{
		returnThis = true;
		_fw->erase((*fw_it).key, (*fw_it).value, (*fw_it).context);
		if (!fw_it.isValid())
			fw_it = _fw->search(entity);
		else ++fw_it;
	}
	DiskMultiMap::Iterator it = _rev->search(entity);
	while (it.isValid())
	{
		returnThis = true;
		_fw->erase((*it).value, entity, (*it).context);
		if(!it.isValid())
			it = _rev->search(entity);
		else ++it;
	}

	return returnThis;
}