#include "Game.h"



Game::Game()
{
	//Initialize the player
	m_player.positionX = WIDTH_GAME - 3 * PLAYER_RADIUS;
	m_player.positionY = 2;
}


Game::~Game()
{
}


//Processes the velocities of the projectiles
void Game::processVelocities()
{
	for (vector<projectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); ++it) {
		it->positionX += it->velocity * (float(m_deltaClock) / CLOCKS_PER_SEC);
	}
}


//Process player moves
void Game::processPlayer(
	const vector<double> &outputs)
{
	if (outputs[0] < 0.0)
	{
		m_player.positionY -= 1;
		if (m_player.positionY <= 0)
			m_player.positionY = 0;
	}
	if (outputs[0] > 0.0)
	{
		m_player.positionY += 1;
		if (m_player.positionY >= NUMBER_ROWS)
			m_player.positionY = NUMBER_ROWS;
	}
}


//Processes the logic of the game
void Game::processLogic(
	vector<double> outputs)
{
	//Processes the time
	processTime();
	//Process projectiles...
	if ((float(m_elapsedClockProjectile) / CLOCKS_PER_SEC) > PROJECTILE_SPAWN_TIME) {
		projectile tmp = createProjectile();
		m_elapsedClockProjectile = 0;
		m_projectiles.push_back(tmp);
	}
	processVelocities();
	destroyOutOfBoundaryProjectile();
	//Adding fitness for the elapsed time
	processPlayer(outputs);
}


//Processes the time
void Game::processTime()
{
	//Process time...
	m_deltaClock = clock() - m_lastClock;
	m_lastClock = clock();
	m_elapsedClockProjectile += m_deltaClock;
	m_elapsedClockTerminalPrint += m_deltaClock;
}


//Creates a new projectile with a random velocity
projectile Game::createProjectile()
{
	projectile tmp;
	tmp.positionY = randomInt(0.0, NUMBER_ROWS);
	tmp.velocity = randomReal(MIN_VELOCITY, MAX_VELOCITY);
	tmp.positionX = 10.0;

	return tmp;
}


//Destroy projectiles that are out of the map
void Game::destroyOutOfBoundaryProjectile()
{
	vector<projectile>::iterator it = m_projectiles.begin();
	while (it != m_projectiles.end()) 
	{
		if (it->positionX >= WIDTH_GAME + PROJECTILE_RADIUS) 
		{
			it = m_projectiles.erase(it);
		}
		else ++it;
	}
}


//Returns the player struct
playerStruct &Game::getPlayer()
{
	return m_player;
}


//Returns the projectiles
vector<projectile> &Game::getProjectiles()
{
	return m_projectiles;
}


//Random engine call
double Game::randomReal(
	const double lowerBoundary,
	const double upperBoundary)
{
	uniform_real_distribution<double> distribution_real(lowerBoundary, upperBoundary);
	return distribution_real(mersenne_generator);
}

int Game::randomInt(
	const int lowerBoundary,
	const int upperBoundary)
{
	uniform_int_distribution<int> distribution_int(lowerBoundary, upperBoundary);
	return distribution_int(mersenne_generator);
}

//Random engine initialisation
random_device Game::seed_generator;
unsigned Game::seed = seed_generator();
mt19937 Game::mersenne_generator(Game::seed);