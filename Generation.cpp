#include "Generation.h"


Generation::Generation()
{
	for (unsigned i = 0; i < SIZE_OF_GENERATION; i++)
	{
		Genome genome;

		//Creating the input nodes
		genome.m_inputNodes = NUMBER_OF_INDEPENDENT_VARAIABLES;
		genome.m_outputNodes = NUMBER_OF_DEPENDENT_VARIABLES;

		for (int i = 0; i < genome.m_inputNodes; i++)
		{
			NodeGene tmp;
			tmp.nodeType = 0;
			tmp.number = genome.m_NodeGenes.size();
			tmp.type = 1;
			genome.m_NodeGenes.push_back(tmp);
		}

		//Creating the output nodes
		for (int i = 0; i < genome.m_outputNodes; i++)
		{
			NodeGene tmp;
			tmp.nodeType = -1;
			tmp.number = genome.m_NodeGenes.size();
			tmp.type = 1;
			genome.m_NodeGenes.push_back(tmp);
		}

		//Create the initial connections
		for (int i = 0; i < NUMBER_OF_INITIAL_CONNECTIONS; i++)
		{
			addConnection(genome);
		}

		//Every initial constructor genome has a chance to mutate a deep layer node
		mutateNode(genome);

		Organism organism(genome);
		m_organisms.push_back(organism);	
	}
}


Generation::Generation(
	Generation &lastGeneration)
{
}


Generation::~Generation()
{
}


//Add a connection
void Generation::addConnection(Genome &genome) {
	//If yes, take a random node
	ConnectionGene tmp;
	tmp.weight = randomReal(-5.0, 5.0);			//Random weight for the new mutated node
	tmp.enabled = true;							//New connection should of course be enabled
	signed randomNode = randomInt(0, genome.m_NodeGenes.size() - 1);
	while (randomNode > genome.m_inputNodes && randomNode < genome.m_inputNodes + genome.m_outputNodes)
	{
		randomNode = randomInt(0, genome.m_NodeGenes.size() - 1);
	}
	//Check if this random node isnt already connected to every other node
	while (true)
	{
		//Get the number of other nodes this node can be connected to
		int numberOfConnectableNodes = genome.m_NodeGenes.size() - 1;
		//Count the nodes this node is already connected to
		int numberOfConnectionForNode = 0;
		for (vector<ConnectionGene>::iterator it = genome.m_ConnectionGenes.begin(); it != genome.m_ConnectionGenes.end(); ++it)
		{
			if (it->ConnFromNodeNumber == randomNode)
			{
				numberOfConnectionForNode += 1;
			}
		}

		//If this node is connected to every other connectable node take a look at the next node or this node is a output node
		if (numberOfConnectionForNode == numberOfConnectableNodes)
		{
			randomNode += 1;
			while ((randomNode > genome.m_inputNodes && randomNode < genome.m_inputNodes + genome.m_outputNodes))
			{
				randomNode += 1;
			}
			if (randomNode >= genome.m_NodeGenes.size()) {
				randomNode = 0;
			}
		}
		//Else ,break the loop if a node was found that is not connected to everybody
		else
		{
			break;
		}
		//If non of the above work set the randomNode to -1
		return;
	}

	//Set the start node to the found node
	tmp.ConnFromNodeNumber = randomNode;

	//Now finding the outputNode
	//Check every other node if it already has a connection to the input node
	randomNode = randomInt(0, genome.m_NodeGenes.size() - 1);
	while (randomNode < genome.m_inputNodes)
	{
		randomNode = randomInt(0, genome.m_NodeGenes.size() - 1);
	}
	bool foundNode = true;
	while (true)
	{
		foundNode = true;
		for (vector<ConnectionGene>::iterator it = genome.m_ConnectionGenes.begin(); it != genome.m_ConnectionGenes.end(); ++it)
		{
			if ((it->ConnFromNodeNumber == tmp.ConnFromNodeNumber && randomNode == it->ConnToNodeNumber))
			{
				randomNode += 1;
				if (randomNode >= genome.m_NodeGenes.size())
				{
					randomNode = 0;
				}
				foundNode = false;
				break;
			}
		}
		if (foundNode)
		{
			tmp.ConnToNodeNumber = randomNode;
			break;
		}
	}
	checkHistoricalMarkings(tmp);
	genome.m_ConnectionGenes.push_back(tmp);
}


