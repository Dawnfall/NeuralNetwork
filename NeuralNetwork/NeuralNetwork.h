#pragma once

#include <math.h>
#include <vector>
#include <Eigen/Core>

#include "MathUtils.h"
#include "Data.h"

namespace dawn
{
	class NeuralNetwork
	{
	private:
		std::vector<Eigen::VectorXf> m_neurons;
		std::vector<Eigen::VectorXf> m_biases;
		std::vector<Eigen::MatrixXf> m_weights;

		std::vector<Eigen::VectorXf> m_errors;

		float (*m_activation)(float);
		float (*m_activationDerivative)(float);

	public:
		NeuralNetwork(const std::vector<int>& layers, float (*activation)(float), float(*activationDerivative)(float));
		void InitWeights();

		void SetInputLayer(const std::vector<float>& inputs)
		{
			for (int i = 0; i < inputs.size(); i++)
				m_neurons[0][i] = inputs[i];
		}

		size_t GetLayerCount() { return m_neurons.size(); }
		std::vector<float> GetLayer(int i){ return std::vector<float>(m_neurons[i].data(), m_neurons[i].data() + m_neurons[i].size()); }
		std::vector<float> GetInputLayer() { return std::vector<float>(m_neurons[0].data(), m_neurons[0].data() + m_neurons[0].size()); }
		std::vector<float> GetOutputLayer() { return std::vector<float>(m_neurons[GetLayerCount() - 1].data(), m_neurons[GetLayerCount() - 1].data() + m_neurons[GetLayerCount() - 1].size()); }

		void SetBiases(const std::vector<std::vector<float>>& biasLayers);
		void SetConnections(const std::vector<std::vector<std::vector<float>>>& connLayers);
		void SetActivation(float (*activation)(float));

		std::vector<float> FeedForward(const std::vector<float>& inputs);
		void BackPropagate(const TrainData& data,float learnRate);

		std::string ToString();

		void Train(const std::vector<TrainData>& data, const TrainParams& params);

		void TempTrain(const std::vector<TrainData>& data, const TrainParams& params);
	};
}
