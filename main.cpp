#include <iostream>
#include "GraphicHandler.h"
#include "Generation.h"
#include "Game.h"

using namespace std;


//Processes the Graphical Interface of the neural network and the game
void processGraphicHandler(
	GraphicHandler &graphicHandler,
	Organism &organism,
	Game &game)
{
	graphicHandler.printOrganism(organism);
	graphicHandler.printGame(game.getProjectiles(), game.getPlayer());
	graphicHandler.getEvents();
}


//Processes the logic of the game and returns true if the player was hit by a projectile
//Also takes an organism and feeds its output to the game
bool processGame(
	Game &game,
	Organism &organism)
{
	return game.processLogic(organism.getOutputs());
}


//Processes the neural network of the organism and feed it fitness
void processOrganism(
	Organism &organism,
	Game &game)
{
	organism.addFitness(game.returnFitnessDelta());
	organism.feedInput(game.getInput());
	organism.processNeuralNetwork();
}


//MAIN
int main(int argc, char *argv[])
{
	Generation generation;
	GraphicHandler graphicHandler;
	Game game;

	generation.getOrganismByIndex(0).printInfo();
	int c;
	cin >> c;
	generation.getOrganismByIndex(1).printInfo();

	Organism testChild = generation.breedNewOrganism(generation.getOrganismByIndex(0), generation.getOrganismByIndex(1));
	cin >> c;
	testChild.printInfo();
	cin >> c;

/*	for (vector<Organism>::iterator it = generation.getOrganisms().begin(); it != generation.getOrganisms().end(); ++it) 
	{
		while (!processGame(game, *it))
		{
			processGraphicHandler(graphicHandler, *it, game);
			if (game.printToTerminal()) { it->printInfo(); }
			processOrganism(*it, game);
		}
		game.cleanGame();
	}*/
	return 0;
}