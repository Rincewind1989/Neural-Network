#pragma once
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <map>
#include <math.h>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

//Struct for the genome of a node
struct m_NodeGene {
	int sense;						//Which layer this node is (0 = input, -1 = output, 1 = hidden)
	int number;						//Historical marking number for nodes
	int type;						//What type of threshold function this is
	float positionX;				//X Position for the print function
	float positionY;				//Y Position for the print function
	vector<double>	inputs = { 0.0 };	//Inputs in this node
	double output;					//Output of this node

};

//Struct for the genome of a connection
struct m_ConnectionGene {
	int ConnFromNodeNumber;	//Node from which the connection flows from
	int ConnToNodeNumber;	//Node to which the connection flows
	double weight;			//Connection weight between the two nodes
	bool enabled;			//Is this connection enabled or not?
	int historicalNumber;	//Historical marking number for connections
};

class Species
{
public:
	Species(const int &numberInputNodes, const int &numberOutputNodes, const int &numberConnections, int &historicalMarkingNumber,vector<Species> &generation, const double &HEIGHT, const double &WIDTH, const int &speciesNumber, const double &crossing = 0.8, const double &mutationConnAdd = 0.05, const double &mutationNode = 0.05, const double &mutateType = 0.05, const double &mutateConn = 0.05);
	Species(const Species &mother, const Species &father, int &historicalMarkingNumber, vector<Species> &generation, const double &crossing = 0.8, const double &mutationConnAdd = 0.05, const double &mutationNode = 0.05, const double &mutateType = 0.05, const double &mutateConn = 0.05);
	~Species();

	//Setting mutation rate for this species (It can be possible to have different mutation rates for different species)
	void setMutationAndCrossing(const double &crossing, const double &mutationConnAdd, const double &mutationNode, const double &mutateType, const double &mutationConn);

	//Updating inputs and outputs in time
	vector<double> process(const vector<double> &inputs);
	void updateInOutputs();

	//Mutation for a connection
	void mutateAddConnection(int &historicalMarkingCounter, vector<Species> &generation);
	void mutateConnection();
	void mutateConnectionEnabling();

	//Mutation for additional node
	void mutateNode(int &historicalMarkingCounter, vector<Species> &generation);

	//Mutation for a type change
	void mutateType();

	//Checks the other species for same mutated connections to keep historical marking organized
	void checkHistoricalMarkings(vector<Species> &generation, int &historicalMarkingCounter, m_ConnectionGene &newConnection);

	//Print this species
	void print(sf::RenderWindow &window);

	//Print this species information about their nodes and connections
	void printInfo(int &generationNumber);
	void printHistoricalMarking();

	//Process all mutationchances
	void processMutation(int &historicalMarkingNumber, vector<Species> &generation);

	//Threshold functions
	//Linear threshold
	double linear(const double &sum);
	//Sigmoid threshold
	double sigmoid(const double &sum);

	//Getter/Setter and Adder for fitness value
	void setFitness(double fitness) { m_fitness = fitness; }
	double getFitness() { return m_fitness; }
	void addFitness(double addValue) { m_fitness += addValue; }

	//Random generator 
	double randomReal(const double lowerBoundary, const double upperBoundary);
	int randomInt(const int lowerBoundary, const int upperBoundary);
	static random_device seed_generator;
	static unsigned seed;
	static mt19937 mersenne_generator;

private:
	//Crossing and mutation rates
	double m_crossingChance = 0.8;
	double m_mutationRateConnection = 0.05;
	double m_mutationRateNode = 0.05;
	double m_mutationRateType = 0.05;
	double m_mutationRateConnectionAdd = 0.05;

	//The Node and Connection Genomes are saved in a vector
	vector<m_NodeGene> m_NodeGenes;
	vector<m_ConnectionGene> m_ConnectionGenes;

	//Data of the species
	int m_speciesNumber;
	int m_inputNodes;
	int m_outputNodes;
	double m_fitness;
};