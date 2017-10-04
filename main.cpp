#include "Functions.h"	//Header for functions


/*MAIN*/
int main(int argc, char *argv[]) {
	//Creating the windows to display the networks and the game
	sf::RenderWindow neuralNetworkWindow(sf::VideoMode(WIDTH_NETWORK, HEIGHT_NETWORK), "Neural Network of species");
	sf::RenderWindow gameWindow(sf::VideoMode(WIDTH_GAME, HEIGHT_GAME), "Game");
	closeWindow(neuralNetworkWindow, gameWindow);
	//Creating first generation
	vector<Species> generation = createFirstGeneration();
	//Running the simulation
	simulation(neuralNetworkWindow, gameWindow, generation);
}
/*MAIN END*/

