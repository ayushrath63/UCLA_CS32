#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
#include <list>
#include <cmath>
#include <queue>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
/*
You must turn in your source code for the simple version of your game, which builds
without errors under either Visual Studio or Xcode. You do not have to get it to run
under more than one compiler. You will turn in a zip file containing nothing more than
these four files:

actor.h // contains base, Star and player class declarations
		// as well as constants required by these classes
actor.cpp // contains the implementation of these classes

StudentWorld.h // contains your StudentWorld class declaration

StudentWorld.cpp // contains your StudentWorld class implementation

You will not be turning in any other files – we’ll test your code with our versions of the
the other .cpp and .h files. Therefore, your solution must NOT modify any of our files or
you will receive zero credit!
*/

////////////////////////////////////////
//    Actor 						  //
////////////////////////////////////////
Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	setVisible(true);
	_world = world;
}

Actor::~Actor()
{}

void Actor::doSomething()
{
}

StudentWorld* Actor::getWorld()
{
	return _world;
}

void Actor::killThis()
{
	_alive = false;
}

bool Actor::isAlive()
{
	return _alive;
}

bool Actor::canBeMovedTo(int x, int y)
{
	if (x >= 0 && x <= 60 && y >= 0 && y <= 60)
	{
		if (_world->isDirt(x, y))
			return false;
		if (getWorld()->overlapsBoulder(x, y, DEFAULTSIZE * 4))
			return false;
		return true;
	}
	else return false;
}

void Actor::annoy(int k, int & count)
{
}

void Actor::numToAnnoy(int k)
{
}

bool Actor::getLeaving()
{
	return false;
}

void Actor::setLeaving(bool i)
{
}

int Actor::getHP() const
{
	return 2000000000;
}

double Actor::distanceTo(Actor * other)
{
	return sqrt(pow(getX() - other->getX(), 2.0) + pow(getY() - other->getY(), 2.0));
}

////////////////////////////////////////
//    Terain Objects				  //
////////////////////////////////////////

