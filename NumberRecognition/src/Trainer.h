#pragma once
#include "NeuralManager.h"
#include <vector>

std::vector<int> layerSizes = { 64,128,32,1 };

class Trainer
{
	dawn::NeuralManager m_nnManager;

	void CreateNN()
	{
		m_nnManager.CreateNNs(layerSizes, 1);
	}


};