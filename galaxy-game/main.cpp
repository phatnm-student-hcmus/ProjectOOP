#include "Game.h"

// this is main function of my game
int main(int argc, char* argv[]) {
	Game galaxy_game;
	if (galaxy_game.init() == false) 
	{
		return 1;
	}

	galaxy_game.run();
	return 0;
	
}
