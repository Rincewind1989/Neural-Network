#include <iostream>
#include "GraphicHandler.h"
#include "Generation.h"
#include "Game.h"

using namespace std;

int main(int argc, char *argv[])
{
	Generation generation;
	GraphicHandler grahicHandler;
	Game game;
	for (vector<Organism>::iterator it = generation.getOrganisms().begin(); it != generation.getOrganisms().end(); ++it) 
	{
		while (true)
		{
			game.processLogic(it->getOutputs());
			grahicHandler.printOrganism((*it));
			grahicHandler.printGame(game.getProjectiles(), game.getPlayer());
		}

	}
	return 0;
}

void runSimulation()
{

}