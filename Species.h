#pragma once
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
#include <map>

using namespace std;

int const WIDTH = 480;							//Window width
int const HEIGHT = 360;							//Window height

class Species
{
public:
	Species(double numberInputNodes, double numberOutputNodes, double crossing = 0.8, double mutationConn = 0.05, double mutationNode = 0.05);
	Species(Species &mother, Species &father, double crossing = 0.8, double mutationConn = 0.05, double mutationNode = 0.05);
	~Species();

	//Setting mutation rate for this species (It can be possible to have different mutation rates for different species)
	void setMutationAndCrossing(double crossing, double mutationConn, double mutationNode);

	//Mutation for a connection
	void mutateConnection();

	//Mutation for additional node
	void mutateNode();

	//Print this species
	void print();

	//Random generator 
	double random_real();
	double random_real_signed();
	int random_int();
	static random_device seed_generator;
	static unsigned seed;
	static mt19937 mersenne_generator;
	static uniform_real_distribution<double> distribution_real;
	static uniform_real_distribution<double> distribution_real_signed;
	static uniform_int_distribution<int> distribution_int; 
	

private:
	//Crossing and mutation rates
	double m_crossingChance = 0.8;
	double m_mutationRateConnection = 0.05;
	double m_mutationRateNode = 0.05;

	//Struct for the genome of a node
	struct m_NodeGen {
		int sense;				//Which layer this node is (0 = input, -1 = output, 1 = hidden)
		int number;				//Historical marking number
		int type;				//What type of threshold function this is
		double positionX;		//X Position for the print function
		double positionY;		//Y Position for the print function

	};

	//Struct for the genome of a connection
	struct m_ConnectionGen {
		int ConnFromNodeNumber;	//Node from which the connection flows from
		int ConnToNodeNumber;	//Node to which the connection flows
		double weight;			//Connection weight between the two nodes
		int innovationNumber;	//This is the historical marking of the Connection
		bool enabled;			//Is this connection enabled or not?
	};

	//The Node and Connection Genomes are saved in a vector
	vector<m_NodeGen> m_NodeGenomes;
	vector<m_ConnectionGen> m_ConnectionGenomes;

	//Data of the species
	int m_speciesNumber;
};