#include <math.h>
#include "Neuron.h"

Neuron::Neuron(int type, int sense) :
	m_thresholdfunc(type), m_sense(sense)
{
}


Neuron::~Neuron()
{
}


//Threshold functions
//Linear threshold
void Neuron::linear() {
	double sum = 0.0;
	for (map<Neuron*, double>::iterator it = m_InputGotFromNeurons.begin(); it != m_InputGotFromNeurons.end(); ++it) {
		sum += it->second;
	}
	if (sum <= 0.0) { m_output = 0.0; }
	else { m_output = sum; }
}

//Sigmoid threshold
void Neuron::sigmoid() {
	double sum = 0.0;
	for (map<Neuron*, double>::iterator it = m_InputGotFromNeurons.begin(); it != m_InputGotFromNeurons.end(); ++it) {
		sum += it->second;
	}
	m_output = (1 / (1 + exp(-sum)));
}

//Feed forward the output into every neuron with its corresponding weight
void Neuron::feedForward() {
	double input;
	for (map<Neuron*, double>::iterator it = m_connNeuronsForward.begin(); it != m_connNeuronsForward.end(); ++it) {
		input = m_output*it->second;
		it->first->setInput(this, input);
	}
}

//Setting a value in the input map for the corresponding neuron
void Neuron::setInput(Neuron* inputNeuron, double input) {
	map<Neuron*, double>::iterator it = m_InputGotFromNeurons.find(inputNeuron);
	if (it != m_InputGotFromNeurons.end()) 
	{
		it->second = input;
	}
	//If this neuron is not yet in the map of the inputneurons add it
	if (m_InputGotFromNeurons.find(inputNeuron) == m_InputGotFromNeurons.end())
	{
		m_InputGotFromNeurons[inputNeuron] = input;
	}
}

//Setting the connected Neurons for the feed forward map via the topology
void Neuron::setConnNeurons(map<Neuron*,double> neurons) {
	for (map<Neuron*, double>::iterator it = neurons.begin(); it != neurons.end(); ++it) {
		m_connNeuronsForward[it->first] = it->second;
	}
}

//Getter for the outputvalue if this neuron is in the end of the neural network
double Neuron::getOutput() {
	return m_output;
}