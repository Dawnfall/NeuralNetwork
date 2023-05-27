#include "NeuralNetwork.h"

#include <sstream>
#include <iostream>
#include <random>

namespace dawn
{
	NeuralNetwork::NeuralNetwork(const std::vector<int>& layers, float(*activation)(float), Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&))
	{
		m_layers = layers;
		m_activation = activation;
		m_activationDerivative = activationDerivative;

		std::random_device rd;
		std::mt19937 gen(rd());
		InitWeights(gen);
		InitBiases(gen);

		InitNeurons();
	}

	void NeuralNetwork::InitWeights(std::mt19937& gen)
	{
		for (int i = 0; i < m_layers.size() - 1; i++)
		{
			m_weights.emplace_back(Eigen::MatrixXf(m_layers[i + 1], m_layers[i]));

			float rangeMax = 1 / std::sqrt(m_weights[i].cols());
			std::uniform_real_distribution<float> dist(-rangeMax, rangeMax);

			for (int row = 0; row < m_weights[i].rows(); row++)
				for (int col = 0; col < m_weights[i].cols(); col++)
					m_weights[i](row, col) = dist(gen);
		}
	}

	void NeuralNetwork::InitBiases(std::mt19937& gen)
	{
		for (int i = 1; i < m_layers.size(); i++)
		{
			m_biases.emplace_back(Eigen::VectorXf(m_layers[i]));

			float rangeMax = 1 / std::sqrt(m_weights[i - 1].cols());
			std::uniform_real_distribution<float> dist(-rangeMax, rangeMax);

			for (float& v : m_biases.back())
				v = dist(gen);
		}
	}
	void NeuralNetwork::InitNeurons()
	{
		for (int i = 0; i < m_layers.size(); i++)
			m_neurons.emplace_back(Eigen::VectorXf::Zero(m_layers[i]));
	}

	void NeuralNetwork::SetConnections(const std::vector<std::vector<std::vector<float>>>& connLayers)
	{
		for (int i = 0; i < connLayers.size(); i++)
			for (int row = 0; row < connLayers[i].size(); row++)
				for (int col = 0; col < connLayers[i][row].size(); col++)
					m_weights[i](row, col) = connLayers[i][row][col];
	}
	void NeuralNetwork::SetBiases(const std::vector<std::vector<float>>& biases)
	{
		for (int i = 0; i < biases.size(); i++)
			for (int j = 0; j < biases[i].size(); j++)
				m_biases[i][j] = biases[i][j];
	}

	std::vector<float> NeuralNetwork::FeedForward(const std::vector<float>& inputs)
	{
		SetInputLayer(inputs);

		for (int i = 0; i < m_weights.size(); i++)
		{
			m_neurons[i + 1] = m_weights[i] * m_neurons[i] + m_biases[i];
			for (float& value : m_neurons[i + 1])
				value = m_activation(value);
		}

		//std::cout << ToString() << std::endl;
		return GetOutputLayer();
	}

	void NeuralNetwork::BackPropagate(const TrainData& data, float learnRate)
	{
		std::vector<Eigen::VectorXf> errors(GetLayerCount() - 1);
		errors.back() = Eigen::VectorXf::Zero(m_layers.back());

		std::vector<float> output = FeedForward(data.input);

		//std::cout << "NeuralNetwork:" << std::endl << ToString() << std::endl;
		//std::cout << "Output: " << MathUtils::VecToString(output) << std::endl;
		//std::cout << "Target: " << MathUtils::VecToString(data.target) << std::endl;

		//MathUtils::SubstractVec(data.target, output, errors.back());
		MathUtils::SubstractVec(output, data.target, errors.back());

		//std::cout << "Output errors: " << errors.back() << std::endl;
		std::cout << "Err squared: " << MathUtils::SumSquaredVector(errors.back()) << std::endl;

		Eigen::VectorXf der = m_activationDerivative(m_neurons.back());
		//std::cout << "Out Der: " << der << std::endl;

		errors.back() =errors.back().cwiseProduct(der);
		//std::cout << "ErrOut:" << std::endl << errors.back() << std::endl;

		for (int i = GetLayerCount() - 2; i > 0; i--)
		{
			auto a = m_weights[i].transpose() * errors[i];
			auto b = m_activationDerivative(m_neurons[i]);
			//std::cout << "A: " << std::endl << a << std::endl << "B:" << std::endl << b << std::endl;

			auto c = a.cwiseProduct(b);

			//std::cout << "C: " << std::endl << c << std::endl;

			errors[i - 1] = c;
		}

		for (int i = 0; i < errors.size(); i++)
		{
			auto gradient = errors[i] * m_neurons[i].transpose();
			//std::cout << "DelWe: " << i << ":" << std::endl << gradient << std::endl;

			//std::cout << "Err " << i << ":" << std::endl << m_errors[i] << std::endl;
			//std::cout << "Neuron " << i << ":" << std::endl << m_neurons[i] << std::endl;
			//std::cout << "Gradient:" << std::endl << gradient << std::endl;

			//std::cout << "Weights pre " << i << ":" << std::endl << m_weights[i] << std::endl;
			m_weights[i] -= learnRate * gradient;
			//std::cout << "Weights after " << i << ":" << std::endl << m_weights[i] << std::endl;

			m_biases[i] -= learnRate * errors[i];
		}

		//output = FeedForward(data.input);
		//MathUtils::SubstractVec(output, data.target, m_errors[m_errors.size() - 1]);
		//std::cout << "Err squared: " << MathUtils::SumSquaredVector(m_errors.back());


		//		std::vector<float> output = FeedForward(data.input);
		//
		//		//output layer has simple error calculation
		//		MathUtils::SubstractVec(output, data.target, m_errors[m_errors.size() - 1]);
		//
		//		std::cout << ToString() << std::endl;
		//		// Propagate the error backwards through the layers
		//		for (int i = m_errors.size() - 1; i > 0; --i)
		//		{
		//			std::cout << "Err-1:" << std::endl << m_errors[i - 1] << std::endl << std::endl;
		//			std::cout << "Err:" << std::endl << m_errors[i] << std::endl << std::endl;
		//
		//			auto a = m_errors[i].cwiseProduct(m_activationDerivative(m_neurons[i + 1]));
		//			std::cout << "a:" << std::endl << a << std::endl << std::endl;
		//
		//			auto b = m_activationDerivative(m_neurons[i + 1]);
		//			std::cout << "b:" << std::endl << b << std::endl << std::endl;
		//
		//			auto c = a.cwiseProduct(b);
		//			m_errors[i] = m_weights[i].transpose() * m_errors[i].cwiseProduct(m_activationDerivative(m_neurons[i + 1]));
		//		}
		//
		//		for (int i = 0; i < m_weights.size(); ++i)
		//		{
		//			std::cout << "we:" << std::endl << m_weights[i] << std::endl << std::endl;
		//			std::cout << "er:" << std::endl << m_errors[i] << std::endl << std::endl;
		//			std::cout << "ne:" << std::endl << m_neurons[i] << std::endl << std::endl;
		//			std::cout << "bi:" << std::endl << m_biases[i] << std::endl << std::endl;
		//
		////			m_weights[i] -= learnRate * (m_errors[i] * m_neurons[i].transpose());
		//			m_biases[i] -= learnRate * m_errors[i];
		//		}
				//float errorSum = MathUtils::SumSquaredVector(m_errors[m_errors.size() - 1]);

				////std::cout << "Error: " << m_errors[m_errors.size() - 1] << std::endl;
				//std::cout << "In: " << data.input[0] << " , " << data.input[1]
				//	<< " ;Out: " << output[0] << " ;Target: " << data.target[0] << " ;SumErrorSquared: " << errorSum << std::endl;


				////1. iterate back through neurons/weights and calculate errors for each layer
				////2. calculate derivative of activation for each layer

				//for (int i = m_errors.size() - 1; i > 0; i--)
				//{
				//	//std::cout << "Err-1:" << std::endl << m_errors[i - 1] << std::endl << std::endl;
				//	//std::cout << "Err:" << std::endl << m_errors[i] << std::endl << std::endl;
				//	//std::cout << "We:" << std::endl << m_weights[i] << std::endl << std::endl;
				//	m_errors[i - 1] = m_weights[i].leftCols(m_weights[i].cols() - 1).transpose() * m_errors[i];
				//	//std::cout << "Err-1:" << std::endl << m_errors[i - 1] << std::endl << std::endl;
				//}

				//for (int i = m_errors.size() - 2; i > 0; i--)
				//{
				//	for (int j = 0; j < m_errors[i].size(); j++)
				//		m_errors[i][j] *= m_neurons[i + 1][j] * (1.0 - m_neurons[i + 1][j]);
				//	//m_errors[i - 1] = m_weights[i].transpose() * m_errors[i];
				//	////m_errors[i][j] *= m_activationDerivative(m_neurons[i + 1][j]);

				//}

				////MathUtils::Print(ToString());
				////std::cout << "Errors:" << std::endl;
				////std::cout << m_errors[1] << std::endl << std::endl;
				////std::cout << m_errors[0] << std::endl << std::endl;

				////for (int i = 0; i < m_errors.size(); i++)
				////	for (int j = 0; j < m_errors[i].size(); j++)
				////		m_errors[i][j] *= m_activationDerivative(m_neurons[i + 1][j]);;

				////std::cout << "Errors:" << std::endl;
				////std::cout << m_errors[1] << std::endl << std::endl;
				////std::cout << m_errors[0] << std::endl << std::endl;

				//for (int i = 0; i < GetLayerCount() - 1; i++)
				//{
				//	m_weights[i] -= learnRate * m_errors[i] * m_neurons[i].transpose();
				//}
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

		ss << "\n\nBiases:\n";
		for (int i = 0; i < m_biases.size(); i++)
			ss << "\nLayer " << std::to_string(i + 1) << ":\n" << m_biases[i];

		ss << "\n\nConnections:\n";
		for (int i = 0; i < m_weights.size(); i++)
			ss << "\nLayer " << std::to_string(i) << ":\n" << m_weights[i];
		ss << "\n\n";

		return ss.str();
	}

}