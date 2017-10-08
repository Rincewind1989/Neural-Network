#pragma once

//Initial network size
static int const NUMBER_OF_INDEPENDENT_VARAIABLES = 2 * 5 + 1;	//This is the number of inputnodes
static int const NUMBER_OF_DEPENDENT_VARIABLES = 1;			//This is the number of output variables which determin the behaviour
static int const NUMBER_OF_INITIAL_CONNECTIONS = 3;			//Maximum Number of initial connections created by random mutations

//Mutation rates and historical marking
static double const CROSSING_CHANCE = 1.0;						//When mating does a crossing occur? (TODO: Read into NEAT. Think they don�t do any crossing)
static double const MUTATION_RATE_CONNECTION = 0.1;				//Chance that a connection mutates when breeding
static double const MUTATION_RATE_ENABLING = 0.1;				//Chance that a connection en- or disables
static double const MUTATION_RATE_ADD_CONNECTION = 0.1;			//Chance that an additional connections mutates when breeding
static double const MUTATION_RATE_NODE = 0.1;					//Chance that an additional node mutates when breeding
static double const MUTATION_RATE_TYPE = 0.1;					//Chance that a node will change its type when breeding

//Graphical constants
static bool WINDOW_MODE_NETWORK = true;						//Shall the neural network be printed to the screen?
static bool WINDOW_MODE_GAME = true;							//Shall the game be printed to the screen?
static bool PRINT_INFO = true;									//Shall the network info be printed?
static int const WIDTH_NETWORK = 680;							//Window width
static int const HEIGHT_NETWORK = 520;							//Window height
static int const WIDTH_GAME = 720;								//Window width
static int const HEIGHT_GAME = 680;							//Window height
static const double NODE_RADIUS = 7.0;							//Radius of a node

//Game constants and objects
static const int SIZE_OF_GENERATION = 200;						//Size of a generation
static double MAX_VELOCITY = 350.0;							//Maximum velocity of the projectiles
static double MIN_VELOCITY = 150.0;							//Minimum velocity of the projectiles
static const int NUMBER_ROWS = 5;								//Number of rows for the projectiles
static const double PROJECTILE_RADIUS = 7.0;					//Radius of a projectile
static double PROJECTILE_SPAWN_TIME = 0.5;						//Spawn time for a projectile in seconds

static const double PLAYER_RADIUS = 7.0;						//Player radius


//Struct for a projectile												
struct projectile
{
	double positionX;
	int positionY;
	double velocity;
};


//Struct for the player
struct playerStruct
{
	double positionX;
	int positionY;
};