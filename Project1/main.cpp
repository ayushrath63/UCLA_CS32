#include "Game.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
	// Create a game
	// Use this instead to create a mini-game:  
	Game g(3, 4, 4);
	//Game g(7, 8, 25);

	// Play the game
	g.play();
}