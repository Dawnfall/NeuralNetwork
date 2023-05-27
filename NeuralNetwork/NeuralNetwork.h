#pragma once

#include <math.h>
#include <vector>
#include <Eigen/Core>
#include <random>

#include "MathUtils.h"
#include "Data.h"


//TODO: bias neurons should not be returned as output layer

namespace dawn
{
	class NeuralNetwork
	{
	private:
		std::vector<int> m_layers;
		std::vector<Eigen::VectorXf> m_neurons;
		std::vector<Eigen::MatrixXf> m_weights;
		std::vector<Eigen::VectorXf> m_biases;

		float (*m_activation)(float);
		Eigen::VectorXf(*m_activationDerivative)(const Eigen::VectorXf&);

		void InitWeights(std::mt19937& gen);
		void InitBiases(std::mt19937& gen);
		void InitNeurons();

	public:
		NeuralNetwork(const std::vector<int>& layers,
			float (*activation)(float) = MathUtils::Sigmoid,
			Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&) = MathUtils::SigmoidDerivativeFromSigmoidInputVec);

		void SetInputLayer(const std::vector<float>& inputs)
		{
			for (int i = 0; i < inputs.size(); i++)
				m_neurons[0][i] = inputs[i];
		}

		inline size_t GetLayerCount() { return m_layers.size(); }
		std::vector<float> GetLayer(int i)
		{
			return std::vector<float>(m_neurons[i].data(), m_neurons[i].data() + m_neurons[i].size());
		}
		std::vector<float> GetInputLayer() { return GetLayer(0); }
		std::vector<float> GetOutputLayer() { return GetLayer(GetLayerCount() - 1); }

		void SetConnections(const std::vector<std::vector<std::vector<float>>>& connLayers);
		void SetBiases(const std::vector<std::vector<float>>& biases);
		void SetActivation(float (*activation)(float), Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&))
		{
			m_activation = activation;
			m_activationDerivative = activationDerivative;
		}

		std::vector<float> FeedForward(const std::vector<float>& inputs);
		void BackPropagate(const TrainData& data, float learnRate);

		void Train(const std::vector<TrainData>& data, const TrainParams& params);
		void TempTrain(const std::vector<TrainData>& data, const TrainParams& params);

		std::string ToString();
	};
}
