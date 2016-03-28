#include "StudentWorld.h"
#include <string>
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iostream>
#include <random>
#include <climits>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::randInt(int low, int high)
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(low, high); // define the range
	return distr(eng);
}

double StudentWorld::distanceBetween(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double StudentWorld::distanceBetween(const Actor& actor, double x2, double y2)
{
	return sqrt(pow(x2 - actor.getX(), 2) + pow(y2 - actor.getY(), 2));
}

void StudentWorld::removeTooClose(vector<vector<int> > &coords, int x, int y)
{
	for (vector<vector<int> >::iterator it = coords.begin(); it != coords.end(); )
	{
		if (distanceBetween(x, y, (*it)[0], (*it)[1]) <= 6.0)
		{
			it = coords.erase(it);
		}

		else ++it;
	}

}

void StudentWorld::findCoordinate(vector<vector<int> > coords, int &x, int &y)
{
	//generate coordinates and push to vector
	int coordinatePair = randInt(0, coords.size() - 1);
	x = coords[coordinatePair][0];
	y = coords[coordinatePair][1];
}

StudentWorld::~StudentWorld()
{
	//delete dirt
	for (int i = 0; i < FIELD_HOR_SIZE; i++)
	{
		for (int j = 0; j < FIELD_VERT_SIZE; j++)
		{
			delete _fieldDirt[i][j];
		}
	}
	//delete player
	delete _player;

	//delete all other actors
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end();)
	{
		delete *it;
		it = _otherActors.erase(it);
	}
}

int StudentWorld::init()
{
	
	//reset amt oil left
	_oilLeft = 0;
	_tickCounter = 0;
	_numProtesters = 0;

	//create dirt
	for (int i = 0; i < FIELD_HOR_SIZE; i++)
	{
		for (int j = 0; j < FIELD_VERT_SIZE; j++)
		{
			_fieldDirt[i][j] = new Dirt(this, i, j);
			if (i >= 30 && i <= 33 && j >= 4)
				removeDirt(i, j, false);
		}
	}

	//stores possible	coordinates of created boulders and interactable objects
	vector<vector<int> > coords;
	for (int i = 0; i < FIELD_HOR_SIZE - 4; i++)
		for (int j = 0; j < FIELD_VERT_SIZE - 4; j++)
			if (!(i >= 26 && i <= 33 && j >= 4))
				coords.push_back({i, j});

	//calculate number of boulders and interactable objects
	int numBoulders = min((_level / 2) + 2, 6);
	int numGold = max(5 - (_level / 2), 2);
	int numOilBarrels = min(2 + _level, 20);
	

	

	//create and distribute boulders
	for (int i = 0; i < numBoulders; i++)
	{
		//find coordinate and remove those too close as candidates
		int x, y;
		findCoordinate(coords, x, y);
		removeTooClose(coords, x, y);

		//create boulders and remove surrounding dirt without playing sound
		_otherActors.push_back(new Boulder(this, x, y));
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				removeDirt(x + i, y + j, false);
	}

	//create and distribute gold
	for (int i = 0; i < numGold; i++)
	{
		//find coordinate and remove those too close as candidates
		int x, y;
		findCoordinate(coords, x, y);
		removeTooClose(coords, x, y);

		//create gold object
		_otherActors.push_back(new Gold(this, x, y));
	}

	//create and distribute oil
	for (int i = 0; i < numOilBarrels; i++)
	{
		//find coordinate and remove those too close as candidates
		int x, y;
		findCoordinate(coords, x, y);
		removeTooClose(coords, x, y);
		
		//create oil barrels
		_oilLeft++;
		_otherActors.push_back(new Oil(this, x, y));
	}
	
	//create player object
	_player = new FrackMan(this);

	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	//set game text
	stringstream oss;
	oss << "Scr: " << getScore()
		<< " Lvl: " << getLevel()
		<< " Hlth: " << (int)((double)_player->getHP() / (double)FRACKMAN_HP * (double)100)
		<< "% Wtr: " << _player->getSquirtCount()
		<< " Gld: " << _player->getGold()
		<< " Sonar: " << _player->getSonar()
		<< " Oil Left: " << _oilLeft;
	setGameStatText(oss.str());
	
	generatePlayerMap();
	generateExitMap();

	//calculate number of protesters & delay between protesters appearing
	int maxProtesters = (int)min(15.0, 2 + _level*1.5);
	int protesterDelay = max(25, 200 - _level);

	//Add new actors as necessary
	createSonarOrWater();
	if (_tickCounter % protesterDelay == 0)
		addProtesters(maxProtesters);
	_tickCounter++;

	//make every actor doSomething() if it is alive, else remove it from the map
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end();)
	{
		if ((*it)->isAlive())
		{
			(*it)->doSomething();
			++it;
		}
		else
		{
			delete (*it);
			it = _otherActors.erase(it);
		}
	}
	_player->doSomething();

	//if player completely annoyed, lose life
	if (_player->getHP() <= 0)	
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (_oilLeft == 0)
	{
		_level++;
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
	//delete dirt
	for (int i = 0; i < FIELD_HOR_SIZE; i++)
	{
		for (int j = 0; j < FIELD_VERT_SIZE; j++)
		{
			delete _fieldDirt[i][j];
		}
	}
	//delete player
	delete _player;

	//delete all other actors
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end();)
	{
		delete *it;
		it = _otherActors.erase(it);
	}
}

