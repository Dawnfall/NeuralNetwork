//#include "../NeuralNetwork/NeuralNetwork.h"
//#include "../NeuralNetwork/Data.h"

#include "NeuralTests.h"

#include <vector>
#include <stdio.h>
#include <iostream>


//
//
//dawn::NeuralNetwork CreateTestNetwork()
//{
//	std::vector<int> layers = { 2,3,2 };
//
//	std::vector<float> firstLayerFirstNeuronConns = { 0.1,0.3,0.6 };
//	std::vector<float> firstLayerSecNeuronConns = { 0.8,0.2,0.5 };
//
//	std::vector<float> secLayerFirstNeuronConns = { 0.2,0.8 };
//	std::vector<float> secLayerSecNeuronConns = { 0.1,0.9 };
//	std::vector<float> secLayerthirdNeuronConns = { 0.2,0.7 };
//
//	std::vector<float> secLayerBiases = { 0.8,0.3,0.1 };
//	std::vector<float> thirdLayerBiases = { 0.2,0.6 };
//
//	dawn::NeuralNetwork n(layers, MathUtils::Sigmoid, MathUtils::SigmoidDerivative);
//
//	n.SetBiases({ secLayerBiases,thirdLayerBiases });
//	n.SetConnections({ {firstLayerFirstNeuronConns,firstLayerSecNeuronConns},{secLayerFirstNeuronConns,secLayerSecNeuronConns,secLayerthirdNeuronConns} });
//
//	return n;
//}
//


int main()
{
	NeuralTests::Run();

	//TestXOr();
	//TestBackpropagation();
	//NeuralNetwork n = CreateTestNetwork();
	//printf(n.Print().c_str());

	//n.FeedForward({ 1,0.5 });

	//printf(n.Print().c_str());
	return 0;
}



