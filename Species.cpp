#include "Species.h"

//Default constructor is for the first generation of networks
Species::Species(const int &numberInputNodes, const int &numberOutputNodes, const int &numberConnections, const double &HEIGHT, const double &WIDTH, const double &crossing, const double &mutationConnAdd, const double &mutationNode, const double &mutateType, const double &mutateConn)
{
	/*Creating the basic construct*/
	//Set the rates for this species
	setMutationAndCrossing(crossing, mutationConnAdd, mutationNode, mutateType, mutateConn);
	//Creating the input nodes
	float nodeRadius = 7.0f;
	m_inputNodes = numberInputNodes;
	m_outputNodes = numberOutputNodes;
	for (int i = 0; i < numberInputNodes; i++) {
		m_NodeGene tmp;
		tmp.sense = 0;
		tmp.number = m_NodeGenes.size();
		tmp.positionX = 25.0f;
		tmp.positionY = (HEIGHT / numberInputNodes) * i + 2 * nodeRadius;
		if (randomReal(0.0, 1.0) <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		m_NodeGenes.push_back(tmp);
	}

	//Creating the output nodes
	for (int i = 0; i < numberOutputNodes; i++) {
		m_NodeGene tmp;
		tmp.sense = -1;
		tmp.number = m_NodeGenes.size();
		tmp.positionX = WIDTH - 25.0f - 2 * nodeRadius;
		tmp.positionY = (HEIGHT / numberOutputNodes) * i + 2 * nodeRadius;
		if (randomReal(0.0, 1.0) <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		m_NodeGenes.push_back(tmp);
	}

	//Create the initial connections
	for (int i = 0; i < numberConnections; i++) {
		mutateAddConnection();
	}
}

//This will create children via breeding of the fittest
Species::Species(const Species &mother, const Species &father, const double &crossing, const double &mutationConnAdd, const double &mutationNode, const double &mutateType, const double &mutateConn)
{
	setMutationAndCrossing(crossing, mutationConnAdd, mutationNode, mutateType, mutateConn);
}


Species::~Species()
{
}

//Setting mutation rate for this species (It can be possible to have different mutation rates for different species)
void Species::setMutationAndCrossing(const double &crossing, const double &mutationConnAdd, const double &mutationNode, const double &mutateType, const double &mutationConn) {
	m_crossingChance = crossing;
	m_mutationRateConnection = mutationConn;
	m_mutationRateNode = mutationNode;
	m_mutationRateType = mutateType;
	m_mutationRateConnectionAdd = mutationConnAdd;
}

//Updating inputs and outputs
vector<double> Species::process(const vector<double> &inputs) {
	updateInOutputs();	//Updating all outputs in nodes through their saved inputs

	int index = 0;
	if (inputs.size() != m_inputNodes) {
		perror("Number of input-values is not equal to number of input nodes! Assuming all other inputs are 0...");
	}
	for (vector<m_NodeGene>::iterator it = m_NodeGenes.begin(); it != m_NodeGenes.begin() + m_inputNodes; ++it) { //Carefull, the first nodes are always the input nodes!
		if (index >= inputs.size()) { 
			it->inputs.push_back(0.0);
		}
		else {
			it->inputs.push_back(inputs[index]);
		}
		index++;
	}
	//Returning the output from the output-nodes
	vector<double> outputs;
	for (vector<m_NodeGene>::iterator it = m_NodeGenes.begin() + m_inputNodes; it != m_NodeGenes.begin() + m_inputNodes + m_outputNodes; ++it) {
		outputs.push_back(it->output);
	}
	return outputs;
}
//Updating all outputs in nodes through their saved inputs
void Species::updateInOutputs() {
	for (vector<m_NodeGene>::iterator it = m_NodeGenes.begin(); it != m_NodeGenes.end(); ++it) {
		double sumInputs = 0.0;
		for (vector<double>::iterator it2 = it->inputs.begin(); it2 != it->inputs.end(); ++it2) {
			sumInputs += *it2;
		}
		if (it->type == 0) {
			it->output = linear(sumInputs);
		}
		if (it->type == 1) {
			it->output = sigmoid(sumInputs);
		}
	}

	//Delete the old inputs
	for (vector<m_NodeGene>::iterator it = m_NodeGenes.begin(); it != m_NodeGenes.end(); ++it) {
		it->inputs.clear();
	}

	//Update all inputs in nodes through connections
	for (vector<m_ConnectionGene>::iterator it = m_ConnectionGenes.begin(); it != m_ConnectionGenes.end(); ++it) {
		double input = 0.0;
		input = m_NodeGenes[it->ConnFromNodeNumber].output * it->weight;
		m_NodeGenes[it->ConnToNodeNumber].inputs.push_back(input);
	}
}

//Process all mutationchances
void Species::processMutation() {
	mutateType();
	mutateNode();
	mutateAddConnection();
	mutateConnection();
}

//Threshold functions
//Linear threshold
double Species::linear(const double &sum) {
	if (sum <= 0.0) { return 0.0; }
	else { return sum; }
}

//Sigmoid threshold
double Species::sigmoid(const double &sum) {
	return ((1 / (1 + exp(-sum))) - 0.5);
}


//Print this species
void Species::print(sf::RenderWindow &window) {
	float nodeRadius = 7.0f;
	//Gather the font from the file
	sf::Font font;
	if (!font.loadFromFile("CurlyLou.ttf"))
	{
		perror("Font file couldnt be loaded.");
	}

	/*Drawing the nodes*/
	//Vector of shapes for the neurons
	vector<sf::CircleShape> neurons;
	//Count the input neurons
	int inputNeuronsNumber = 0;
	int outputNeuronNumber = 0;
	int hiddenNeuronNumber = 0;
	for (vector<m_NodeGene>::iterator it = m_NodeGenes.begin(); it != m_NodeGenes.end(); ++it) {
		if (it->sense == 0) {
			inputNeuronsNumber += 1;
		}
		if (it->sense == -1) {
			outputNeuronNumber += 1;
		}
		if (it->sense != 0 && it->sense != -1) {
			hiddenNeuronNumber += 1;
		}
	}
	int numberInputNeuronsSoFar = 0;
	int numberOutputNeuronsSoFar = 0;
	int numberHiddenNeuronsSoFar = 0;
	//Position and node number is saved in a map for later connection drawing (TODO: Is there maybe a faster code to do this?)
	map<int, vector<double>> nodePositons;
	//Text for the node-output-values
	vector<vector<double>> textVector;
	for (vector<m_NodeGene>::iterator it = m_NodeGenes.begin(); it != m_NodeGenes.end(); ++it) {
		//Add the input neurons and their ouputvalue
		if (it->sense == 0) {
			//Create the neuron shape
			sf::CircleShape tmp(nodeRadius);
			tmp.setPosition(it->positionX, it->positionY);
			if (it->type == 0) {
				tmp.setFillColor(sf::Color::Black);
			}
			else {
				tmp.setFillColor(sf::Color::Blue);
			}
			neurons.push_back(tmp);
			//Save the neuron shape positon
			vector<double> position = { it->positionX, it->positionY };
			nodePositons[it->number] = position;
			numberInputNeuronsSoFar += 1;
			//Save the neuron text positon
			vector<double> text = { it->positionX - 2*nodeRadius, it->positionY - 1.5*nodeRadius , it->output };
			textVector.push_back(text);
		}
		//Add the output neurons and their ouputvalue
		if (it->sense == -1) {
			//Create the neuron shape
			sf::CircleShape tmp(nodeRadius);
			tmp.setPosition(it->positionX, it->positionY);
			if (it->type == 0) {
				tmp.setFillColor(sf::Color::Black);
			}
			else {
				tmp.setFillColor(sf::Color::Blue);
			}
			neurons.push_back(tmp);
			//Save the neuron shape positon
			vector<double> position = { it->positionX, it->positionY };
			nodePositons[it->number] = position;
			numberOutputNeuronsSoFar += 1;
			//Save the neuron text positon
			vector<double> text = { it->positionX - 2 * nodeRadius, it->positionY - 1.5*nodeRadius , it->output };
			textVector.push_back(text);
		}
		//Add the deep layer neurons and their ouputvalue
		if (it->sense != 0 && it->sense != -1) {
			//Create the neuron shape
			sf::CircleShape tmp(nodeRadius);
			tmp.setPosition(it->positionX, it->positionY);
			if (it->type == 0) {
				tmp.setFillColor(sf::Color::Black);
			}
			else {
				tmp.setFillColor(sf::Color::Blue);
			}
			neurons.push_back(tmp);
			//Save the neuron shape positon
			vector<double> position = { it->positionX, it->positionY };
			nodePositons[it->number] = position;
			numberHiddenNeuronsSoFar += 1;
			//Save the neuron text positon
			vector<double> text = { it->positionX - 2 * nodeRadius, it->positionY - 1.5*nodeRadius , it->output };
			textVector.push_back(text);
		}
	}

	/*Drawing the connections*/
	//Vector of 2-dimension vertex for the lines
	vector<vector<sf::Vertex>> lines;
	//Map iterator for the 2 vertex positions of the connetion ends
	map<int, vector<double>>::iterator Position1;
	map<int, vector<double>>::iterator Position2;
	//Go through the m:ConnectionGen vector and look for connections that are enabled
	for (vector<m_ConnectionGene>::iterator it = m_ConnectionGenes.begin(); it != m_ConnectionGenes.end(); ++it) {
		if (it->enabled) {
			Position1 = nodePositons.find(it->ConnFromNodeNumber);
			if (Position1 == nodePositons.end()) {
				perror("Draw error: Connection-end 1 not connected to any Node!");
			}
			Position2 = nodePositons.find(it->ConnToNodeNumber);
			if (Position2 == nodePositons.end()) {
				perror("Draw error: Connection-end 2 not connected to any Node!");
			}
			vector<sf::Vertex> tmp = { sf::Vector2f(Position1->second[0] + nodeRadius, Position1->second[1] + nodeRadius) , sf::Vector2f(Position2->second[0] + nodeRadius, Position2->second[1] + nodeRadius) };
			if (it->weight < 0) {
				tmp[0].color = sf::Color::Red;
				tmp[1].color = sf::Color::Red;
			}
			else {
				tmp[0].color = sf::Color::Black;
				tmp[1].color = sf::Color::Black;
			}
			lines.push_back(tmp);
		}
	}

	//Draw the nodes
	for (vector<sf::CircleShape>::iterator it = neurons.begin(); it != neurons.end(); ++it) {
		window.draw(*it);
	}
	//Draw the connections
	for (vector<vector<sf::Vertex>>::iterator it = lines.begin(); it != lines.end(); ++it) {
		//For some reason SFML only takes arrays but no vectors (maybe my fault) so we have to convert the vector back into an array
		sf::Vertex vertexArray[] = { (*it)[0], (*it)[1] };
		vertexArray[0].color = (*it)[0].color;
		vertexArray[1].color = (*it)[1].color;
		window.draw(vertexArray, 2, sf::Lines);
	}
	//Draw the outputvalues
	for (vector<vector<double>>::iterator it = textVector.begin(); it != textVector.end(); ++it) {
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(16);
		text.setFillColor(sf::Color::Black);
		text.setOutlineColor(sf::Color::Black);
		text.setString(to_string((*it)[2]));
		sf::Vector2f pos((*it)[0], (*it)[1]);
		text.setPosition(pos);
		window.draw(text);
	}
}

//Print this species information about their nodes and connections
void Species::printInfo() {
	//Printing the information to the consolde
	system("cls");
	cout << "This species information:\n\n";
	cout << "Fitness: " << m_fitness << endl;
	cout << "Number of nodes: " << m_NodeGenes.size() << endl;
	cout << "Number of input-nodes: " << m_inputNodes << endl;
	cout << "Number of output-Nodes: " << m_outputNodes << endl;
	cout << "Number of hidden-nodes: " << m_NodeGenes.size() - m_inputNodes - m_outputNodes << endl;
	cout << "Number of connections: " << m_ConnectionGenes.size() << endl;
	int numberBadConnections = 0;
	int numberGoodConnections = 0;
	for (vector<m_ConnectionGene>::iterator it = m_ConnectionGenes.begin(); it != m_ConnectionGenes.end(); it++) {
		if (it->weight < 0.0)
			numberBadConnections += 1;
		if (it->weight > 0.0)
			numberGoodConnections += 1;
	}
	cout << "Number of negative connections: " << numberBadConnections << endl;
	cout << "Number of positive connections: " << numberGoodConnections << endl;

}

//Mutations for a connection
void Species::mutateAddConnection() {
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= m_mutationRateConnectionAdd) {
		//If yes, take a random node
		m_ConnectionGene tmp;
		tmp.weight = randomReal(-5.0, 5.0);			//Random weight for the new mutated node
		tmp.innovationNumber = 0;					//STILL TO DO THE INNOVATION NUMBER!
		tmp.enabled = true;							//New connection should of course be enabled
		unsigned randomNode = randomInt(0, m_NodeGenes.size() - 1);
		//Check if this random node isnt already connected to every other node
		while (true){
			//Get the number of other nodes this node can be connected to
			int numberOfConnectableNodes = m_NodeGenes.size() - 1;
			//Count the nodes this node is already connected to
			int numberOfConnectionForNode = 0;
			for (vector<m_ConnectionGene>::iterator it = m_ConnectionGenes.begin(); it != m_ConnectionGenes.end(); ++it) {
				if (it->ConnFromNodeNumber == randomNode) {
					numberOfConnectionForNode += 1;
				}
			}

			//If this node is connected to every other connectable node take a look at the next node
			if (numberOfConnectionForNode == numberOfConnectableNodes) {
				randomNode += 1;
				if (randomNode >= m_NodeGenes.size()) {
					randomNode = 0;
				}
			}
			//Else ,break the loop if a node was found that is not connected to everybody
			else {
				break;
			}
			//If non of the above work set the randomNode to -1
			randomNode = -1;
		}

		//Set the start node to the found node
		if (randomNode != -1) {
			tmp.ConnFromNodeNumber = randomNode;
		}
		//Return if every node is already connected to every other node
		else {
			return;
		}

		//Now finding the outputNode
		//Check every other node if it already has a connection to the input node
		randomNode = randomInt(0, m_NodeGenes.size() - 1);
		bool foundNode = true;
		while (true) {
			foundNode = true;
			for (vector<m_ConnectionGene>::iterator it = m_ConnectionGenes.begin(); it != m_ConnectionGenes.end(); ++it) {			
				if ((it->ConnFromNodeNumber == tmp.ConnFromNodeNumber && randomNode == it->ConnToNodeNumber)) {
					randomNode += 1;
					if (randomNode >= m_NodeGenes.size()) {
						randomNode = 0;
					}
					foundNode = false;
					break;
				}
			}
			if (foundNode) {
				tmp.ConnToNodeNumber = randomNode;
				break;
			}
		}
		m_ConnectionGenes.push_back(tmp);
	}
}
void Species::mutateConnection() {
	//Is there a connection for a mutation?
	if (m_ConnectionGenes.size() == 0) { return; }
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= m_mutationRateConnection) {
		int randomConnection = randomInt(0, m_ConnectionGenes.size());
		m_ConnectionGenes[randomConnection].weight += randomReal(-2.0 * m_ConnectionGenes[randomConnection].weight, 2.0 * m_ConnectionGenes[randomConnection].weight);
	}
}
void Species::mutateConnectionEnabling() {
	//Is there a connection for a mutation?
	if (m_ConnectionGenes.size() == 0) { return; }
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= m_mutationRateConnection) {
		int randomConnection = randomInt(0, m_ConnectionGenes.size());
		m_ConnectionGenes[randomConnection].enabled != m_ConnectionGenes[randomConnection].enabled;
	}
}

