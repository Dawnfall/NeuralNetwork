#pragma once

#include "NeuralNetwork.h"
#include "NeuralManager.h"
#include "MathUtils.h"

#include <vector>
#include<iostream>
#include<random>

class NeuralTests
{
public:
	static void Run()
	{
		Test_Equality();
		Test_Duplication();

		Test_Export();
		Test_ConstructFromData();

		Test_ForwardPropagation1();
		Test_ForwardPropagation2();

		Test_XOr();

		Test_Mutate();
		Test_CrossOver();

		//Test_BackPropagation1();
		//Test_BackPropagation2();


		//*****************
		// Neural manager

		Test_CreateNNs();
	}

private:
	//**************
	// Equality test

	static void Test_Equality()
	{
		std::vector<float> nn1
		{ 3.0f, 2.0f, 0.4f, -0.7f, -0.3f, -0.6f, -0.2f, 0.9f, -0.2f, 0.1f, -0.3f, 1.0f, 3.0f, 0.5f, -0.1f, 0.3f, 0.4f };
		std::vector<float> nn2
		{ 3.0f, 2.0f, 0.4f, -0.7f, -0.3f, -0.6f, -0.2f, 0.9f, -0.2f, 0.1f, -0.3f, 1.0f, 3.0f, 0.5f, -0.1f, 0.3f, 0.4f };
		std::vector<float> nn3
		{ 3.0f, 2.0f, 0.4f, -0.7f, -0.3f, -0.6f, -0.237188f, 0.9f, -0.2f, 0.1f, -0.3f, 1.0f, 3.0f, 0.489658f, -0.1f, 0.3f, 0.4f };

		if (nn1 != nn2 || nn1 == nn3 || nn2 == nn3)
		{
			std::cout << "Equality test fail!\n";
			return;
		}
		std::cout << "Equality test success!\n";
	}

	//*************
// Duplicate Test

	static void Test_Duplication()
	{
		std::vector<float> nnAsData
		{ 3.0f,2.0f,0.4f,-0.7f,-0.3f,-0.6f,-0.2f,0.9f,-0.2f,0.1f,-0.3f,1.0f,3.0f,0.5f,-0.1f,0.3f,0.4f };

		dawn::NeuralNetwork nn(nnAsData);

		std::vector<dawn::NeuralNetwork> duplicates = nn.Duplicate(5);

		if (duplicates.size() != 5)
		{
			std::cout << "Duplication test failed; incorrect size" << std::endl;
		}
		for (auto d : duplicates)
			if (d != nn)
			{
				std::cout << "Duplication test failed; duplicates not equal" << std::endl;
				std::cout << "Ori: " << MathUtils::VecToString(nn.ExportData()) << std::endl;
				std::cout << "New: " << MathUtils::VecToString(d.ExportData()) << std::endl;
				return;
			}

		std::cout << "Duplication test success" << std::endl;
	}

	//*************
	// Test 1

	static void Test_ForwardPropagation1()
	{
		std::vector<int> layers = { 2,3,1 };

		std::vector<float> in = { 1.0f,0.5f };

		std::vector<float> firsLayerFirstNeuronIn = { 0.4f,-0.7f };
		std::vector<float> firstLayerSecNeuronIn = { -0.3f,-0.6f };
		std::vector<float> firstLayerThirdNeuronIn = { -0.2f,0.9f };
		std::vector<float> secLayerFirstNeuronIn = { 0.5f,-0.1f,0.3f };

		std::vector<float>secLayerBiases = { -0.2f,0.1f,-0.3f };
		std::vector<float>thirdLayerBiases = { 0.4f };

		std::vector<float> expectedFirstLayer = { 0.4625701547f,0.3775406688f,0.4875026035f };
		std::vector<float> expectedSecLayer = { 0.6769481382f };

		dawn::NeuralNetwork n(
			{ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn,firstLayerThirdNeuronIn},{secLayerFirstNeuronIn} },
			{ secLayerBiases,thirdLayerBiases });

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

		std::vector<float> in = { 1.0f,0.5f };
		std::vector<float> target = { 0.8f };

		std::vector<float> firsLayerFirstNeuronIn = { 0.4f,-0.7f };
		std::vector<float> firstLayerSecNeuronIn = { -0.3f,-0.6f };
		std::vector<float> firstLayerThirdNeuronIn = { -0.2f,0.9f };
		std::vector<float> secLayerFirstNeuronIn = { 0.5f,-0.1f,0.3f };

