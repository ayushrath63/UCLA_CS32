#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>
#include <vector>

#define FIELD_VERT_SIZE 60
#define FIELD_HOR_SIZE 64
#define RIGHT 1
#define LEFT 2
#define UP 3
#define DOWN 4

class Dirt;
class FrackMan;
class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
		_level = 0;
		_tickCounter = 0;
	}
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	int randInt(int low, int high);
	int getLevel();
	int getTicks();
	void removeDirt(int x, int y, bool sound);
	FrackMan* getPlayer();
	bool isDirt(int x, int y);
	bool containsDirt(int x1, int y1, int x2, int y2);
	bool containsBoulderAtDistance(int x1, int y1, int x2, int y2, double distance);
	bool overlapsBoulder(int x, int y, int size);
	void nearProtester(int x, int y, int dmg, Actor* caller);
	bool nearNugget(int x, int y);
	bool nearBoulder(int x, int y);
	void decOil();
	void playerDropGold();
	void playerSquirt(int x, int y);
	void activateSonar(int x, int y);
	void createSonarOrWater();
	int numObjects();
	void floodFill(int x, int y, int  map[66][66]);
	void generateExitMap();
	void generatePlayerMap();
	void getClosest(int x, int y, int &dir, int map[66][66]);
	void getClosestToPlayer(int x, int y, int &dir);
	void getClosestToExit(int x, int y, int &dir);
	int distanceToPlayer(int x, int y);
	void printFloodFillExit(); //Used for Debugging Only
protected:
	double distanceBetween(double x1, double y1, double x2, double y2);
	double distanceBetween(const Actor& actor, double x2, double y2);
	void removeTooClose(std::vector<std::vector<int> > &coords, int x, int y);
	void findCoordinate(std::vector<std::vector<int> > coords, int &x, int &y);
	void addProtesters(int target);
private:
	int _level;
	int _oilLeft;
	int _tickCounter;
	int _numProtesters;
	Dirt* _fieldDirt[FIELD_HOR_SIZE][FIELD_VERT_SIZE];
	FrackMan* _player;
	std::list<Actor*> _otherActors;
	int _playerMap[FIELD_HOR_SIZE + 2][FIELD_VERT_SIZE + 4 + 2];
	int _exitMap[FIELD_HOR_SIZE + 2][FIELD_VERT_SIZE + 4 + 2];
	class Coord
	{
	public:
		Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
		bool operator==(const Coord& a) { return a.r() == this->r() && a.c() == this->c(); }
		int r() const { return m_r; }
		int c() const { return m_c; }
	private:
		int m_r;
		int m_c;
	};
};

#endif // STUDENTWORLD_H_