#pragma once

#include "../NeuralNetwork/NeuralNetwork.h"

#include <vector>
#include<iostream>

class NeuralTests
{
public:
	static void Run()
	{
		Test_ForwardPropagation();
		Test_BackPropagation();
		//Test_XOr();
	}

private:
	static void Test_ForwardPropagation()
	{
		std::vector<int> layers = { 2,3,1 };

		std::vector<float> in = { 1.0,0.5 };

		std::vector<float> bias1 = { -0.2,0.1,-0.3 };
		std::vector<float> bias2 = { 0.4 };

		std::vector<float> firsLayerFirstNeuronIn = { 0.4,-0.7 };
		std::vector<float> firstLayerSecNeuronIn = { -0.3,-0.6 };
		std::vector<float> firstLayerThirdNeuronIn = { -0.2,0.9 };
		std::vector<float> secLayerFirstNeuronIn = { 0.5,-0.1,0.3 };

		std::vector<float> expectedFirstLayer = { 0.4625701547,0.3775406688,0.4875026035 };
		float expectedOut = 0.6769481382;

		dawn::NeuralNetwork n(layers, MathUtils::Sigmoid, MathUtils::SigmoidDerivative);
		n.SetBiases({ bias1,bias2 });
		n.SetConnections({ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn,firstLayerThirdNeuronIn},{secLayerFirstNeuronIn} });

		n.FeedForward(in);

		MathUtils::Print(n.ToString());

		bool isOk = true;
		if (!MathUtils::AreEqual(n.GetOutputLayer()[0], expectedOut))
		{
			isOk = false;
			MathUtils::Print("Feedforward output not match\n");
		}

		std::vector<float> firstLayer = n.GetLayer(1);
		for (int i = 0; i < expectedFirstLayer.size(); i++)
		{
			if (!MathUtils::AreEqual(expectedFirstLayer[i], firstLayer[i]))
			{
				isOk = false;
				MathUtils::Print("Feedforward first layer not match\n");
			}
		}


		if (isOk)
		{
			MathUtils::Print("Feedforward success\n");
		}
	}

	static void Test_BackPropagation()
	{
		std::vector<int> layers = { 2,3,1 };

		std::vector<float> in = { 1.0,0.5 };
		std::vector<float> target = { 0.8 };

		std::vector<float> bias1 = { -0.2,0.1,-0.3 };
		std::vector<float> bias2 = { 0.4 };

		std::vector<float> firsLayerFirstNeuronIn = { 0.4,-0.7 };
		std::vector<float> firstLayerSecNeuronIn = { -0.3,-0.6 };
		std::vector<float> firstLayerThirdNeuronIn = { -0.2,0.9 };
		std::vector<float> secLayerFirstNeuronIn = { 0.5,-0.1,0.3 };

		dawn::NeuralNetwork n(layers, MathUtils::Sigmoid, MathUtils::SigmoidDerivative);
		n.SetBiases({ bias1,bias2 });
		n.SetConnections({ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn,firstLayerThirdNeuronIn},{secLayerFirstNeuronIn} });

		dawn::TrainData d;
		d.input = in;
		d.target = target;

		float learnRate = 0.2;

		n.BackPropagate(d, learnRate);
	}

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

		std::vector <dawn::TrainData> trainData{ train1,train2,train3,train4 };
		dawn::TrainParams params;
		params.learnRate = 0.02;
		params.maxEpoch = 1000;
		params.minEpoch = 1000;
		params.minError = 0.02;

		dawn::NeuralNetwork n(layers, MathUtils::Sigmoid, MathUtils::SigmoidDerivative);
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