//Mutation for an additional node
void Species::mutateNode() {
	//Is there a connection for a mutation?
	if (m_ConnectionGenes.size() == 0) { return; }
	//Does a mutation occur?
	if (randomReal(0.0, 1.0) <= m_mutationRateNode) {
		//Yes, then add a node
		m_NodeGene tmp;
		tmp.number = m_NodeGenes.size(); //STILL TO DO THE INNOVATION NUMBER!
		tmp.sense = 1;	//New nodes are for now always in the hidden layer (TODO: Maybe later give additional input/output nodes?!)
		//Getting randomly the type of this neuron
		if (randomReal(0.0, 1.0) <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		//Set up the new inbetween connection
		int randomConnection = randomInt(0, m_ConnectionGenes.size());
		for (unsigned i = 0; i < m_ConnectionGenes.size(); i++) {
			if (m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].enabled) {
				//Disable the connection
				m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].enabled = false;
				//Connect the 2 before nodes with the new node
				m_ConnectionGene conn1;
				m_ConnectionGene conn2;
				conn1.ConnFromNodeNumber = m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].ConnFromNodeNumber;
				conn1.ConnToNodeNumber = tmp.number;
				conn2.ConnFromNodeNumber = tmp.number;
				conn2.ConnToNodeNumber = m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].ConnToNodeNumber;
				conn1.enabled = true;
				conn2.enabled = true;
				conn1.weight = 1.0;
				conn2.weight = m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].weight;
				//Set the new node position
				tmp.positionX = (m_NodeGenes[m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].ConnFromNodeNumber].positionX + m_NodeGenes[m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].ConnToNodeNumber].positionX) / 2.0;
				tmp.positionY = (m_NodeGenes[m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].ConnFromNodeNumber].positionY + m_NodeGenes[m_ConnectionGenes[randomConnection + i % m_ConnectionGenes.size()].ConnToNodeNumber].positionY) / 2.0;
				//TODO. INOOVATION NUMBER!
				//Push the new connection into the vector of connections
				m_ConnectionGenes.push_back(conn1);
				m_ConnectionGenes.push_back(conn2);
				//Push the new node into the vector
				m_NodeGenes.push_back(tmp);
				break;
			}
		}
	}
}

//Mutation for a type change
void Species::mutateType() {
	//Does a mutation occur?
	if (randomReal(0.0,1.0) <= m_mutationRateType) {
		unsigned randomNode = randomInt(0, m_NodeGenes.size());
		if (m_NodeGenes[randomNode].type == 0) {
			m_NodeGenes[randomNode].type = 1;
		}
		else {
			m_NodeGenes[randomNode].type = 0;
		}

	}
}

//Random engine call
double Species::randomReal(const double lowerBoundary, const double upperBoundary) {
	uniform_real_distribution<double> distribution_real(lowerBoundary, upperBoundary);
	return distribution_real(mersenne_generator);
}

int Species::randomInt(const int lowerBoundary, const int upperBoundary) {
	uniform_int_distribution<int> distribution_int(lowerBoundary, upperBoundary);
	return distribution_int(mersenne_generator);
}

//Random engine initialisation
random_device Species::seed_generator;
unsigned Species::seed = seed_generator();
mt19937 Species::mersenne_generator(Species::seed);