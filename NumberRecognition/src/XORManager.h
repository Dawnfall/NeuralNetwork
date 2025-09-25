#pragma once

#include <string>
#include "Utils.h"

#include "NeuralNetwork.h"

class XORManager
{
public:

	float getXORLearnRate()
	{
		try
		{
			return std::stof(Xor_m_learnRateInput);
		}
		catch (...)
		{
			Utils::printErr("Invalid learning rate input");
			return 0.0f;
		}
	}
	int getXORMaxTrain()
	{
		try
		{
			return  std::stoi(Xor_maxTrainInput);
		}
		catch (...)
		{
			Utils::printErr("Invalid train count");
			return 0;
		}
	}
	float getXORMinError()
	{
		try
		{
			return  std::stof(Xor_minErrInput);
		}
		catch (...)
		{
			Utils::printErr("Invalid min Error");
			return 0.0f;
		}
	}
	int getXORInputA()
	{
		try
		{
			return  std::stof(Xor_test_aInput);
		}
		catch (...)
		{
			Utils::printErr("Invalid input A");
			return 0.0f;
		}
	}
	int getXORInputB()
	{
		try
		{
			return  std::stof(Xor_test_bInput);
		}
		catch (...)
		{
			Utils::printErr("Invalid input B");
			return 0.0f;
		}
	}

	dawn::NeuralNetwork* GetNN() { return m_nn ? m_nn.get() : nullptr; }

	void Train();
	void Run();
	void CreateNewNN();

	std::string Xor_NN_text = "Neural network: ";

	//xor
	std::string Xor_layersInput = "2 3 1";
	std::string Xor_maxTrainInput = "10000";
	std::string Xor_m_learnRateInput = "0.01";
	std::string Xor_minErrInput = "0.05";
	std::string Xor_test_resultText = "";
	std::string Xor_test_aInput = "";
	std::string Xor_test_bInput = "";

private:
	std::unique_ptr<dawn::NeuralNetwork> m_nn = nullptr;

	const std::vector<std::vector<float>> xor_inputs =
	{
		{ 0.99f,0.99f },
		{ 0.99f,0.01f },
		{ 0.01f,0.99f },
		{ 0.01f,0.01f }
	};

	const std::vector<std::vector<float>> xor_outputs =
	{
		{ 0.01f },
		{ 0.99f},
		{ 0.99f },
		{ 0.01f }
	};
};