int StudentWorld::getLevel()
{
	return _level;
}

int StudentWorld::getTicks()
{
	return _tickCounter;
}

void StudentWorld::removeDirt(int x, int y, bool sound)
{	
	if(_fieldDirt[x][y] && sound)
		playSound(SOUND_DIG);
	delete _fieldDirt[x][y];
	_fieldDirt[x][y] = nullptr;
}

FrackMan * StudentWorld::getPlayer()
{
	return _player;
}

//returns true if (x,y) contains dirt
bool StudentWorld::isDirt(int x, int y)
{
	if (y >= 60) return false;
	if (_fieldDirt[x][y] == nullptr)
		return false;
	else return true;
}

bool StudentWorld::containsDirt(int x1, int y1, int x2, int y2)
{
	//make sure not checking outside bounds of array
	if (y2 >= 60) y2 = 60;
	if (y1 >= 60) y1 = 60;

	for (int i = 0; i < x2 - x1; i++)
		for (int j = 0; j < y2 - y1; j++)
			if (isDirt(i + x1, j + y1))
				return true;
	return false;
}

bool StudentWorld::containsBoulderAtDistance(int x1, int y1, int x2, int y2, double distance)
{
	for (int i = 0; i < x2 - x1; i++)
		for (int j = 0; j < y2 - y1;j++)
			for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end(); ++it)
			{
				if ((*it)->getType() == boulder)
				{
					if (distanceBetween((*(*it)), i + x1, j + y1) <= distance)
						return true;
				}
			}
	return false;
}

bool StudentWorld::overlapsBoulder(int x, int y, int size)
{
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end(); ++it)
	{
		if ((*it)->getType() == boulder)
		{
			int boulderX = (*it)->getX();
			int boulderY = (*it)->getY();
			//hyperplane separation theorem in R2
			if ((x + size - 1 < boulderX) || (boulderX + size - 1 < x) || (y + size - 1 < boulderY) || (boulderY + size - 1 < y))
				continue;
			else return true;
		}
	}
	return false;
}

void StudentWorld::nearProtester(int x, int y, int dmg, Actor* caller)
{
	int numAnnoyed = 0;
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end(); ++it)
	{
		if ((*it)->getType() == hostile)
		{
			if ((*it)->getLeaving())
				continue;
			if (distanceBetween((*(*it)), x, y) <= 3.0)
			{
				numAnnoyed++;
			}
		}
	}
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end(); ++it)
	{
		if ((*it)->getType() == hostile || (*it)->getType() == hostileHC)
		{
			if ((*it)->getLeaving())
				continue;
			if (distanceBetween((*(*it)), x, y) <= 3.0)
			{
				(*it)->annoy(dmg, numAnnoyed);
				if ((*it)->getHP() <= 0)
				{
					if (caller->getType() == boulder)
						increaseScore(500);
					else if (caller->getType() == squirt)
						if((*it)->getType() == hostileHC)
							increaseScore(250);
						else increaseScore(100);
				}
			}
		}
	}
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end(); ++it)
	{
		if ((*it)->getType() == squirt)
		{
			(*it)->numToAnnoy(0);
		}
	}
}