		std::vector<float>secLayerBiases = { -0.2f,0.1f,-0.3f };
		std::vector<float>thirdLayerBiases = { 0.4f };

		dawn::NeuralNetwork n(
			{ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn,firstLayerThirdNeuronIn},{secLayerFirstNeuronIn} },
			{ secLayerBiases,thirdLayerBiases });

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

		std::vector<float> in = { 0.05f,0.1f };

		std::vector<float> firsLayerFirstNeuronIn = { 0.15f,0.2f };
		std::vector<float> firstLayerSecNeuronIn = { 0.25f,0.3f };
		std::vector<float> secLayerFirstNeuronIn = { 0.4f,0.45f };
		std::vector<float> secLayerSecNeuronIn = { 0.5f,0.55f };

		std::vector<float> firstLayerBiases = { 0.35f,0.35f };
		std::vector<float> secLayerBiases = { 0.6f,0.6f };

		std::vector<float> expectedFirstLayer = { 0.5932699921f,0.5968843783f };
		std::vector<float> expectedSecLayer = { 0.7513650696f,0.7729284653f };

		dawn::NeuralNetwork n(
			{ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn},{secLayerFirstNeuronIn,secLayerSecNeuronIn} },
			{ firstLayerBiases,secLayerBiases });

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

		std::vector<float> in = { 0.05f,0.1f };

		std::vector<float> firsLayerFirstNeuronIn = { 0.15f,0.2f };
		std::vector<float> firstLayerSecNeuronIn = { 0.25f,0.3f };
		std::vector<float> secLayerFirstNeuronIn = { 0.4f,0.45f };
		std::vector<float> secLayerSecNeuronIn = { 0.5f,0.55f };

		std::vector<float> firstLayerBiases = { 0.35f,0.35f };
		std::vector<float> secLayerBiases = { 0.6f,0.6f };

		std::vector<float> target = { 0.5f,0.2f };

		dawn::NeuralNetwork n(
			{ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn},{secLayerFirstNeuronIn,secLayerSecNeuronIn} },
			{ firstLayerBiases,secLayerBiases });

		dawn::TrainData d;
		d.input = in;
		d.target = target;

		float learnRate = 0.15f;

		for (int i = 0; i < 250; i++)
			n.BackPropagate(d, learnRate);
	}

	//*************
	// Full Test

	static void Test_XOr()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::vector<int> layers = { 2,4,1 };

		std::vector<float> out1{ 0.99f };
		std::vector<float> out2{ 0.01f };

		std::vector<float> in1{ 0.99f,0.99f };
		std::vector<float> in2{ 0.99f,0.01f };
		std::vector<float> in3{ 0.01f,0.99f };
		std::vector<float> in4{ 0.01f,0.01f };

		dawn::TrainData train1 = { in1,out2 };
		dawn::TrainData train2 = { in2,out1 };
		dawn::TrainData train3 = { in3,out1 };
		dawn::TrainData train4 = { in4,out2 };

		std::vector <dawn::TrainData> trainData{ train1,train2,train3,train4 };//, train2, train3, train4

		dawn::TrainParams params;
		params.learnRate = 0.3f;
		params.maxEpoch = 10000;
		params.minEpoch = 10000;
		params.minError = 0.1f;

		dawn::NeuralNetwork n(layers, gen);
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
			MathUtils::Print("XOR test success\n");
	}

	//*************
	// Export and Import Test

	static void Test_Export()
	{
		std::vector<float> firsLayerFirstNeuronIn = { 0.4f,-0.7f };
		std::vector<float> firstLayerSecNeuronIn = { -0.3f,-0.6f };
		std::vector<float> firstLayerThirdNeuronIn = { -0.2f,0.9f };
		std::vector<float> secLayerFirstNeuronIn = { 0.5f,-0.1f,0.3f };

		std::vector<float>secLayerBiases = { -0.2f,0.1f,-0.3f };
		std::vector<float>thirdLayerBiases = { 0.4f };

		std::vector<float> expectedFirstLayer = { 0.4625701547f,0.3775406688f,0.4875026035f };
		std::vector<float> expectedSecLayer = { 0.6769481382f };

		dawn::NeuralNetwork n(
			{ {firsLayerFirstNeuronIn,firstLayerSecNeuronIn,firstLayerThirdNeuronIn},{secLayerFirstNeuronIn} },
			{ secLayerBiases,thirdLayerBiases });

		std::vector<float> data = n.ExportData();
		std::vector<float> expectedResult{ 3.0f,2.0f,0.4f,-0.7f,-0.3f,-0.6f,-0.2f,0.9f,-0.2f,0.1f,-0.3f,1.0f,3.0f,0.5f,-0.1f,0.3f,0.4f };

		if (!MathUtils::AreEqual(data, expectedResult))
		{
			std::cout << "Export test fail!\n";
			return;
		}

		std::cout << "Export test success!\n";
	}
	static void Test_ConstructFromData()
	{
		std::vector<float> nnAsData
		{ 3.0f,2.0f,

		 0.4f,-0.7f,
		 -0.3f,-0.6f,
		 -0.2f,0.9f,

		-0.2f,0.1f,-0.3f,

		1.0f,3.0f,

		0.5f,-0.1f,0.3f,

		0.4f };

		dawn::NeuralNetwork nn(nnAsData);
		std::vector<float> nnDataOutput = nn.ExportData();

		if (!MathUtils::AreEqual(nnAsData, nnDataOutput))
		{
			std::cout << "Construct from data test fail!\n";
			return;
		}

		std::cout << "Construct from data test success!\n";
	}



	//*************
	// Genetic tests
	static void Test_Mutate()
	{
		std::vector<float> nnAsData
		{ 3.0f,2.0f,0.4f,-0.7f,-0.3f,-0.6f,-0.2f,0.9f,-0.2f,0.1f,-0.3f,1.0f,3.0f,0.5f,-0.1f,0.3f,0.4f };

		std::vector<float> nnAsDataExpected
		{ 3.0f,2.0f,0.4f,-0.7f,-0.3f,-0.6f,-0.237188f,0.9f,-0.2f,0.1f,-0.3f,1.0f,3.0f,0.489658f,-0.1f,0.3f,0.4f };

		float changeProb = 0.1f;
		float maxRange = 0.05f;

		std::mt19937 gen(1);

		dawn::NeuralNetwork nn(nnAsData);
		nn.Mutate(changeProb, maxRange, gen);

		std::vector<float> nnAsDataAfterMutate = nn.ExportData();

		if (!MathUtils::AreEqual(nnAsDataAfterMutate, nnAsDataExpected))
		{
			std::cout << MathUtils::VecToString(nnAsDataAfterMutate) << std::endl;
			std::cout << MathUtils::VecToString(nnAsDataExpected) << std::endl;

			std::cout << "Mutate test fail!\n";
			return;
		}
		std::cout << "Mutate test success!\n";
	}
	static void Test_CrossOver()
	{
		std::vector<float> nnAsData1
		{ 3.0f,2.0f,-1.0f,-1.1f,-1.2f,-1.3f,-1.4f,-1.5f,-1.6f,-1.7f,-1.8f,1.0f,3.0f,-1.9f,-2.0f,-2.1f,-2.2f };

		std::vector<float> nnAsData2
		{ 3.0f,2.0f,3.0f,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f,3.8f,1.0f,3.0f,3.9f,4.0f,4.1f,4.2f };

		std::vector<float> nnResultExpected
		{ 3.0f,2.0f,3.0f,-1.1f,-1.2f,3.3f,3.4f,-1.5f,-1.6f,3.7f,3.8f,1.0f,3.0f,3.9f,4.0f,4.1f,4.2f };

		dawn::NeuralNetwork nn1 = dawn::NeuralNetwork(nnAsData1);
		dawn::NeuralNetwork nn2 = dawn::NeuralNetwork(nnAsData2);

		std::mt19937 gen(1);
		dawn::NeuralNetwork nnResult = dawn::NeuralNetwork::CrossOver(nn1, nn2, gen);
		std::vector<float> nnResultAsData = nnResult.ExportData();

		if (!MathUtils::AreEqual(nnResultExpected, nnResultAsData))
		{
			std::cout << "Crossover test fail!" << std::endl;
			std::cout << "Exp: " << MathUtils::VecToString(nnResultExpected) << std::endl;
			std::cout << "Rec: " << MathUtils::VecToString(nnResultAsData) << std::endl;
			return;
		}
		std::cout << "Crossover test success!\n";

	}

	//**************************
	// Neural Manager

	//*************
	// Create NNs

	static void Test_CreateNNs()
	{
		dawn::NeuralManager manager;
		std::vector<int> indices = manager.CreateNNs({ 3,2,1 }, 3);
		if (indices.size() != 3)
		{
			std::cout << "Create NNs on Neural manager failed! " << std::endl;
			return;
		}

		std::cout << "CreateNNs success!";
	}

};