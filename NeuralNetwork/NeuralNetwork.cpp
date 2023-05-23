#include "NeuralNetwork.h"

#include <sstream>
#include <iostream>
#include <random>

namespace dawn
{
	NeuralNetwork::NeuralNetwork(const std::vector<int>& layers, float(*activation)(float), float(*activationDerivative)(float))
	{
		m_activation = activation;
		m_activationDerivative = activationDerivative;

		for (int i = 0; i < layers.size(); i++)
		{
			if (i < layers.size() - 1)
			{
				m_neurons.emplace_back(Eigen::VectorXf::Zero(layers[i] + 1));
				m_neurons[i][m_neurons[i].size() - 1] = 1;
			}
			else
				m_neurons.emplace_back(Eigen::VectorXf::Zero(layers[i]));

			if (i > 0)
			{
				m_errors.emplace_back(Eigen::VectorXf::Zero(layers[i] + 1));
				m_weights.emplace_back(Eigen::MatrixXf(layers[i], layers[i - 1] + 1));
			}
		}

		InitWeights();
	}

	void NeuralNetwork::InitWeights()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		for (Eigen::MatrixXf& weightLayer : m_weights)
		{
			float rangeMax = 1 / std::sqrt(weightLayer.cols());
			std::uniform_real_distribution<float> dist(-rangeMax, rangeMax);

			for (int row = 0; row < weightLayer.rows(); row++)
				for (int col = 0; col < weightLayer.cols(); col++)
					weightLayer(row, col) = dist(gen);
		}
	}


	void NeuralNetwork::SetConnections(const std::vector<std::vector<std::vector<float>>>& connLayers)
	{
		for (int i = 0; i < connLayers.size(); i++)
			for (int row = 0; row < connLayers[i].size(); row++)
				for (int col = 0; col < connLayers[i][row].size(); col++)
					m_weights[i](row, col) = connLayers[i][row][col];
	}

	void NeuralNetwork::SetActivation(float(*activation)(float))
	{
		m_activation = activation;
	}

	std::vector<float> NeuralNetwork::FeedForward(const std::vector<float>& inputs)
	{
		SetInputLayer(inputs);

		for (int i = 0; i < m_weights.size(); i++)
		{
			int size = (i == m_weights.size() - 1) ? m_neurons[i + 1].size() : m_neurons[i + 1].size() - 1;

			m_neurons[i + 1].head(size) = m_weights[i] * m_neurons[i];
			for (int j = 0; j < size; j++)
				m_neurons[i + 1][j] = m_activation(m_neurons[i + 1][j]);
		}

		return GetOutputLayer();
	}

	void NeuralNetwork::BackPropagate(const TrainData& data, float learnRate)
	{
		std::vector<float> output = FeedForward(data.input);

		//output layer has simple error calculation
		MathUtils::SubstractVec(output, data.target, m_errors[m_errors.size() - 1]);

		float errorSum = MathUtils::SumSquaredVector(m_errors[m_errors.size() - 1]);

		std::cout << "Error: " << m_errors[m_errors.size() - 1] << std::endl;
		std::cout << "In: " << data.input[0] << " , " << data.input[1]
			<< " ;Out: " << output[0] << " ;Target: " << data.target[0] << " ;SumErrorSquared: " << errorSum << std::endl;


		//1. iterate back through neurons/weights and calculate errors for each layer
		//2. calculate derivative of activation for each layer

		for (int i = m_errors.size() - 1; i > 0; i--)
		{
			for (int j = 0; j < m_errors[i].size(); j++)
				m_errors[i][j] *= m_neurons[i + 1][j] * (1.0 - m_neurons[i + 1][j]);
			//m_errors[i][j] *= m_activationDerivative(m_neurons[i + 1][j]);
			m_errors[i - 1] = m_weights[i].transpose() * m_errors[i];
		}

		//MathUtils::Print(ToString());
		//std::cout << "Errors:" << std::endl;
		//std::cout << m_errors[1] << std::endl << std::endl;
		//std::cout << m_errors[0] << std::endl << std::endl;

		//for (int i = 0; i < m_errors.size(); i++)
		//	for (int j = 0; j < m_errors[i].size(); j++)
		//		m_errors[i][j] *= m_activationDerivative(m_neurons[i + 1][j]);;

		//std::cout << "Errors:" << std::endl;
		//std::cout << m_errors[1] << std::endl << std::endl;
		//std::cout << m_errors[0] << std::endl << std::endl;

		for (int i = 0; i < GetLayerCount() - 1; i++)
		{
			m_weights[i] -= learnRate * m_errors[i] * m_neurons[i].transpose();
		}
	}

	void NeuralNetwork::Train(const std::vector<TrainData>& data, const TrainParams& params)
	{
		//Train(data[0], params.learnRate);

		int epochCount = 0;
		while (epochCount <= params.maxEpoch)
		{
			for (const TrainData& d : data)
			{
				BackPropagate(d, params.learnRate);
				//printf((std::to_string(err) + "\n").c_str());
			}

			epochCount++;
		}
	}

	void NeuralNetwork::TempTrain(const std::vector<TrainData>& data, const TrainParams& params)
	{
		//int epochCount = 0;
		//while (epochCount <= params.maxEpoch)
		//{
		//	for (const TrainData& d : data)
		//	{
		//		SetInputLayer(d.input);

		//		//feed forward
		//		for (int i = 0; i < m_weights.size(); i++)
		//		{
		//			//Eigen::VectorXf nextLayer = (m_weights[i] * m_neurons[i]) + m_biases[i];

		//			//for (int j = 0; j < m_neurons[i + 1].size(); j++)
		//			//	m_neurons[i + 1][j] = m_activation(nextLayer[j]);
		//		}

		//		//calculate error based on target
		//		MathUtils::SubstractVec(d.target, GetOutputLayer(), m_errors[m_errors.size() - 1]);

		//		//backpropagation
		//		for (int i = m_errors.size() - 1; i > 0; i--)
		//		{
		//			for (int j = 0; j < m_errors[i].size(); j++)
		//				m_errors[i][j] *= m_activationDerivative(m_neurons[i + 1][j]);
		//			m_errors[i - 1] = m_weights[i].transpose() * m_errors[i];
		//		}

		//		for (int i = 0; i < GetLayerCount() - 1; i++)
		//		{
		//			m_weights[i] -= params.learnRate * m_errors[i] * m_neurons[i].transpose();
		//			m_biases[i] -= params.learnRate * m_errors[i];
		//		}
		//	}

		//	epochCount++;
		//}
	}

	std::string NeuralNetwork::ToString()
	{
		std::stringstream ss;

		ss << "Neurons:\n";
		for (int i = 0; i < m_neurons.size(); i++)
			ss << "\nLayer " << std::to_string(i) << ":\n" << m_neurons[i];

		//ss << "\n\nBiases:\n";
		//for (int i = 0; i < m_biases.size(); i++)
		//	ss << "\nLayer " << std::to_string(i + 1) << ":\n" << m_biases[i];

		ss << "\n\nConnections:\n";
		for (int i = 0; i < m_weights.size(); i++)
			ss << "\nLayer " << std::to_string(i) << ":\n" << m_weights[i];
		ss << "\n\n";

		return ss.str();
	}
}