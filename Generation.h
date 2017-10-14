#pragma once

#include <vector>
#include <random>
#include <string>
#include "GameData.h"
#include "Organism.h"

using namespace std;

class Generation
{
public:
	Generation();
	Generation(Generation &lastGeneration);
	~Generation();


	//Breeds a new Organism by combining the Genomes of the father and mother and adds random mutations
	Organism breedNewOrganism(
		Organism &father,
		Organism &mother);


	//Compares  the compatibility of two Organism for the speciation of NEAT
	double compatibilityDistance(
		Organism &organism1,
		Organism &organism2);


	//Add a connection
	void addConnection(Genome &genome);


	//Process all mutationchances
	void processMutation(Genome &genome);


	//Mutates an additional connection
	void mutateAddConnection(Genome &genome);


	//Mutates the weight of a connection
	void mutateConnection(Genome &genome);


	//Mutates a connection to enabled or disabled
	void mutateConnectionEnabling(ConnectionGene &connection);


	//Mutation for additional node
	void mutateNode(Genome &genome);


	//Mutation for a type change
	void mutateType(Genome &genome);


	//Checks the other species for same mutated connections to keep historical marking organized
	void Generation::checkHistoricalMarkings(
		ConnectionGene &newConnection);


	//Gets an organism out of this generation by index
	Organism getOrganismByIndex(int index);


	//Gets an organism out of this generation by index
	vector<Organism> &getOrganisms();


	//Returns the historical Marking of this generation
	int getHistoricalMarking() { return m_historicalMarking; }


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

	vector<Organism> m_organisms;

	int m_historicalMarking = 0;
};

