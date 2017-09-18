#pragma once
#include <vector>
#include <map>

using namespace std;

class Neuron
{
public:
	Neuron(int type, int sense);
	~Neuron();

	//Threshold functions
	void linear();
	void sigmoid();

	//Feed forward the output into every neuron with its corresponding weight
	void feedForward();

	//Setting a value in the input map
	void setInput(Neuron* inputNeuron, double input);

	//Setting the connected Neurons for the feed forward map via the topology
	void setConnNeurons(map<Neuron*, double> neurons);

	//Getter for the outputvalue if this neuron is in the end of the neural network
	double getOutput();

	//Setting the historical marking via the genetic algortihm
	double setHistoricalMarking(int mark) { m_historicalMarking = mark; };

private:
	double m_output = 0.0;						//OutputValue of this neuron	
	map<Neuron*, double> m_connNeuronsForward;	//Map of neurons this neurons outputvalue flows into
												//Only part that is determined by the topology class
	map<Neuron*, double> m_InputGotFromNeurons;	//Map of neurons with the output that flowed into this neuron
	int m_thresholdfunc;						//Which kind of neuron this is

	int m_historicalMarking = 0;					//The historical marking of this neuron to match it up in the genetic evolution algorithm

	//Variables for the input neurons
	int m_sense;								//Which sense has the neuron as an input (TODO: Maybe one neuron is responsible for multiple senses)
												//0 stands for no sense, so this neuron is not an input neuron (TODO: Enum maybe better?)
												//-1 stands for output neuron
};

