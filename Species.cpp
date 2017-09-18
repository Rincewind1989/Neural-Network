#include "Species.h"

//Default constructor is for the first generation of networks
Species::Species(double numberInputNodes, double numberOutputNodes, double crossing, double mutationConn, double mutationNode)
{
	//Create the nodes for the dummy network
	/*
	for (int i = 2; i < ((random_int() % 9) + 4); i++) {
		m_NodeGen tmp;
		tmp.sense = 0;
		if (random_real() <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		tmp.number = m_NodeGenomes.size();
		m_NodeGenomes.push_back(tmp);
	}
	for (int i = 1; i < ((random_int() % 9) + 3); i++) {
		m_NodeGen tmp;
		tmp.sense = -1;
		if (random_real() <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		tmp.number = m_NodeGenomes.size();
		m_NodeGenomes.push_back(tmp);
	}
	for (int i = 2; i < ((random_int() % 9) + 4); i++) {
		m_NodeGen tmp;
		tmp.sense = 2;
		if (random_real() <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		tmp.number = m_NodeGenomes.size();
		m_NodeGenomes.push_back(tmp);
	}

	//Create the connections for the dummy network
	for (int i = 0; i < m_NodeGenomes.size(); i++) {
		m_ConnectionGen tmp;
		int randomNode = random_int() % m_NodeGenomes.size();
		int randomNode2 = random_int() % m_NodeGenomes.size();
		while (m_NodeGenomes[randomNode2].sense == m_NodeGenomes[randomNode].sense) {
			randomNode2 = random_int() % m_NodeGenomes.size();
		}
		tmp.ConnFromNodeNumber = randomNode;
		tmp.ConnToNodeNumber = randomNode2;
		tmp.weight = random_real_signed();
		tmp.enabled = true;
		m_ConnectionGenomes.push_back(tmp);
	}
	*/

	/*Creating the basic construct*/
	//Creating the input nodes
	float nodeRadius = 7.0f;
	for (int i = 0; i < numberInputNodes; i++) {
		m_NodeGen tmp;
		tmp.sense = 0;
		tmp.number = m_NodeGenomes.size();
		tmp.positionX = 10.0f;
		tmp.positionY = (HEIGHT / numberInputNodes) * i + 2 * nodeRadius;
		if (random_real() <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		m_NodeGenomes.push_back(tmp);
	}

	//Creating the output nodes
	for (int i = 0; i < numberOutputNodes; i++) {
		m_NodeGen tmp;
		tmp.sense = -1;
		tmp.number = m_NodeGenomes.size();
		tmp.positionX = WIDTH - 10.0f - 2 * nodeRadius;
		tmp.positionY = (HEIGHT / numberOutputNodes) * i + 2 * nodeRadius;
		if (random_real() <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		m_NodeGenomes.push_back(tmp);
	}

	//Create the connections for the dummy network
	for (int i = 0; i < 1; i++) {
		m_ConnectionGen tmp;
		int randomNode = random_int() % m_NodeGenomes.size();
		int randomNode2 = random_int() % m_NodeGenomes.size();
		while (m_NodeGenomes[randomNode2].sense == m_NodeGenomes[randomNode].sense) {
			randomNode2 = random_int() % m_NodeGenomes.size();
		}
		tmp.ConnFromNodeNumber = randomNode;
		tmp.ConnToNodeNumber = randomNode2;
		tmp.weight = random_real_signed();
		tmp.enabled = true;
		m_ConnectionGenomes.push_back(tmp);
	}
	//Set the rates for this species
	setMutationAndCrossing(crossing, mutationConn, mutationNode);
}

Species::Species(Species &mother, Species &father, double crossing, double mutationConn, double mutationNode)
{
	setMutationAndCrossing(crossing, mutationConn, mutationNode);
}


Species::~Species()
{
}

//Setting mutation rate for this species (It can be possible to have different mutation rates for different species)
void Species::setMutationAndCrossing(double crossing = 0.8, double mutationConn = 0.05, double mutationNode = 0.05) {
	m_crossingChance = crossing;
	m_mutationRateConnection = mutationConn;
	m_mutationRateNode = mutationNode;
}

//Random engine initialisation
random_device Species::seed_generator;
unsigned Species::seed = seed_generator();
uniform_real_distribution<double> Species::distribution_real(0.0, 1.0);
uniform_real_distribution<double> Species::distribution_real_signed(-1.0, 1.0);
uniform_int_distribution<int> Species::distribution_int(0, INT_MAX);
mt19937 Species::mersenne_generator(Species::seed);


//Print this species
void Species::print() {
	//Create window to draw in
	float nodeRadius = 7.0f;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Neural Network of species " + to_string(m_speciesNumber));

	/*Drawing the nodes*/
	//Vector of shapes for the neurons
	vector<sf::CircleShape> neurons;
	//Count the input neurons
	int inputNeuronsNumber = 0;
	int outputNeuronNumber = 0;
	int hiddenNeuronNumber = 0;
	for (vector<m_NodeGen>::iterator it = m_NodeGenomes.begin(); it != m_NodeGenomes.end(); ++it) {
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
	for (vector<m_NodeGen>::iterator it = m_NodeGenomes.begin(); it != m_NodeGenomes.end(); ++it) {
		//Add the input neurons
		if (it->sense == 0) {
			sf::CircleShape tmp(nodeRadius);
			tmp.setPosition(it->positionX, it->positionY);
			if (it->type == 0) {
				tmp.setFillColor(sf::Color::Black);
			}
			else {
				tmp.setFillColor(sf::Color::Blue);
			}
			neurons.push_back(tmp);
			vector<double> position = { it->positionX, it->positionY };
			nodePositons[it->number] = position;
			numberInputNeuronsSoFar += 1;
		}
		//Add the output neurons
		if (it->sense == -1) {
			sf::CircleShape tmp(nodeRadius);
			tmp.setPosition(it->positionX, it->positionY);
			if (it->type == 0) {
				tmp.setFillColor(sf::Color::Black);
			}
			else {
				tmp.setFillColor(sf::Color::Blue);
			}
			neurons.push_back(tmp);
			vector<double> position = { it->positionX, it->positionY };
			nodePositons[it->number] = position;
			numberOutputNeuronsSoFar += 1;
		}
		//Add the deep layer neurons
		if (it->sense != 0 && it->sense != -1) {
			sf::CircleShape tmp(nodeRadius);
			tmp.setPosition(it->positionX, it->positionY);
			if (it->type == 0) {
				tmp.setFillColor(sf::Color::Black);
			}
			else {
				tmp.setFillColor(sf::Color::Blue);
			}
			neurons.push_back(tmp);
			vector<double> position = { it->positionX, it->positionY };
			nodePositons[it->number] = position;
			numberHiddenNeuronsSoFar += 1;
		}
	}

	/*Drawing the connections*/
	//Vector of 2-dimension vertex for the lines
	vector<vector<sf::Vertex>> lines;
	//Map iterator for the 2 vertex positions of the connetion ends
	map<int, vector<double>>::iterator Position1;
	map<int, vector<double>>::iterator Position2;
	//Go through the m:ConnectionGen vector and look for connections that are enabled
	for (vector<m_ConnectionGen>::iterator it = m_ConnectionGenomes.begin(); it != m_ConnectionGenomes.end(); ++it) {
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
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		sf::Color color(100, 100, 100);
		window.clear(color);
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
		window.display();
	}
}

//Random engine call
double Species::random_real() {
	return distribution_real(mersenne_generator);
}

double Species::random_real_signed() {
	return distribution_real_signed(mersenne_generator);
}

int Species::random_int() {
	return distribution_int(mersenne_generator);
}


//Mutation for a connection
void Species::mutateConnection() {
	//Does a mutation occur?
	if (random_real() <= m_mutationRateConnection) {
		//If yes, take a random node
		m_ConnectionGen tmp;
		tmp.weight = random_real_signed();			//Every mutated weight starts with 1 to give the new species time to test this new node
		tmp.innovationNumber = 0;					//STILL TO DO THE INNOVATION NUMBER!
		tmp.enabled = true;							//New connection should of course be enabled
		unsigned randomNode = random_int() % m_NodeGenomes.size();
		//Check if this random node isnt already connected to every other node
		while (true){
			//Get the number of other nodes this node can be connected to (Only other sense nodes)
			int numberOfConnectableNodes = 0;
			for (vector<m_NodeGen>::iterator it = m_NodeGenomes.begin(); it != m_NodeGenomes.end(); ++it) {
				if (it->sense != m_NodeGenomes[randomNode].sense) {
					numberOfConnectableNodes += 1;
				}
			}
			//Count the nodes this node is already connected to
			int numberOfConnectionForNode = 0;
			for (vector<m_ConnectionGen>::iterator it = m_ConnectionGenomes.begin(); it != m_ConnectionGenomes.end(); ++it) {
				if (it->ConnFromNodeNumber == randomNode || it->ConnToNodeNumber == randomNode) {
					numberOfConnectionForNode += 1;
				}
			}

			//If this node is connected to every other connectable node take a look at the next node
			if (numberOfConnectionForNode == numberOfConnectableNodes) {
				randomNode += 1;
				if (randomNode >= m_NodeGenomes.size()) {
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
		randomNode = random_int() % m_NodeGenomes.size();
		bool foundNode = true;
		while (true) {
			foundNode = true;
			if (m_NodeGenomes[randomNode].sense == m_NodeGenomes[tmp.ConnFromNodeNumber].sense) {
				randomNode += 1;
				if (randomNode >= m_NodeGenomes.size()) {
					randomNode = 0;
				}
				foundNode = false;
				continue;
			}
			for (vector<m_ConnectionGen>::iterator it = m_ConnectionGenomes.begin(); it != m_ConnectionGenomes.end(); ++it) {
				//If this node is a node from the same sense, continue the loop				
				if ((it->ConnFromNodeNumber == tmp.ConnFromNodeNumber && randomNode == it->ConnToNodeNumber) || (it->ConnToNodeNumber == tmp.ConnFromNodeNumber && randomNode == it->ConnFromNodeNumber)) {
					randomNode += 1;
					if (randomNode >= m_NodeGenomes.size()) {
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
		m_ConnectionGenomes.push_back(tmp);
	}
}

//Mutation for an additional node
void Species::mutateNode() {
	//Is there a connection for a mutation?
	if (m_ConnectionGenomes.size() == 0) { return; }
	//Does a mutation occur?
	if (random_real() <= m_mutationRateNode) {
		//Yes, then add a node
		m_NodeGen tmp;
		tmp.number = m_NodeGenomes.size(); //STILL TO DO THE INNOVATION NUMBER!
		tmp.sense = 1;	//New nodes are for now always in the hidden layer (TODO: Maybe later give additional input/output nodes?!)
		//Getting randomly the type of this neuron
		if (random_real() <= 0.5) {
			tmp.type = 0;
		}
		else {
			tmp.type = 1;
		}
		//Set up the new inbetween connection
		int randomConnection = random_int() % m_ConnectionGenomes.size();
		for (unsigned i = 0; i < m_ConnectionGenomes.size(); i++) {
			if (m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].enabled) {
				//Disable the connection
				m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].enabled = false;
				//Connect the 2 before nodes with the new node
				m_ConnectionGen conn1;
				m_ConnectionGen conn2;
				conn1.ConnFromNodeNumber = m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].ConnFromNodeNumber;
				conn1.ConnToNodeNumber = tmp.number;
				conn2.ConnFromNodeNumber = tmp.number;
				conn2.ConnToNodeNumber = m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].ConnToNodeNumber;
				conn1.enabled = true;
				conn2.enabled = true;
				conn1.weight = 1.0;
				conn2.weight = m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].weight;
				//Set the new node position
				tmp.positionX = (m_NodeGenomes[m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].ConnFromNodeNumber].positionX + m_NodeGenomes[m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].ConnToNodeNumber].positionX) / 2.0;
				tmp.positionY = (m_NodeGenomes[m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].ConnFromNodeNumber].positionY + m_NodeGenomes[m_ConnectionGenomes[randomConnection + i % m_ConnectionGenomes.size()].ConnToNodeNumber].positionY) / 2.0;
				//TODO. INOOVATION NUMBER!
				//Push the new connection into the vector of connections
				m_ConnectionGenomes.push_back(conn1);
				m_ConnectionGenomes.push_back(conn2);
				//Push the new node into the vector
				m_NodeGenomes.push_back(tmp);
				break;
			}
		}
	}
}