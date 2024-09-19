#pragma once
//
#include <vector>
#include <Eigen/Core>
#include <random>
//
#include "Core.h"
#include "MathUtils.h"
#include "TrainData.h"

namespace dawn
{
	class NeuralNetwork
	{
	public:
#pragma region Constructors
		NeuralNetwork(
			const std::vector<int>& layers,
			std::mt19937& randGen,
			float (*activation)(float) = MathUtils::Sigmoid,
			Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&) = MathUtils::SigmoidDerivativeFromSigmoidInputVec);

		NeuralNetwork(
			const std::vector<std::vector<std::vector<float>>>& weights,
			const std::vector<std::vector<float>>& biases,
			float (*activation)(float) = MathUtils::Sigmoid,
			Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&) = MathUtils::SigmoidDerivativeFromSigmoidInputVec);

		NeuralNetwork(
			const std::vector<float>& data
		);
#pragma endregion

#pragma region Properties
		void SetActivation(float (*activation)(float), Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&));
		void SetWeight(int layer, int row, int col, float newValue);
		void SetBias(int layer, int row, float newValue);

		std::vector<int> GetLayers()const;
		int GetLayerCount()const;
		int GetLayerSize(int layer)const;
		std::vector<float> GetLayer(size_t i)const;
		float GetWeight(int layer, int row, int col)const;
		float GetBias(int layer, int row)const;
#pragma endregion

#pragma region Functionality

		std::vector<float> FeedForward(const std::vector<float>& inputs); // calculates output of the network
		std::vector<Eigen::VectorXf> BackPropagate(const std::vector<float>& input, const std::vector<float>& targetOutput); //calculates the error of the network
		void UpdateWeights(const std::vector<Eigen::VectorXf>& errors, float learnRate); //updates the weights of the network based on the error

		std::vector<NeuralNetwork> Duplicate(int count)const;
		void Mutate(float changeProb, float maxChange, std::mt19937& randGen);
		static NeuralNetwork CrossOver(NeuralNetwork& a, NeuralNetwork& b, std::mt19937& randGen);

		bool operator==(const NeuralNetwork& other)const;
		bool operator!=(const NeuralNetwork& other)const;
#pragma endregion

		std::vector<float> ExportData()const;
		std::string ToString()const;

		static void Train(NeuralNetwork& nn, const TrainData& data);


	private:
		std::vector<Eigen::VectorXf> m_neurons;
		std::vector<Eigen::MatrixXf> m_weights;
		std::vector<Eigen::VectorXf> m_biases;

		float (*m_activation)(float);
		Eigen::VectorXf(*m_activationDerivative)(const Eigen::VectorXf&);

		void Create(const std::vector<int>& layers);
		void InitWeights(std::mt19937& gen);
		void InitBiases(std::mt19937& gen);
		void InitWeights(const std::vector<std::vector<std::vector<float>>>& weights);
		void InitBiases(const std::vector<std::vector<float>>& biases);
	};
}
