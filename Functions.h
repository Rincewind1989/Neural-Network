#pragma once

#include <time.h>
#include <Windows.h>
#include "Species.h"
#include "GameData.h"	//Header for the game data

/*FORWARD DECLARATIONS*/

//Create the first generation of networks
vector<Species> createFirstGeneration();	

//Closes the windows if window mode is deactivated
void closeWindow(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow); 

//Initializes the player
playerStruct initPlayer(); 

//Processes the velocities of the projectiles
void processVelocities(vector<projectile> &projectiles, const double deltaTime);

//Process player moves
void processPlayer(const vector<double> &outputs, playerStruct &player);

//Processes the logic of the game
void processLogic(clock_t &deltaClock, clock_t &elapsedClock, vector<projectile> &projectiles, Species &species, playerStruct &player);

//Running the simulation itself
void simulation(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow, vector<Species> &generation);

//Gets the input of the game for the neural network
vector<double> getInput(vector<projectile> &projectiles, playerStruct player);

//Prints the graphics into the windows
void printGrapics(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow, Species &species, vector<projectile> &projectiles, playerStruct &player);

//Creates a new projectile with a random velocity
projectile createProjectile();

//Destroy projectiles that are out of the map
void destroyOutOfBoundaryProjectile(vector<projectile> &projectiles);

//Collision checks
bool collisionCheck(vector<projectile> &projectiles, playerStruct &player);

//Prints the game entites to the window
void printGame(vector<projectile> &projectiles, sf::RenderWindow &window, playerStruct &player);

//Createsa a random double in the boundaries of the arguments
double randomReal(const double &lowerBoundary, const double &upperBoundary);

//Createsa a random int in the boundaries of the arguments
double randomInt(const int &lowerBoundary, const int &upperBoundary);

/*FORWARD DECLARATIONS END*/

/*DEFINITIONS*/
//Create the first generation of networks
vector<Species> createFirstGeneration() {
	vector<Species> generation;
	for (int i = 0; i < SIZE_OF_GENERATION; i++) {
		Species tmp(NUMBER_OF_INDEPENDENT_VARAIABLES, NUMBER_OF_DEPENDENT_VARIABLES, NUMBER_OF_INITIAL_CONNECTIONS, HISTORICAL_MARKING, generation, HEIGHT_NETWORK, WIDTH_NETWORK, i, CROSSING_CHANCE, MUTATION_RATE_CONNECTION, MUTATION_RATE_NODE, MUTATION_RATE_TYPE);
		generation.push_back(tmp);
	}
	return generation;
}

//Closes the windows if window mode is deactivated
void closeWindow(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow) {
	if (!WINDOW_MODE_NETWORK) {
		neuralNetworkWindow.close();
	}
	if (!WINDOW_MODE_GAME) {
		gameWindow.close();
	}
}

//Initializes the player
playerStruct initPlayer() {
	playerStruct player;
	player.positionX = WIDTH_GAME - 3 * PLAYER_RADIUS;
	player.positionY = 2;
	return player;
}


//Processes the velocities of the projectiles
void processVelocities(vector<projectile> &projectiles, const double deltaTime) {
	for (vector<projectile>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		it->positionX += it->velocity * deltaTime;
	}
}

//Process player moves
void processPlayer(const vector<double> &outputs, playerStruct &player) {
	if (outputs[0] < 0.0)
	{
		player.positionY -= 1;
		if (player.positionY <= 0)
			player.positionY = 0;
	}
	if (outputs[0] > 0.0)
	{
		player.positionY += 1;
		if (player.positionY >= NUMBER_ROWS)
			player.positionY = NUMBER_ROWS;
	}
}