//Process all mutations
void Generation::processMutation(Genome &genome)
{
	mutateType(genome);
	mutateNode(genome);
	mutateAddConnection(genome);
	mutateConnection(genome);
}


//Mutations for a connection
void Generation::mutateAddConnection(Genome &genome) {
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= MUTATION_RATE_ADD_CONNECTION)
	{
		//If yes, take a random node
		ConnectionGene tmp;
		tmp.weight = randomReal(-5.0, 5.0);			//Random weight for the new mutated node
		tmp.enabled = true;							//New connection should of course be enabled
		signed randomNode = randomInt(0, genome.m_NodeGenes.size() - 1);
		while (randomNode > genome.m_inputNodes && randomNode < genome.m_inputNodes + genome.m_outputNodes)
		{
			randomNode = randomInt(0, genome.m_NodeGenes.size() - 1);
		}
		//Check if this random node isnt already connected to every other node
		while (true)
		{
			//Get the number of other nodes this node can be connected to
			int numberOfConnectableNodes = genome.m_NodeGenes.size() - 1;
			//Count the nodes this node is already connected to
			int numberOfConnectionForNode = 0;
			for (vector<ConnectionGene>::iterator it = genome.m_ConnectionGenes.begin(); it != genome.m_ConnectionGenes.end(); ++it)
			{
				if (it->ConnFromNodeNumber == randomNode)
				{
					numberOfConnectionForNode += 1;
				}
			}

			//If this node is connected to every other connectable node take a look at the next node or this node is a output node
			if (numberOfConnectionForNode == numberOfConnectableNodes)
			{
				randomNode += 1;
				while ((randomNode > genome.m_inputNodes && randomNode < genome.m_inputNodes + genome.m_outputNodes))
				{
					randomNode += 1;
				}
				if (randomNode >= genome.m_NodeGenes.size()) {
					randomNode = 0;
				}
			}
			//Else ,break the loop if a node was found that is not connected to everybody
			else
			{
				break;
			}
			//If non of the above work set the randomNode to -1
			return;
		}

		//Set the start node to the found node
		tmp.ConnFromNodeNumber = randomNode;

		//Now finding the outputNode
		//Check every other node if it already has a connection to the input node
		randomNode = randomInt(0, genome.m_NodeGenes.size() - 1);
		while (randomNode < genome.m_inputNodes)
		{
			randomNode = randomInt(0, genome.m_NodeGenes.size() - 1);
		}
		bool foundNode = true;
		while (true)
		{
			foundNode = true;
			for (vector<ConnectionGene>::iterator it = genome.m_ConnectionGenes.begin(); it != genome.m_ConnectionGenes.end(); ++it)
			{
				if ((it->ConnFromNodeNumber == tmp.ConnFromNodeNumber && randomNode == it->ConnToNodeNumber))
				{
					randomNode += 1;
					if (randomNode >= genome.m_NodeGenes.size())
					{
						randomNode = 0;
					}
					foundNode = false;
					break;
				}
			}
			if (foundNode)
			{
				tmp.ConnToNodeNumber = randomNode;
				break;
			}
		}
		checkHistoricalMarkings(tmp);
		genome.m_ConnectionGenes.push_back(tmp);
	}
}


//Mutates a weight of a connection
void Generation::mutateConnection(Genome &genome) {
	//Is there a connection for a mutation?
	if (genome.m_ConnectionGenes.size() == 0) { return; }
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= MUTATION_RATE_CONNECTION) {
		int randomConnection = randomInt(0, genome.m_ConnectionGenes.size());
		genome.m_ConnectionGenes[randomConnection].weight += randomReal(-2.0 * genome.m_ConnectionGenes[randomConnection].weight, 2.0 * genome.m_ConnectionGenes[randomConnection].weight);
	}
}


//Mutates that a connection is dis or enabled
void Generation::mutateConnectionEnabling(Genome &genome) {
	//Is there a connection for a mutation?
	if (genome.m_ConnectionGenes.size() == 0) { return; }
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= MUTATION_RATE_ENABLING) {
		int randomConnection = randomInt(0, genome.m_ConnectionGenes.size());
		genome.m_ConnectionGenes[randomConnection].enabled != genome.m_ConnectionGenes[randomConnection].enabled;
	}
}


