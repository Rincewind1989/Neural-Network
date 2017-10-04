#pragma once

//Initial network size
int const NUMBER_OF_INDEPENDENT_VARAIABLES = 2 * 5 + 1;	//This is the number of inputnodes
int const NUMBER_OF_DEPENDENT_VARIABLES = 1;		//This is the number of output variables which determin the behaviour
int const NUMBER_OF_INITIAL_CONNECTIONS = 7;		//Maximum Number of initial connections created by random mutations

//Mutation rates and historical marking
int HISTORICAL_MARKING = 0;							//The historical marking for the connections
double const CROSSING_CHANCE = 1.0;					//When mating does a crossing occur? (TODO: Read into NEAT. Think they don´t do any crossing)
double const MUTATION_RATE_CONNECTION = 1.0;		//Chance that a connection mutates when breeding
double const MUTATION_RATE_NODE = 0.25;				//Chance that an additional node mutates when breeding
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
double MAX_VELOCITY = 350.0;							//Maximum velocity of the projectiles
double MIN_VELOCITY = 150.0;
const int NUMBER_ROWS = 5;							//Number of rows for the projectiles
const double PROJECTILE_RADIUS = 7.0;				//Radius of a projectile
double PROJECTILE_SPAWN_TIME = 0.5;					//Spawn time for a projectile in seconds

//Struct for a projectile
struct projectile {		
	double positionX;
	int positionY;
	double velocity;
};

const double PLAYER_RADIUS = 7.0;					//Player radius

//Struct for the player
struct playerStruct {								
	double positionX;
	int positionY;
};