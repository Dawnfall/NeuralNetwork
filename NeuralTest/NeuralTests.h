#pragma once

#include "../NeuralNetwork/NeuralNetwork.h"

#include <vector>
#include<iostream>

class NeuralTests
{
public:
	static void Run()
	{
		//Test_ForwardPropagation1();
		//Test_ForwardPropagation2();
		//Test_BackPropagation1();
		//Test_BackPropagation2();
		Test_XOr();
	}

private:
	//*************
	// Test 1

	static void Test_ForwardPropagation1()
	{
		std::vector<int> layers = { 2,3,1 };

		std::vector<float> in = { 1.0,0.5 };

		std::vector<float> firsLayerFirstNeuronIn = { 0.4,-0.7 };
		std::vector<float> firstLayerSecNeuronIn = { -0.3,-0.6 };
		std::vector<float> firstLayerThirdNeuronIn = { -0.2,0.9 };
		std::vector<float> secLayerFirstNeuronIn = { 0.5,-0.1,0.3 };

		std::vector<float>firstLayerBiases = { -0.2,0.1,-0.3 };
		std::vector<float>secLayerBiases = { 0.4 };

		std::vector<float> expectedFirstLayer = { 0.4625701547,0.3775406688,0.4875026035 };
		std::vector<float> expectedSecLayer = { 0.6769481382 };

		dawn::NeuralNetwork n(layers);

		n.SetConnections({ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn,firstLayerThirdNeuronIn},{secLayerFirstNeuronIn} });
		n.SetBiases({ firstLayerBiases,secLayerBiases });
		n.FeedForward(in);

		std::vector<float> firstLayerOut = n.GetLayer(1);
		std::vector<float> secLayerOut = n.GetLayer(2);

