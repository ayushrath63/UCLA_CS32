#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <list>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

////////////////////////////////////////
//    Actor 						  //
////////////////////////////////////////
enum childType { dirt, boulder, squirt, oil, gold, goldBribe, water, sonar, player, hostile, hostileHC};
#define DEFAULTSIZE 1
#define RIGHT 1
#define LEFT 2
#define UP 3
#define DOWN 4

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0);
	virtual ~Actor();
	virtual void doSomething();
	StudentWorld* getWorld();
	void killThis();
	bool isAlive();
	virtual bool canBeMovedTo(int x, int y);
	virtual childType getType() = 0;
	virtual void annoy(int k, int &count);
	virtual void numToAnnoy(int k);
	virtual bool getLeaving();
	virtual void setLeaving(bool i);
	virtual int getHP() const;
	double distanceTo(Actor* other);
protected:
	bool intInBound(int r)
	{ return r >= 0 && r < 64; }
private:
	bool _alive;
	StudentWorld* _world;
};


////////////////////////////////////////
//    Terrain Objects				  //
////////////////////////////////////////

class TerrainObject : public Actor
{
public:
	TerrainObject(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
	virtual ~TerrainObject();
private:
};


#define DIRTSIZE 0.25
#define DIRTDEPTH 3
class Dirt : public TerrainObject
{
public:
	Dirt(StudentWorld* world, int startX, int startY);
	virtual ~Dirt();
	virtual childType getType();
private:
};

#define BOULDERSIZE 1
#define BOULDERDEPTH 1
enum BoulderState { stable, waiting, falling };
class Boulder : public TerrainObject
{
public:
	Boulder(StudentWorld* world, int startX, int startY);
	virtual ~Boulder();
	BoulderState getState() const;
	void setState(BoulderState state);
	bool canFall();
	virtual void doSomething();
	virtual childType getType();
private:
	BoulderState _state;
	int _waitTime;
};

////////////////////////////////////////
//   Interactables					  //
////////////////////////////////////////

enum InteractableBy {protestor, frackman};
class Interactable : public Actor
{
public:
	Interactable(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
	virtual ~Interactable();
	void setInteractable(InteractableBy agentType);
	InteractableBy getInteractable() const;
	virtual void numToAnnoy(int k);
	void setFound(bool found);
	bool getFound();
	void setLifeTime(int time);
	void decLifeTime();
	int getLifeTime();
private:
	InteractableBy _interactableBy;
	bool _found;
	int _lifeTime;
};

#define GOLDSIZE 1.0
#define GOLDDEPTH 2
class Gold : public Interactable
{
public:
	Gold(StudentWorld* world, int startX, int startY);
	Gold(StudentWorld* world, int startX, int startY, bool visible, InteractableBy agentType);
	virtual ~Gold();
	virtual void doSomething();
	virtual childType getType();
};

#define SONARSIZE 1.0
#define SONARDEPTH 2
class Sonar : public Interactable
{
public:
	Sonar(StudentWorld* world, int startX, int startY);
	virtual ~Sonar();
	virtual void doSomething();
	virtual childType getType();
};

#define OILSIZE 1.0
#define OILDEPTH 2
class Oil : public Interactable
{
public:
	Oil(StudentWorld* world, int startX, int startY);
	virtual ~Oil();
	virtual void doSomething();
	virtual childType getType();
};

#define WATERSIZE 1.0
#define WATERDEPTH 2
class Water : public Interactable
{
public:
	Water(StudentWorld* world, int startX, int startY);
	virtual ~Water();
	virtual void doSomething();
	virtual childType getType();
};

#define SQUIRTSIZE 1.0
#define SQUIRTDEPTH 1
#define SQUIRT_DIST 4
class Squirt : public Interactable
{
public:
	Squirt(StudentWorld* world, int startX, int startY, Direction dir);
	virtual ~Squirt();
	virtual void doSomething();
	virtual childType getType();
	virtual bool canBeMovedTo(int x, int y);
	void incDist();
	virtual void numToAnnoy(int k);
private:
	int _distTraveled;
	int _numToAnnoy;
};

////////////////////////////////////////
//   Agents							  //
////////////////////////////////////////

class Agent : public Actor
{
public:
	Agent(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
	virtual ~Agent();
	virtual int getHP() const;
	void setHP(int hp);
	virtual void decHPBy(int k);
private:
	int _hp;
};

#define PROTESTERSIZE 1
#define PROTESTERDEPTH 0
#define PROTESTER_HP 5
enum ProtestState {rest, active};
class Protester : public Agent
{
private:
	int _numSquaresToMoveCurrentDir;
	int _restTicks;
	bool _leavingField;
	virtual void annoy(int k, int &count);
	ProtestState _activity;
	bool _canShout;
	int _timeOfLastShout;
	int _lastPerpTurn;
	int _stunTime;
	bool _stunned;
public:
	Protester(StudentWorld* world, int startX, int startY, bool isHardcore = false);
	virtual ~Protester();
	virtual childType getType();
	virtual void doSomething();
	virtual bool hardCoreMovement();
	virtual bool canBeMovedTo(int x, int y);
	virtual void getBribed();
	ProtestState getState();
	bool lineOfSightToPlayer();
	void stepRight();
	void stepLeft();
	void stepDown();
	void stepUp();
	void stepCurrentDir();
	bool canMoveUp();
	bool canMoveDown();
	bool canMoveLeft();
	bool canMoveRight();
	void setLeaving(bool i);
	bool getLeaving();
	void moveTowardsExit();
	void moveTowardsPlayer();
	void setStun();
};

class HardcoreProtester : public Protester
{
public:
	HardcoreProtester(StudentWorld* world, int startX, int startY);
	virtual ~HardcoreProtester();
	virtual bool hardCoreMovement();
	virtual void getBribed();
	virtual childType getType();
private:
};

#define FRACKMANSTARTX 30
#define FRACKMANSTARTY 60
#define FRACKMANSIZE 1.0
#define FRACKMANDEPTH 0
#define FRACKMAN_HP 10
class FrackMan : public Agent
{
public:
	FrackMan(StudentWorld* world, int hp = FRACKMAN_HP ,int water = 5,int sonar = 1, int gold = 0);
	virtual ~FrackMan();
	virtual void doSomething();
	virtual bool canBeMovedTo(int x, int y);
	virtual childType getType();
	void incGold();
	void decGold();
	int getGold();
	void decSquirt();
	void incSquirtBy(int k);
	int getSquirtCount();
	void incSonarBy(int k);
	void decSonar();
	int getSonar();
private:
	int _squirtCount;
	int _sonarCount;
	int _gold;
};
#endif // ACTOR_H_