bool StudentWorld::nearNugget(int x, int y)
{
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end(); ++it)
	{
		if ((*it)->getType() == goldBribe)
		{
			if (distanceBetween((*(*it)), x, y) <= 3.0)
			{
				(*it)->killThis();
				increaseScore(25);
				playSound(SOUND_PROTESTER_FOUND_GOLD);
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::nearBoulder(int x, int y)
{
	for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end(); ++it)
	{
		if ((*it)->getType() == boulder)
		{
			if (distanceBetween((*(*it)), x, y) <= 3.0)
			{
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::decOil()
{
	_oilLeft--;
}

void StudentWorld::playerDropGold()
{
	_otherActors.push_back(new Gold(this, _player->getX(), _player->getY(), true, protestor));
	_player->decGold();
}

void StudentWorld::playerSquirt(int x, int y)
{
	playSound(SOUND_PLAYER_SQUIRT);
	
	//use 1 squirt
	_player->decSquirt();

	if (x > 60 || x < 0 || y > 60 || y < 4)
		return;

	if (containsDirt(x, y, x + 4, y + 4) || containsBoulderAtDistance(x, y, x + 1, y + 1, 3.0))
		return;

	_otherActors.push_back(new Squirt(this, x, y, _player->getDirection()));
}

void StudentWorld::activateSonar(int x, int y)
{
	if (_player->getSonar())
	{
		_player->decSonar();
		for (list<Actor*>::iterator it = _otherActors.begin(); it != _otherActors.end(); ++it)
		{
			if (distanceBetween(x, y, (*it)->getX(), (*it)->getY()) <= 12.0)
				(*it)->setVisible(true);
		}
	}
}

void StudentWorld::createSonarOrWater()
{
	int chanceNewGoodieDenom = getLevel() * 25 + 300;
	if(randInt(1,chanceNewGoodieDenom) == 1)
	{
		int chanceSonar = 1;
		int chanceWater = 4;
		if (randInt(1, chanceSonar + chanceWater) <= chanceSonar)
			_otherActors.push_back(new Sonar(this, 0, FIELD_VERT_SIZE));
		else
		{
			int x, y;
			//populate vector with all coordinates in oil field without dirt that form an empty 4x4 area
			vector<vector<int> > coords;
			for (int i = 0; i < FIELD_HOR_SIZE - 4; i++)
				for (int j = 0; j < FIELD_VERT_SIZE - 4; j++)
				{
					bool pushThis = true;
					for (int m = 0; m < DEFAULTSIZE * 4; m++)
						for (int n = 0; n < DEFAULTSIZE * 4; n++)
							if (isDirt(i + m, j + n))
								pushThis = false;
					if (pushThis)
						coords.push_back({ i,j });
				}
			findCoordinate(coords, x, y);
			_otherActors.push_back(new Water(this, x, y));
		}
	}
}

int StudentWorld::numObjects()
{
	return _otherActors.size();
}

void StudentWorld::addProtesters(int target)
{
	if (_numProtesters < target)
	{
		int probHardcore = min(90, _level * 10 + 30);
		if (randInt(1, 100) < probHardcore)
		{
			_otherActors.push_back(new HardcoreProtester(this, FIELD_HOR_SIZE - 4, FIELD_VERT_SIZE));
		}
		else
			_otherActors.push_back(new Protester(this, FIELD_HOR_SIZE - 4, FIELD_VERT_SIZE));
		_numProtesters++;
	}
	else return;
}

void StudentWorld::floodFill(int x, int y, int map[66][66])
{
	//mark all points as impassable (-2) or undiscovered (-1)
	for (int i = 0; i < FIELD_HOR_SIZE + 2; i++)
		for (int j = 0; j < FIELD_VERT_SIZE + 4 + 2; j++)
		{
			if (j == 0 || j > 64)
				map[i][j] = -2;
			else if (i == 0 || i > 64)
				map[i][j] = -2;
			else if (containsDirt(i - 1, j - 1, i + 4 - 1, j + 4 - 1) || overlapsBoulder(i-1, j-1, DEFAULTSIZE * 4))
				map[i][j] = -2;
			else map[i][j] = -1;
		}

	//Queue based flood fill using lee algorithm
	queue<Coord> Q;
	Q.push(Coord(x, y));
	map[x + 1][y + 1] = 0;

	while (Q.size() != 0)
	{
		Coord current = Q.front();
		Q.pop();
		int r = current.r() + 1;
		int c = current.c() + 1;
		//Check all adjacent

		if (map[r - 1][c] == -1)
		{
			Q.push(Coord(r - 1 - 1, c - 1));
			map[r - 1][c] = map[r][c] + 1;
		}
		if (map[r + 1][c] == -1)
		{
			Q.push(Coord(r + 1 - 1, c - 1));
			map[r + 1][c] = map[r][c] + 1;
		}
		if (map[r][c + 1] == -1)
		{
			Q.push(Coord(r - 1, c + 1 - 1));
			map[r][c + 1] = map[r][c] + 1;
		}
		if (map[r][c - 1] == -1)
		{
			Q.push(Coord(r - 1, c - 1 - 1));
			map[r][c - 1] = map[r][c] + 1;
		}
	}
}

void StudentWorld::printFloodFillExit()
{
	for (int i = 65; i >= 0; i--)
	{
		for (int j = 0; j < 66; j++)
		{
			if (_exitMap[j][i] == -2)
				cout << '#';
			else if (_exitMap[j][i] < 10)
				cout << _exitMap[j][i];
			else if (_exitMap[j][i] < 36)
				cout << (char)(_exitMap[j][i] - 10 + 'a');
			else if (_exitMap[j][i] < 62)
				cout << (char)(_exitMap[j][i] - 36  + 'A');
			else cout << '?';
		}
		cout << endl;
	}
}

void StudentWorld::generateExitMap()
{
	floodFill(60, 60, _exitMap);
}

void StudentWorld::generatePlayerMap()
{
	floodFill(_player->getX(), _player->getY(), _playerMap);
}

void StudentWorld::getClosest(int x, int y, int &dir, int map[66][66])
{
	//adjust for map grid
	int X = x + 1;
	int Y = y + 1;
	int up, left, right, down;
	up = down = left = right = -2;
	//check all directions and determine cost of travel
	if (map[X][Y + 1] < map[X][Y] && map[X][Y + 1] != -2)
		up = map[X][Y + 1];
	else up = INT_MAX;
	if (map[X][Y - 1] < map[X][Y] && map[X][Y - 1] != -2)
		down = map[X][Y - 1];
	else down = INT_MAX;
	if (map[X+1][Y] < map[X][Y] && map[X + 1][Y] != -2)
		right = map[X + 1][Y];
	else right = INT_MAX;
	if (map[X - 1][Y] < map[X][Y] && map[X - 1][Y] != -2)
		left = map[X - 1][Y];
	else left = INT_MAX;
	//find closest point to target
	int leastDist = min(up, min(down, min(left, right)));

	//set resultX and resultY to grid position of closest point
	if (leastDist == right)
	{
		dir = RIGHT;
		return;
	}
	if (leastDist == left)
	{
		dir = LEFT;
		return;
	}
	if (leastDist == up)
	{
		dir = UP;
		return;
	}
	if (leastDist == down)
	{

		dir = DOWN;
		return;
	}

}

void StudentWorld::getClosestToPlayer(int x, int y, int &dir)
{
	getClosest(x, y, dir, _playerMap);
}

void StudentWorld::getClosestToExit(int x, int y, int &dir)
{
	getClosest(x, y, dir, _exitMap);
}

int StudentWorld::distanceToPlayer(int x, int y)
{
	return _playerMap[x + 1][y + 1];
}