//Processes the logic of the game
void processLogic(clock_t &deltaClock, clock_t &lastClock, clock_t &elapsedClockTerminalPrint, clock_t &elapsedClockProjectile, vector<projectile> &projectiles, Species &species, playerStruct &player) {
	//Process time...
	deltaClock = clock() - lastClock;
	lastClock = clock();
	elapsedClockProjectile += deltaClock;
	elapsedClockTerminalPrint += deltaClock;
	//Process projectiles...
	if ((float(elapsedClockProjectile) / CLOCKS_PER_SEC) > PROJECTILE_SPAWN_TIME) {
		projectile tmp = createProjectile();
		elapsedClockProjectile = 0;
		projectiles.push_back(tmp);
	}
	processVelocities(projectiles, (float(deltaClock) / CLOCKS_PER_SEC));
	destroyOutOfBoundaryProjectile(projectiles);
	//Process the neural network...
	vector<double> outputs = species.process(getInput(projectiles, player));
	//Adding fitness for the elapsed time
	species.addFitness((float(deltaClock) / CLOCKS_PER_SEC));
	processPlayer(outputs, player);
}

//Running the simulation itself
void simulation(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow, vector<Species> &generation) {
	vector<projectile> projectiles;
	clock_t deltaClock = 0;
	clock_t elapsedClockProjectile = 0;
	clock_t lastClock = 0;
	clock_t elapsedClockTerminalPrint = 0;
	playerStruct player = initPlayer();
	int generationNumber = 0;
	while (true) {
		for (vector<Species>::iterator it = generation.begin(); it != generation.end(); ++it) {
			if (WINDOW_MODE_NETWORK || WINDOW_MODE_GAME) {
				while (neuralNetworkWindow.isOpen() && gameWindow.isOpen())
				{
					//Printing stats of the neural network
					if (PRINT_INFO && (float(elapsedClockTerminalPrint) / CLOCKS_PER_SEC) > 2.0) {
						elapsedClockTerminalPrint = 0;
						(*it).printInfo(generationNumber);
						(*it).printHistoricalMarking();
					}
					//Processing the logic
					processLogic(deltaClock, lastClock, elapsedClockTerminalPrint, elapsedClockProjectile, projectiles, (*it), player);
					//Check for collisions
					if (collisionCheck(projectiles, player)) {
						projectiles.clear();
						break;
					}
					//For printing the graphics
					printGrapics(neuralNetworkWindow, gameWindow, (*it), projectiles, player);
				}
			}
			else {
				while (true) {
					//Printing stats of the neural network
					if (PRINT_INFO && (float(elapsedClockTerminalPrint) / CLOCKS_PER_SEC) > 2.0) {
						elapsedClockTerminalPrint = 0;
						(*it).printInfo(generationNumber);
					}
					//Processing the logic
					processLogic(deltaClock, lastClock, elapsedClockTerminalPrint, elapsedClockProjectile, projectiles, (*it), player);
					//Check for collisions
					if (collisionCheck(projectiles, player)) {
						break;
					}
				}
			}
		}
		generationNumber++;
	}
}

//Gets the input of the game for the neural network
vector<double> getInput(vector<projectile> &projectiles, playerStruct player) {
	vector<vector<projectile>::iterator> frontProjectiles;
	for (vector<projectile>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		if (frontProjectiles.size() < (int)(NUMBER_OF_INDEPENDENT_VARAIABLES / 2.0)) {
			frontProjectiles.push_back(it);
		}
		else {
			double xValue = 0.0;
			vector<vector<projectile>::iterator>::iterator tmpIt;
			for (vector<vector<projectile>::iterator>::iterator it2 = frontProjectiles.begin(); it2 != frontProjectiles.end(); ++it2) {
				if ((**it2).positionX > xValue) {
					if ((*it).positionX < xValue) {
						(*it2) = it;
						break;
					}
					else {
						xValue = (**it2).positionX;
					}

				}
			}
		}
	}
	vector<double> inputVector;
	for (vector<vector<projectile>::iterator>::iterator it = frontProjectiles.begin(); it != frontProjectiles.end(); ++it) {
		double tmpX = 0.0;
		int tmpY = 0.0;
		tmpX = ((**it).positionX / 100) + (**it).velocity;
		tmpY = ((**it).positionY);
		inputVector.push_back(tmpX);
		inputVector.push_back(tmpY);
	}
	while (inputVector.size() < NUMBER_OF_INDEPENDENT_VARAIABLES - 1) {
		inputVector.push_back(0.0);
	}
	inputVector.push_back(player.positionY);
	return inputVector;
}