//Mutation for an additional node
void Generation::mutateNode(Genome &genome)
{
	//Is there a connection for a mutation?
	if (genome.m_ConnectionGenes.size() == 0) { return; }
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= MUTATION_RATE_NODE)
	{
		//Yes, then add a node
		NodeGene tmp;
		tmp.number = genome.m_NodeGenes.size();
		tmp.nodeType = 1;				//New nodes are for now always in the hidden layer (TODO: Maybe later give additional input/output nodes?!)
										//Getting randomly the type of this neuron
		tmp.type = 1;
		//Set up the new inbetween connection
		int randomConnection = randomInt(0, genome.m_ConnectionGenes.size() - 1);
		for (unsigned i = 0; i < genome.m_ConnectionGenes.size(); i++)
		{
			if (genome.m_ConnectionGenes[randomConnection + i % genome.m_ConnectionGenes.size()].enabled)
			{
				//Disable the connection
				genome.m_ConnectionGenes[randomConnection + i % genome.m_ConnectionGenes.size()].enabled = false;
				//Connect the 2 before nodes with the new node
				ConnectionGene conn1;
				ConnectionGene conn2;
				conn1.ConnFromNodeNumber = genome.m_ConnectionGenes[randomConnection + i % genome.m_ConnectionGenes.size()].ConnFromNodeNumber;
				conn1.ConnToNodeNumber = tmp.number;
				conn2.ConnFromNodeNumber = tmp.number;
				conn2.ConnToNodeNumber = genome.m_ConnectionGenes[randomConnection + i % genome.m_ConnectionGenes.size()].ConnToNodeNumber;
				conn1.enabled = true;
				conn2.enabled = true;
				conn1.weight = 1.0;
				conn2.weight = genome.m_ConnectionGenes[randomConnection + i % genome.m_ConnectionGenes.size()].weight;
				//Push the new connection into the vector of connections
				checkHistoricalMarkings(conn1);
				genome.m_ConnectionGenes.push_back(conn1);
				checkHistoricalMarkings(conn2);
				genome.m_ConnectionGenes.push_back(conn2);
				//Push the new node into the vector
				genome.m_NodeGenes.push_back(tmp);
				break;
			}
		}
	}
}


//Mutation for a type change
void Generation::mutateType(Genome &genome)
{
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= MUTATION_RATE_TYPE)
	{
		unsigned randomNode = randomInt(0, genome.m_NodeGenes.size());
		if (genome.m_NodeGenes[randomNode].type == 0)
		{
			genome.m_NodeGenes[randomNode].type = 1;
		}
		else
		{
			genome.m_NodeGenes[randomNode].type = 0;
		}

	}
}


//Checks the other species for same mutated connections to keep historical marking organized
void Generation::checkHistoricalMarkings(
	ConnectionGene &newConnection)
{
	for (vector<Organism>::iterator it = m_organisms.begin(); it != m_organisms.end(); ++it)
	{
		//Because of a bug im compiler we have to use this dummy genome
		for (vector<ConnectionGene>::iterator it2 = it->getGenome().m_ConnectionGenes.begin(); it2 != it->getGenome().m_ConnectionGenes.end(); ++it2)
		{
			if (newConnection.ConnFromNodeNumber == it2->ConnFromNodeNumber && newConnection.ConnToNodeNumber == it2->ConnToNodeNumber)
			{
				newConnection.historicalNumber = it2->historicalNumber;
				return;
			}
		}
	}
	newConnection.historicalNumber = m_historicalMarking;
	m_historicalMarking += 1;
}


//Gets an organism out of this generation by index
Organism Generation::getOrganismByIndex(
	int index)
{
	return m_organisms[index];
}


//Gets an organism out of this generation by index
vector<Organism> &Generation::getOrganisms()
{
	return m_organisms;
}


//Random engine call
double Generation::randomReal(
	const double lowerBoundary,
	const double upperBoundary)
{
	uniform_real_distribution<double> distribution_real(lowerBoundary, upperBoundary);
	return distribution_real(mersenne_generator);
}

int Generation::randomInt(
	const int lowerBoundary,
	const int upperBoundary)
{
	uniform_int_distribution<int> distribution_int(lowerBoundary, upperBoundary);
	return distribution_int(mersenne_generator);
}

//Random engine initialisation
random_device Generation::seed_generator;
unsigned Generation::seed = seed_generator();
mt19937 Generation::mersenne_generator(Generation::seed);