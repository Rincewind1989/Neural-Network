#pragma once

#include <vector>
#include <time.h>
#include <random>
#include "GameData.h"

using namespace std;

class Game
{
public:
	Game();
	~Game();


	//Processes the velocities of the projectiles
	void processVelocities();


	//Process player moves
	void Game::processPlayer(
		const vector<double> &outputs);


	//Processes the logic of the game
	void Game::processLogic(
		vector<double> outputs);


	//Processes the time
	void Game::processTime();


	//Creates a new projectile with a random velocity
	projectile createProjectile();


	//Destroy projectiles that are out of the map
	void destroyOutOfBoundaryProjectile();


	//Returns the player struct
	playerStruct &getPlayer();


	//Returns the projectiles
	vector<projectile> &getProjectiles();


	//Random generator 
	double randomReal(
		const double lowerBoundary,
		const double upperBoundary);

	int randomInt(
		const int lowerBoundary,
		const int upperBoundary);

	static random_device seed_generator;
	static unsigned seed;
	static mt19937 mersenne_generator;

private:
	playerStruct m_player;
	vector<projectile> m_projectiles;

	//Time
	clock_t m_deltaClock = 0;
	clock_t m_lastClock = 0;
	clock_t m_elapsedClockTerminalPrint = 0;
	clock_t m_elapsedClockProjectile = 0;
};