//Prints the graphics into the windows
void printGrapics(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow, Species &species, vector<projectile> &projectiles, playerStruct &player) {
	sf::Event event;
	while (neuralNetworkWindow.pollEvent(event) || gameWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			neuralNetworkWindow.close();
			gameWindow.close();
		}
	}
	sf::Color color(100, 100, 100);
	neuralNetworkWindow.clear(color);
	gameWindow.clear(color);
	species.print(neuralNetworkWindow);
	printGame(projectiles, gameWindow, player);
	neuralNetworkWindow.display();
	gameWindow.display();
}

//Creates a new projectile with a random velocity
projectile createProjectile() {
	projectile tmp;
	tmp.positionY = randomInt(0.0, NUMBER_ROWS);
	tmp.velocity = randomReal(MIN_VELOCITY, MAX_VELOCITY);
	tmp.positionX = 10.0;

	return tmp;
}

//Destroy projectiles that are out of the map
void destroyOutOfBoundaryProjectile(vector<projectile> &projectiles) {
	vector<projectile>::iterator it = projectiles.begin();
	while (it != projectiles.end()) {
		if (it->positionX >= WIDTH_GAME + PROJECTILE_RADIUS) {
			it = projectiles.erase(it);
		}
		else ++it;
	}
}

//Collision checks
bool collisionCheck(vector<projectile> &projectiles, playerStruct &player) {
	for (vector<projectile>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		if (it->positionY == player.positionY && fabs(it->positionX - player.positionX) < PLAYER_RADIUS) {
			return true;
		}
	}
	return false;
}

//Prints the game entites to the window
void printGame(vector<projectile> &projectiles, sf::RenderWindow &window, playerStruct &player) {
	//Vector of shapes for the projectiles
	vector<sf::CircleShape> projectileShapes;
	for (vector<projectile>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		sf::CircleShape tmp(PROJECTILE_RADIUS);
		tmp.setFillColor(sf::Color::Red);
		tmp.setPosition(it->positionX, (it->positionY + 0.5) * (HEIGHT_GAME / (NUMBER_ROWS + 1)));
		projectileShapes.push_back(tmp);
	}
	//Draw the projectiles (Better overlook, but 2 loops instead of one!)
	for (vector<sf::CircleShape>::iterator it = projectileShapes.begin(); it != projectileShapes.end(); ++it) {
		window.draw(*it);
	}
	//Draw the player
	sf::CircleShape tmp(PLAYER_RADIUS);
	tmp.setFillColor(sf::Color::Blue);
	tmp.setPosition(player.positionX, (player.positionY + 0.5) * (HEIGHT_GAME / (NUMBER_ROWS + 1)));
	projectileShapes.push_back(tmp);
	window.draw(tmp);
}

//Random generator 
static random_device seed_generator;
static unsigned seed;
static mt19937 mersenne_generator;
static uniform_real_distribution<double> distribution_real;

//Createsa a random double in the boundaries of the arguments
double randomReal(const double &lowerBoundary, const double &upperBoundary) {
	//Random engine initialisation
	random_device seed_generator;
	unsigned seed = seed_generator();
	uniform_real_distribution<double> distribution_real(lowerBoundary, upperBoundary);
	mt19937 mersenne_generator(seed);

	return distribution_real(mersenne_generator);
}

//Createsa a random int in the boundaries of the arguments
double randomInt(const int &lowerBoundary, const int &upperBoundary) {
	//Random engine initialisation
	random_device seed_generator;
	unsigned seed = seed_generator();
	uniform_int_distribution<int> distribution_int(lowerBoundary, upperBoundary);
	mt19937 mersenne_generator(seed);

	return distribution_int(mersenne_generator);
}

/*DEFINITIONS END*/