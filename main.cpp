#include <time.h>
#include <Windows.h>
#include "Species.h"

using namespace std;

/*GLOBAL CONSTANTS*/
/*For testing we use global constants*/
//Initial network size
int const NUMBER_OF_INDEPENDENT_VARAIABLES = 5;	//This is the number of inputnodes
int const NUMBER_OF_DEPENDENT_VARIABLES = 1;		//This is the number of output variables which determin the behaviour
int const NUMBER_OF_INITIAL_CONNECTIONS = 7;		//Maximum Number of initial connections
//Mutation rates
double const CROSSING_CHANCE = 0.8;					//When mating does a crossing occur? (TODO: Read into NEAT. Think they don´t do any crossing)
double const MUTATION_RATE_CONNECTION = 0.75;		//Chance that a connection mutates when breeding
double const MUTATION_RATE_NODE = 0.75;				//Chance that an additional node mutates when breeding
double const MUTATION_RATE_TYPE = 0.75;				//Chance that a node will change its type when breeding
//Graphical constants
bool WINDOW_MODE_NETWORK = true;					//Shall the neural network be printed to the screen?
bool WINDOW_MODE_GAME = true;						//Shall the game be printed to the screen?
bool PRINT_INFO = true;								//Shall the network info be printed?
int const WIDTH_NETWORK = 680;						//Window width
int const HEIGHT_NETWORK = 520;						//Window height
int const WIDTH_GAME = 720;							//Window width
int const HEIGHT_GAME = 680;						//Window height
//Game constants and objects
const int SIZE_OF_GENERATION = 200;					//Size of a generation
double MAX_VELOCITY = 2.5;							//Maximum velocity of the projectiles
const int NUMBER_ROWS = 5;							//Number of rows for the projectiles
const double PROJECTILE_RADIUS = 7.0;				//Radius of a projectile
struct projectile{									//Struct for a projectile
	double positionX;
	int positionY;
	double velocity;
};
const double PLAYER_RADIUS = 7.0;					//Player radius
struct playerStruct {								//Struct for the player
	double positionX;
	int positionY;
};
/*GLOBAL CONSTANTS END*/

/*FORWARD DECLARATIONS*/
//Create the first generation of networks
vector<Species> createFirstGeneration();

//Closes the windows if window mode is deactivated
void closeWindow(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow);

//Running the simulation itself
void simulation(sf::RenderWindow &neuralNetwork, sf::RenderWindow &game, vector<Species> &generation);

//Processes the velocities of the projectiles
void processVelocities(vector<projectile> &projectiles, const double deltaTime);

//Process player oves
void processPlayer(const vector<double> &outputs, playerStruct &player);

//Processes the logic of the game
void processLogic(clock_t &deltaClock, clock_t &elapsedClock, vector<projectile> &projectiles, Species &species, playerStruct &player);

//Gets the input of the game for the neural network
vector<double> getInput(vector<projectile> &projectiles);

//Prints the graphics into the windows
void printGrapics(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow, Species &species, vector<projectile> &projectiles, playerStruct &player);

//Creates a new projectile with a random velocity
projectile createProjectile();

//Createsa a random double in the boundaries of the arguments
double randomReal(const double &lowerBoundary, const double &upperBoundary);

//Createsa a random int in the boundaries of the arguments
double randomInt(const int &lowerBoundary, const int &upperBoundary);

//Destroy projectiles that are out of the map
void destroyOutOfBoundaryProjectile(vector<projectile> &projectiles);

//Prints the game entites to the window
void printGame(vector<projectile> &projectiles, sf::RenderWindow &window, playerStruct &player);

//Initializes the player
playerStruct initPlayer();

//Collision checks
bool collisionCheck(vector<projectile> &projectiles, playerStruct &player);
/*FORWARD DECLARATIONS END*/

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

//Closes the windows if window mode is deactivated
void closeWindow(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow) {
	if (!WINDOW_MODE_NETWORK) {
		neuralNetworkWindow.close();
	}
	if (!WINDOW_MODE_GAME) {
		gameWindow.close();
	}
}

/*DEFINITIONS*/
//Create the first generation of networks
vector<Species> createFirstGeneration() {
	vector<Species> generation;
	for (int i = 0; i < SIZE_OF_GENERATION; i++) {
		Species tmp(NUMBER_OF_INDEPENDENT_VARAIABLES, NUMBER_OF_DEPENDENT_VARIABLES, NUMBER_OF_INITIAL_CONNECTIONS, HEIGHT_NETWORK, WIDTH_NETWORK, CROSSING_CHANCE, MUTATION_RATE_CONNECTION, MUTATION_RATE_NODE, MUTATION_RATE_TYPE);
		generation.push_back(tmp);
	}
	return generation;
}

//Running the simulation itself
void simulation(sf::RenderWindow &neuralNetworkWindow, sf::RenderWindow &gameWindow, vector<Species> &generation) {
	vector<projectile> projectiles;
	clock_t deltaClock = 0;
	clock_t elapsedClock = 0;
	playerStruct player = initPlayer();
	for (vector<Species>::iterator it = generation.begin(); it != generation.end(); ++it) {
		if (WINDOW_MODE_NETWORK || WINDOW_MODE_GAME) {
			while (neuralNetworkWindow.isOpen() && gameWindow.isOpen())
			{
				//Printing stats of the neural network
				if (PRINT_INFO && (float(elapsedClock) / CLOCKS_PER_SEC) > 240) { (*it).printInfo(); }
				//Processing the logic
				processLogic(deltaClock, elapsedClock, projectiles, (*it), player);
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
				if (PRINT_INFO && (float(elapsedClock) / CLOCKS_PER_SEC) > 240) { (*it).printInfo(); }
				//Processing the logic
				processLogic(deltaClock, elapsedClock, projectiles, (*it), player);
				//Check for collisions
				if (collisionCheck(projectiles, player)) {
					break;
				}
			}
		}
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
void processLogic(clock_t &deltaClock, clock_t &elapsedClock, vector<projectile> &projectiles, Species &species, playerStruct &player) {
	//Process time...
	deltaClock = clock() - deltaClock;
	elapsedClock += deltaClock;
	//Process projectiles...
	if ((float(elapsedClock) / CLOCKS_PER_SEC) > 250.0) {
		projectile tmp = createProjectile();
		elapsedClock = 0;
		projectiles.push_back(tmp);
	}
	processVelocities(projectiles, (float(deltaClock) / CLOCKS_PER_SEC));
	destroyOutOfBoundaryProjectile(projectiles);
	//Process the neural network...
	vector<double> outputs = species.process(getInput(projectiles));
	//Adding fitness for the elapsed time
	species.addFitness((float(deltaClock) / CLOCKS_PER_SEC) / 100.0);
	processPlayer(outputs, player);
}

//Gets the input of the game for the neural network
vector<double> getInput(vector<projectile> &projectiles) {
	vector<vector<projectile>::iterator> frontProjectiles;
	for (vector<projectile>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		if (frontProjectiles.size() < NUMBER_OF_INDEPENDENT_VARAIABLES) {
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
		double tmp = 0.0;
		tmp = ((**it).positionX / 100) + (**it).velocity;
		inputVector.push_back(tmp);
	}
	cout << endl;
	cout << endl;
	while (inputVector.size() < NUMBER_OF_INDEPENDENT_VARAIABLES) {
		inputVector.push_back(0.0);
	}
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
	tmp.velocity = randomReal(0.0, MAX_VELOCITY);
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
			std::cout << "Game over!\n";
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