TerrainObject::TerrainObject(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
	: Actor(world,imageID, startX, startY, dir, size, depth)
{}

TerrainObject::~TerrainObject()
{
}

Dirt::Dirt(StudentWorld* world, int startX, int startY)
	: TerrainObject(world, IID_DIRT, startX, startY, right, DIRTSIZE, DIRTDEPTH)
{
}

Dirt::~Dirt()
{
}

childType Dirt::getType()
{
	return dirt;
}

Boulder::Boulder(StudentWorld* world, int startX, int startY)
	: TerrainObject(world, IID_BOULDER, startX, startY, down, BOULDERSIZE, BOULDERDEPTH)
{
	_waitTime = 30;
	setState(stable);
}

Boulder::~Boulder()
{
}

BoulderState Boulder::getState() const
{
	return _state;
}

void Boulder::setState(BoulderState state)
{
	_state = state;
}

//returns false if any of the 4 points directly below the boulder cannot be moved to
bool Boulder::canFall()
{
	int numFree = 4;
	for (int i = 0; i < 4; i++)
		if ((getWorld()->isDirt(getX() + i, getY() - 1)))
			numFree--;
	if(numFree == 4)
		return true;
	else return false;
}

childType Boulder::getType()
{
	return boulder;
}

void Boulder::doSomething()
{
	//set the state to waiting if the boulder is stable and can fall
	if (getState() == stable && canFall())
		setState(waiting);

	//decrement wait timer while waiting
	if (getState() == waiting)
	{
		if (_waitTime == 0)
		{
			setState(falling);
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		_waitTime--;
	}

	//if falling, fall if space below is free
	if (getState() == falling)
	{
		if (distanceTo(getWorld()->getPlayer()) <= 3.0)
		{
			getWorld()->getPlayer()->decHPBy(100);
		}
		int curNumObj = getWorld()->numObjects();
		getWorld()->nearProtester(getX(), getY(), 100, this);
		if (canFall())
			moveTo(getX(), getY() - 1);
		else
			killThis();
	}
}

////////////////////////////////////////
//   Interactables					  //
////////////////////////////////////////
Interactable::Interactable(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
	:Actor(world, imageID, startX, startY, dir, size, depth)
{
	_found = false;
}

Interactable::~Interactable()
{}

void Interactable::setInteractable(InteractableBy agentType)
{
	_interactableBy = agentType;
}

InteractableBy Interactable::getInteractable() const
{
	return _interactableBy;
}

void Interactable::numToAnnoy(int k)
{
}

void Interactable::setFound(bool found)
{
	_found = found;
}

bool Interactable::getFound()
{
	return _found;
}

void Interactable::setLifeTime(int time)
{
	_lifeTime = time;
}

void Interactable::decLifeTime()
{
	_lifeTime--;
}

int Interactable::getLifeTime()
{
	return _lifeTime;
}

Gold::Gold(StudentWorld * world, int startX, int startY)
	:Interactable(world, IID_GOLD, startX, startY, right, GOLDSIZE, GOLDDEPTH)
{
	setVisible(false);
	setInteractable(frackman);
}

Gold::Gold(StudentWorld* world, int startX, int startY, bool visible, InteractableBy agentType)
	:Interactable(world, IID_GOLD, startX, startY, right, GOLDSIZE, GOLDDEPTH)
{
	setInteractable(agentType);
	if (visible && agentType == protestor)
	{
		setVisible(true);
		setLifeTime(100);
	}
}

Gold::~Gold()
{
}

void Gold::doSomething()
{
	if (!isAlive())
		return;


	//ADD BRIBE STUFF
	if (getInteractable() == protestor)
	{
		if (getLifeTime() == 0)
			killThis();
		else
			decLifeTime();
		return;
	}

	if (distanceTo(getWorld()->getPlayer()) <= 4.0 && getInteractable() == frackman && !getFound())
	{
		setVisible(true);
		setFound(true);
		return;
	}
	if (distanceTo(getWorld()->getPlayer()) <= 3.0 && getInteractable() == frackman)
	{
		killThis();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->getPlayer()->incGold();
		getWorld()->increaseScore(10);
	}
}

childType Gold::getType()
{
	if (getInteractable() == protestor)
		return goldBribe;
	return gold;
}

Sonar::Sonar(StudentWorld* world, int startX, int startY)
	:Interactable(world, IID_SONAR, startX, startY, right, SONARSIZE, SONARDEPTH)
{
	setLifeTime(max(100, 300 - (10 * getWorld()->getLevel())));
}

Sonar::~Sonar()
{
}

void Sonar::doSomething()
{
	if (!isAlive())
		return;

	if (getLifeTime() <= 0)
	{
		killThis();
		return;
	}

	decLifeTime();

	if (distanceTo(getWorld()->getPlayer()) <= 3.0)
	{
		killThis();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(75);
		getWorld()->getPlayer()->incSonarBy(2);
	}
}

childType Sonar::getType()
{
	return sonar;
}

Oil::Oil(StudentWorld* world, int startX, int startY)
	:Interactable(world, IID_BARREL, startX, startY, right, OILSIZE, OILDEPTH)
{
	setVisible(false);
}

Oil::~Oil()
{
}

void Oil::doSomething()
{
	if (!isAlive())
		return;

	if (distanceTo(getWorld()->getPlayer()) <= 4.0 && !getFound())
	{
		setVisible(true);
		setFound(true);
		return;
	}
	if (distanceTo(getWorld()->getPlayer()) <= 3.0)
	{
		killThis();
		getWorld()->decOil();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
	}
}

childType Oil::getType()
{
	return oil;
}

Water::Water(StudentWorld* world, int startX, int startY)
	:Interactable(world, IID_WATER_POOL, startX, startY, right, WATERSIZE, WATERDEPTH)
{
	setLifeTime(max(100, 300 - (10 * getWorld()->getLevel())));
}

Water::~Water()
{
}

void Water::doSomething()
{
	if (!isAlive())
		return;

	if (getLifeTime() <= 0)
	{
		killThis();
		return;
	}

	decLifeTime();

	if (distanceTo(getWorld()->getPlayer()) <= 3.0)
	{
		killThis();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(100);
		getWorld()->getPlayer()->incSquirtBy(5);
	}
}

childType Water::getType()
{
	return water;
}

Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction dir)
	:Interactable(world, IID_WATER_SPURT, startX, startY, dir, SQUIRTSIZE, SQUIRTDEPTH)
{
	_distTraveled = 0;
}

Squirt::~Squirt()
{
}

bool Squirt::canBeMovedTo(int x, int y)
{
	if (x >= 0 && x <= 60 && y >= 60 && y <= 64)
		return true;

	for (int i = 0; i < SQUIRTSIZE * 4; i++)
		for (int j = 0; j < SQUIRTSIZE * 4; j++)
			if (getWorld()->isDirt(x+i, y+j))
				return false;
	if (getWorld()->overlapsBoulder(x, y, DEFAULTSIZE * 4))
		return false;
	return true;
}

void Squirt::doSomething()
{

	if (_distTraveled == SQUIRT_DIST || _numToAnnoy == 0)
	{
		killThis();
		return;
	}

	getWorld()->nearProtester(getX(), getY(), 2, this);
	switch (getDirection())
	{
	case up:
		if (canBeMovedTo(getX(), getY() + 1))
		{
			moveTo(getX(), getY() + 1);
			_distTraveled++;
		}
		else killThis();
		break;
	case right:
		if (Squirt::canBeMovedTo(getX() + 1, getY()))
		{
			moveTo(getX() + 1, getY());
			_distTraveled++;
		}
		else killThis();
		break;
	case left:
		if (canBeMovedTo(getX() - 1, getY()))
		{
			moveTo(getX() - 1, getY());
			_distTraveled++;
		}
		else killThis();
		break;
	case down:
		if (canBeMovedTo(getX(), getY() - 1))
		{
			moveTo(getX(), getY() - 1);
			_distTraveled++;
		}
		else killThis();
		break;
	}
}

childType Squirt::getType()
{
	return squirt;
}

void Squirt::incDist()
{
	_distTraveled++;
}

void Squirt::numToAnnoy(int k)
{
	_numToAnnoy = k;
}

////////////////////////////////////////
//    Agents						  //
////////////////////////////////////////

Agent::Agent(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
	: Actor(world, imageID, startX, startY, dir, size, depth)
{
}

Agent::~Agent()
{
}

FrackMan::FrackMan(StudentWorld* world, int hp,int water,int sonar,int gold)
	: Agent(world, IID_PLAYER, FRACKMANSTARTX, FRACKMANSTARTY, right, FRACKMANSIZE, FRACKMANDEPTH)
{
	_squirtCount	= water;
	_sonarCount		= sonar;
	_gold = gold;
	setHP(hp);
}

FrackMan::~FrackMan()
{
}

int Agent::getHP() const
{
	return _hp;
}

void Agent::setHP(int hp)
{
	_hp = hp;
}

void Agent::decHPBy(int k)
{
	_hp -= k;
}

//FrackMan

bool FrackMan::canBeMovedTo(int x, int y)
{
	if (x >= 0 && x <= 60 && y >= 0 && y <= 60)
	{ 
		if (getWorld()->containsBoulderAtDistance(x, y, x+1, y+1, 3.0))
			return false;
		return true;
	}
	else return false;
}

childType FrackMan::getType()
{
	return player;
}

void FrackMan::doSomething()
{
	//Check lives, return if dead
	if (getWorld()->getLives() == 0)
		return;

	//if no HP left, kill self and return
	if (getHP() <= 0)
	{
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
		killThis();
		return;
	}

	//remove dirt from 4x4 image area (x->x+3,y->y+3) and play sound
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			//if FrackMan overlaps the dirt grid
			if ((i + getX()) >= 0 && (i + getX()) < 64 && (j + getY()) >= 0 && (j + getY()) < 60)
				getWorld()->removeDirt((i + getX()), (j + getY()), true);


	//check for keypress
	int keyPress;
	getWorld()->getKey(keyPress);
	switch (keyPress)
	{
	//if escape, abort/set self to dead
	case KEY_PRESS_ESCAPE:
		setHP(0);
		break;
	//if direction key
		//if not facing, turn to face
		//else, move if valid position (not within radius 3 of boulder; 0<=x<=60, 0<=y<=60)
	case KEY_PRESS_DOWN:
		if (getDirection() != down)
			setDirection(down);
		else if (canBeMovedTo(getX(), getY() - 1))
			moveTo(getX(), getY() - 1);
		else moveTo(getX(), getY());
		break;
	case KEY_PRESS_UP:
		if (getDirection() != up)
			setDirection(up);
		else if (canBeMovedTo(getX(), getY() + 1))
			moveTo(getX(), getY() + 1);
		else moveTo(getX(), getY());
		break;
	case KEY_PRESS_RIGHT:
		if (getDirection() != right)
			setDirection(right);
		else if (canBeMovedTo(getX() + 1, getY()))
			moveTo(getX() + 1, getY());
		else moveTo(getX(), getY());
		break;
	case KEY_PRESS_LEFT:
		if (getDirection() !=  left)
			setDirection(left);
		else if(canBeMovedTo(getX() - 1, getY()))
			moveTo(getX() - 1, getY());
		else moveTo(getX(), getY());
		break;
	//if TAB drop gold nugget with lifetime of 100 ticks at current location
	case KEY_PRESS_TAB:
		if(_gold > 0)
			getWorld()->playerDropGold();
		break;

	//if space, create squirt 4 units from player in same direction
	//play  SOUND_PLAYER_SQUIRT
	//if initial location of squirt is occupied by dirt or within radius 3.0 of boulder,
	//waste 1 unit of water, do not create squirt object on field
	case KEY_PRESS_SPACE:
		if (_squirtCount > 0)
		{
			switch (getDirection())
			{
			case up:
				getWorld()->playerSquirt(getX(), getY() + FRACKMANSIZE*4);
				break;
			case down:
				getWorld()->playerSquirt(getX(), getY() - FRACKMANSIZE * 4);
				break;
			case right:
				getWorld()->playerSquirt(getX() + FRACKMANSIZE * 4, getY());
				break;
			case left:
				getWorld()->playerSquirt(getX() - FRACKMANSIZE * 4, getY());
				break;
			}
		}
		break;

	//if 'Z' or 'z', use sonar if available to show all objects within radius of 12
	case 'Z':
	case 'z':
		//For Testing Pathfinding: getWorld()->printFloodFillExit();
		if (_sonarCount > 0)
		{
			getWorld()->activateSonar(getX(), getY());
			getWorld()->playSound(SOUND_SONAR);
		}
		break;

	
	}

	//if annoyed, decrease _hp by 2, if _hp <= 0, play SOUND_PLAYER_GIVE_UP and kill player
	return;
}

void FrackMan::incGold()
{
	_gold++;
}

void FrackMan::decGold()
{
	_gold--;
}

int FrackMan::getGold()
{
	return _gold;
}

void FrackMan::decSquirt()
{
	_squirtCount--;
}

void FrackMan::incSquirtBy(int k)
{
	_squirtCount += k;
}

int FrackMan::getSquirtCount()
{
	return _squirtCount;
}

void FrackMan::incSonarBy(int k)
{
	_sonarCount += k;
}

void FrackMan::decSonar()
{
	_sonarCount--;
}

int FrackMan::getSonar()
{
	return _sonarCount;
}

//Protesters

Protester::Protester(StudentWorld * world, int startX, int startY, bool isHardcore)
	:Agent(world, isHardcore ? IID_HARD_CORE_PROTESTER : IID_PROTESTER, startX, startY, left, PROTESTERSIZE, PROTESTERDEPTH)
{
	setHP(PROTESTER_HP);
	_restTicks = max(0, 3 - getWorld()->getLevel() / 4 );
	_leavingField = false;
	_canShout = true;
	_numSquaresToMoveCurrentDir = getWorld()->randInt(8, 61);
	_lastPerpTurn = 0;
	_stunTime = max(50, (-1* getWorld()->getLevel() * 10) + 100);
	_stunned = false;
}

Protester::~Protester()
{
}

childType Protester::getType()
{
	return hostile;
}

void Protester::annoy(int k, int &count)
{
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	decHPBy(k);
	_stunned = true;
	count = 0;
}

void Protester::doSomething()
{

	if (!isAlive())
		return;

	if (getHP() <= 0 && !_leavingField)
	{
		_leavingField = true;
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
	}

	Direction currentDir = getDirection();
	//Start to leave field if bribed
	if (getWorld()->nearNugget(getX(), getY()))
		getBribed();

	//deterimine if should be active
	if (!_stunned)
	{
		if (getWorld()->getTicks() % (_restTicks + 1) == 0)
			_activity = active;
		else _activity = rest;
	}

	else if (_stunned)
	{
		if (_stunTime > 0)
		{		
			_stunTime--;
			return;
		}
		else
		{
			_stunTime = max(50, (-1 * getWorld()->getLevel() * 10) + 100);
			_stunned = false;
		}
	}

	if (getState() == rest)
		return;

	else
	{
		if (_leavingField)
		{
			if (getX() == 60 && getY() == 60)
			{
				killThis();
				return;
			}
			else moveTowardsExit();
			return;
		}

		//Determine if can shout
		if (!_canShout && (getWorld()->getTicks() - _timeOfLastShout) >= 15)
			_canShout = true;

		//determine if facing player
		bool facingPlayer = false;
		switch(this->getDirection())
		{
		case up:
			if (getWorld()->getPlayer()->getY() - this->getY() > 0)
				facingPlayer = true;
			else facingPlayer = false;
			break;
		case down:
			if (getWorld()->getPlayer()->getY() - this->getY() < 0)
				facingPlayer = true;
			else facingPlayer = false;
			break;
		case right:
			if (getWorld()->getPlayer()->getX() - this->getX() > 0)
				facingPlayer = true;
			else facingPlayer = false;
			break;
		case left:
			if (getWorld()->getPlayer()->getX() - this->getX() < 0)
				facingPlayer = true;
			else facingPlayer = false;
			break;
		default:
			facingPlayer = false;
			break;
		}
		//protester is facing player if they are at the same x,y position
		if (this->getX() == getWorld()->getPlayer()->getX() && this->getY() == getWorld()->getPlayer()->getY())
			facingPlayer = true;

		// else, if too within 4.0 of player, facing player and can shout, shout
		if (distanceTo(getWorld()->getPlayer()) <= 4.0 && facingPlayer && _canShout)
		{
			_timeOfLastShout = getWorld()->getTicks();
			getWorld()->getPlayer()->decHPBy(2);
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			_canShout = false;
			return;
		}

		//moveTowardsPlayer() if is hardcore protester within certain range, then return
		if (hardCoreMovement())
			return;

		//else if line of sight to FrackMan is clear
		if (lineOfSightToPlayer() && distanceTo(getWorld()->getPlayer()) > 4.0)
		{
			stepCurrentDir();
			return;
		}
		
		//if out of steps, chose new direction
		if (_numSquaresToMoveCurrentDir <= 0)
		{
			bool directionChosen = false;
			while (!directionChosen)
			{
				switch (getWorld()->randInt(0, 4))
				{
				case 0: //right
					if (canMoveRight())
					{
						directionChosen = true;
						setDirection(right);
						if (currentDir == up || currentDir == down)
							_lastPerpTurn = getWorld()->getTicks();
					}
					break;
				case 1: //up
					if (canMoveUp())
					{
						directionChosen = true;
						setDirection(up);
						if (currentDir == left || currentDir == right)
							_lastPerpTurn = getWorld()->getTicks();
					}
					break;
				case 2: //left
					if (canMoveLeft())
					{
						directionChosen = true;
						setDirection(left);
						if (currentDir == up || currentDir == down)
							_lastPerpTurn = getWorld()->getTicks();
					}
					break;
				case 3: //down
					if (canMoveDown())
					{
						directionChosen = true;
						setDirection(down);
						if (currentDir == left || currentDir == right)
							_lastPerpTurn = getWorld()->getTicks();
					}
					break;
				}
			}
			_numSquaresToMoveCurrentDir = getWorld()->randInt(8, 61) - 1;
		}

		//Turn Perpendicular if haven't for 201+ ticks
		bool canTurnPerpVert = (getDirection() == left || getDirection() == right) && (canMoveUp() || canMoveDown());
		bool canTurnPerpHor = (getDirection() == up || getDirection() == down) && (canMoveLeft() || canMoveRight());
		if (getWorld()->getTicks() - _lastPerpTurn > 200)
		{
			if(canTurnPerpVert)
			{ 
				if (canMoveDown() && canMoveUp())
				{
					switch (getWorld()->randInt(0, 2))
					{
					case 0:
						setDirection(up);
						break;
					case 1:
						setDirection(down);
						break;
					}
				}
				else if (canMoveDown())
					setDirection(down);
				else if (canMoveUp())
					setDirection(up);
			}
			else if(canTurnPerpHor)
			{
				if (canMoveLeft() && canMoveRight())
				{
					switch (getWorld()->randInt(0, 2))
					{
					case 0:
						setDirection(right);
						break;
					case 1:
						setDirection(left);
						break;
					}
				}
				else if (canMoveLeft())
					setDirection(left);
				else if (canMoveRight())
					setDirection(right);
			}
			_numSquaresToMoveCurrentDir = getWorld()->randInt(8, 61);
		}

		//move in current direction if possible
		_numSquaresToMoveCurrentDir--;
		bool canMove = false;
		switch (getDirection())
		{
		case right: //right
			if (canMoveRight())
			{
				canMove = true;
			}
			break;
		case up: //up
			if (canMoveUp())
			{
				canMove = true;
			}
			break;
		case left: //left
			if (canMoveLeft())
			{
				canMove = true;
			}
			break;
		case down: //down
			if (canMoveDown())
			{
				canMove = true;
			}
			break;
		}
		if (!canMove)
			_numSquaresToMoveCurrentDir = 0;
		stepCurrentDir();
		_numSquaresToMoveCurrentDir--;
	}
}

bool Protester::hardCoreMovement()
{
	return false;
}

bool Protester::canBeMovedTo(int x, int y)
{
	if (x > 60 || y > 60 || x < 0 || y < 0)
		return false;
	if (x >= 0 && x <= 60 && y == 60)
		return true;
	if (getWorld()->containsDirt(x, y, x + PROTESTERSIZE * 4, y + PROTESTERSIZE * 4))
		return false;
	if (getWorld()->containsBoulderAtDistance(x,y,x+1,y+1,3.0))
		return false;
	return true;
}

void Protester::getBribed()
{
	setLeaving(true);
}

ProtestState Protester::getState()
{
	return _activity;
}

bool Protester::lineOfSightToPlayer()
{
	bool inLineHor = false;
	bool inLineVert = false;
	int playerY = getWorld()->getPlayer()->getY();
	int thisY = this->getY();
	int playerX = getWorld()->getPlayer()->getX();
	int thisX = this->getX();
	Direction currentDir = getDirection();

	//check if in line horizontally by checking for vertical overlap
	if (playerY  <  thisY + 4 * PROTESTERSIZE && playerY + 4 * FRACKMANSIZE > thisY)
		inLineHor = true;
	//check if in line vertically by checking for horizontal overlap
	else if (playerX  <  thisX + 4 * PROTESTERSIZE && playerX + 4 * FRACKMANSIZE > thisX)
		inLineVert = true;
	
	//return false if player not in line with protester
	if (!inLineHor && !inLineVert)
		return false;

	//check if clear path to player
	if (inLineVert)
	{
		if (getWorld()->containsDirt(thisX, min(playerY, thisY), thisX + PROTESTERSIZE * 4, max(playerY, thisY)))
			return false;
		if (getWorld()->containsBoulderAtDistance(thisX, min(playerY, thisY), thisX, max(playerY, thisY) + 1, 3.0))
			return false;
		if (getWorld()->getPlayer()->getY() > getY()) setDirection(up);
		else if (getWorld()->getPlayer()->getY() < getY()) setDirection(down);
		if (currentDir == left || currentDir == right)
			_lastPerpTurn = getWorld()->getTicks();
	}
	if (inLineHor)
	{
		if (getWorld()->containsDirt(min(playerX, thisX), thisY, max(playerX, thisX), thisY + PROTESTERSIZE * 4))
			return false;
		if (getWorld()->containsBoulderAtDistance(min(playerX, thisX), thisY, max(playerX, thisX) + 1, thisY +  1, 3.0))
			return false;
		if (getWorld()->getPlayer()->getX() > getX()) setDirection(right);
		else if (getWorld()->getPlayer()->getX() < getX()) setDirection(left);
		if (currentDir == up || currentDir == down)
			_lastPerpTurn = getWorld()->getTicks();
	}

	return true;
}

void Protester::stepRight()
{
	if (canBeMovedTo(getX() + 1, getY()))
	{
		moveTo(getX() + 1, getY());
	}
}
void Protester::stepLeft()
{
	if (canBeMovedTo(getX() - 1, getY()))
	{
		moveTo(getX() - 1, getY());
	}
}
void Protester::stepDown()
{
	if (canBeMovedTo(getX(), getY() - 1))
	{
		moveTo(getX(), getY() - 1);
	}
}
void Protester::stepUp()
{
	if (canBeMovedTo(getX(), getY() + 1))
	{
		moveTo(getX(), getY() + 1);
	}
}

void Protester::stepCurrentDir()
{
	switch (getDirection())
	{
	case right:
		stepRight();
		break;
	case left:
		stepLeft();
		break;
	case up:
		stepUp();
		break;
	case down:
		stepDown();
		break;
	}
}

bool Protester::canMoveUp()
{
	if (canBeMovedTo(getX(), getY() + 1))
		return true;
	return false;
}
bool Protester::canMoveDown()
{
	if (canBeMovedTo(getX(), getY() - 1))
		return true;
	return false;
}
bool Protester::canMoveLeft()
{
	if (canBeMovedTo(getX() - 1, getY()))
		return true;
	return false;
}
bool Protester::canMoveRight()
{
	if (canBeMovedTo(getX() + 1, getY()))
		return true;
	return false;
}

void Protester::setLeaving(bool i)
{
	_leavingField = i;
}

bool Protester::getLeaving()
{
	return _leavingField;
}

void Protester::moveTowardsExit()
{
	/*
	if (getX() < 60)
	{
		setDirection(right);
		moveTo(getX() + 1, getY());
	}
	else if (getY() < 60)
	{
		setDirection(up);
		moveTo(getX(), getY()+ 1);
	}
	*/
	int next;
	getWorld()->generateExitMap();
	getWorld()->getClosestToExit(getX(), getY(), next);
	if (next == 1 && canMoveRight())
	{
		setDirection(right);
		stepRight();
	}
	if (next == 2 && canMoveLeft())
	{
		setDirection(left);
		stepLeft();
	}
	if (next == 3 && canMoveUp())
	{
		setDirection(up);
		stepUp();
	}
	if (next == 4 && canMoveDown())
	{
		setDirection(down);
		stepDown();
	}
}

void Protester::moveTowardsPlayer()
{
	int next;
	getWorld()->getClosestToPlayer(getX(), getY(), next);

	if (next == RIGHT && canMoveRight())
	{
		setDirection(right);
		stepRight();
	}
	if (next == LEFT && canMoveLeft())
	{
		setDirection(left);
		stepLeft();
	}
	if (next == UP && canMoveUp())
	{
		setDirection(up);
		stepUp();
	}
	if (next == DOWN && canMoveDown())
	{
		setDirection(down);
		stepDown();
	}
}

void Protester::setStun()
{
	_stunned = true;
}

HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX, int startY)
	: Protester(world, startX, startY, true)
{
	setHP(20);
}

HardcoreProtester::~HardcoreProtester()
{

}

bool HardcoreProtester::hardCoreMovement()
{
	int senseTiles = 16 + getWorld()->getLevel() * 2;
	if (senseTiles < getWorld()->distanceToPlayer(getX(),getY()))
		return false;
	else
	{
		moveTowardsPlayer();
		return true;
	}
}

void HardcoreProtester::getBribed()
{
	getWorld()->increaseScore(25);
	setStun();
}

childType HardcoreProtester::getType()
{
	return hostileHC;
}