		bool isOk = true;
		if (!MathUtils::AreEqual(firstLayerOut, expectedFirstLayer))
		{
			isOk = false;
			MathUtils::Print("Feedforward first layer not match\n");
		}
		if (!MathUtils::AreEqual(secLayerOut, expectedSecLayer))
		{
			isOk = false;
			MathUtils::Print("Feedforward out layer not match\n");
		}
		if (isOk)
		{
			MathUtils::Print("First Test ; Feedforward success\n");
		}
	}

	static void Test_BackPropagation1()
	{
		std::vector<int> layers = { 2,3,1 };

		std::vector<float> in = { 1.0,0.5 };
		std::vector<float> target = { 0.8 };

		std::vector<float> firsLayerFirstNeuronIn = { 0.4,-0.7 };
		std::vector<float> firstLayerSecNeuronIn = { -0.3,-0.6 };
		std::vector<float> firstLayerThirdNeuronIn = { -0.2,0.9 };
		std::vector<float> secLayerFirstNeuronIn = { 0.5,-0.1,0.3 };

		std::vector<float>firstLayerBiases = { -0.2,0.1,-0.3 };
		std::vector<float>secLayerBiases = { 0.4 };

		dawn::NeuralNetwork n(layers, MathUtils::Sigmoid, MathUtils::SigmoidDerivativeFromSigmoidInputVec);

		n.SetConnections({ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn,firstLayerThirdNeuronIn},{secLayerFirstNeuronIn} });
		n.SetBiases({ firstLayerBiases,secLayerBiases });

		dawn::TrainData d;
		d.input = in;
		d.target = target;

		float learnRate = 0.5;

		for (int i = 0; i < 1000; i++)
			n.BackPropagate(d, learnRate);
	}


	//*************
	// Test 2

	static void Test_ForwardPropagation2()
	{
		std::vector<int> layers = { 2,2,2 };

		std::vector<float> in = { 0.05,0.1 };

		std::vector<float> firsLayerFirstNeuronIn = { 0.15,0.2 };
		std::vector<float> firstLayerSecNeuronIn = { 0.25,0.3 };
		std::vector<float> secLayerFirstNeuronIn = { 0.4,0.45 };
		std::vector<float> secLayerSecNeuronIn = { 0.5,0.55 };

		std::vector<float> firstLayerBiases = { 0.35,0.35 };
		std::vector<float> secLayerBiases = { 0.6,0.6 };

		std::vector<float> expectedFirstLayer = { 0.5932699921,0.5968843783 };
		std::vector<float> expectedSecLayer = { 0.7513650696,0.7729284653 };

		dawn::NeuralNetwork n(layers, MathUtils::Sigmoid, MathUtils::SigmoidDerivativeFromSigmoidInputVec);

		n.SetConnections({ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn},{secLayerFirstNeuronIn,secLayerSecNeuronIn} });
		n.SetBiases({ firstLayerBiases,secLayerBiases });

		n.FeedForward(in);

		std::vector<float> firstLayerOut = n.GetLayer(1);
		std::vector<float> secLayerOut = n.GetLayer(2);

		bool isOk = true;
		if (!MathUtils::AreEqual(firstLayerOut, expectedFirstLayer))
		{
			isOk = false;
			MathUtils::Print("Sec Test;Feedforward first layer not match\n");
		}
		if (!MathUtils::AreEqual(secLayerOut, expectedSecLayer))
		{
			isOk = false;
			MathUtils::Print("Sec Test;Feedforward out layer not match\n");
		}
		if (isOk)
		{
			MathUtils::Print("Sec Test;Feedforward success\n");
		}
	}

	static void Test_BackPropagation2()
	{
		std::vector<int> layers = { 2,2,2,2 };

		std::vector<float> in = { 0.05,0.1 };

		std::vector<float> firsLayerFirstNeuronIn = { 0.15,0.2 };
		std::vector<float> firstLayerSecNeuronIn = { 0.25,0.3 };
		std::vector<float> secLayerFirstNeuronIn = { 0.4,0.45 };
		std::vector<float> secLayerSecNeuronIn = { 0.5,0.55 };

		std::vector<float> firstLayerBiases = { 0.35,0.35 };
		std::vector<float> secLayerBiases = { 0.6,0.6 };

		std::vector<float> target = { 0.5,0.2 };

		dawn::NeuralNetwork n(layers, MathUtils::Sigmoid, MathUtils::SigmoidDerivativeFromSigmoidInputVec);

		n.SetConnections({ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn},{secLayerFirstNeuronIn,secLayerSecNeuronIn} });
		n.SetBiases({ firstLayerBiases,secLayerBiases });

		dawn::TrainData d;
		d.input = in;
		d.target = target;

		float learnRate = 0.15;

		for (int i = 0; i < 250; i++)
			n.BackPropagate(d, learnRate);
	}

	//*************
	// Full Test

	static void Test_XOr()
	{
		std::vector<int> layers = { 2,3,1 };

		std::vector<float> out1{ 0.99 };
		std::vector<float> out2{ 0.01 };

		std::vector<float> in1{ 0.99,0.99 };
		std::vector<float> in2{ 0.99,0.01 };
		std::vector<float> in3{ 0.01,0.99 };
		std::vector<float> in4{ 0.01,0.01 };

		dawn::TrainData train1 = { in1,out2 };
		dawn::TrainData train2 = { in2,out1 };
		dawn::TrainData train3 = { in3,out1 };
		dawn::TrainData train4 = { in4,out2 };

		std::vector <dawn::TrainData> trainData{ train1,train2,train3,train4 };//, train2, train3, train4
	
		dawn::TrainParams params;
		params.learnRate = 0.3;
		params.maxEpoch = 10000;
		params.minEpoch = 10000;
		params.minError = 0.1;

		dawn::NeuralNetwork n(layers, MathUtils::Sigmoid, MathUtils::SigmoidDerivativeFromSigmoidInputVec);
		n.Train(trainData, params);

		//test
		bool isOk = true;
		if (n.FeedForward(in1)[0] > 0.1)
		{
			MathUtils::Print("Test 1 failed\n");
			isOk = false;
		}
		if (n.FeedForward(in2)[0] < 0.9)
		{
			MathUtils::Print("Test 2 failed\n");
			isOk = false;
		}
		if (n.FeedForward(in3)[0] < 0.9)
		{
			MathUtils::Print("Test 3 failed\n");
			isOk = false;
		}
		if (n.FeedForward(in4)[0] > 0.1)
		{
			MathUtils::Print("Test 4 failed\n");
			isOk = false;
		}
		if (isOk)
			MathUtils::Print("XOR test success");
	}


};