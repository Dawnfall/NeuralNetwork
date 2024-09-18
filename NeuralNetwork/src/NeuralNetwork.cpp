#include "NeuralNetwork.h"

#include <sstream>
#include <iostream>
#include <random>

// Data -> [R1,C1,W1,W2,W3,....B1,B2,....][R2,C2,W1,W2,W3....B1B2,...]...

namespace dawn
{
#pragma region  Constructors

	NeuralNetwork::NeuralNetwork(
		const std::vector<int>& layers,
		std::mt19937& randGen,
		float(*activation)(float),
		Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&))
	{
		m_activation = activation;
		m_activationDerivative = activationDerivative;

		Create(layers);

		InitWeights(randGen);
		InitBiases(randGen);

	}

	NeuralNetwork::NeuralNetwork(
		const std::vector<std::vector<std::vector<float>>>& weights,
		const std::vector<std::vector<float>>& biases,
		float(*activation)(float),
		Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&))
	{
		std::vector<int> layers; //TODO: make this better
		for (auto weightLayer : weights)
			layers.emplace_back((int)weightLayer[0].size());
		layers.emplace_back((int)weights[weights.size() - 1].size());

		m_activation = activation;
		m_activationDerivative = activationDerivative;

		Create(layers);
		InitWeights(weights);
		InitBiases(biases);
	}

	NeuralNetwork::NeuralNetwork(
		const std::vector<float>& data)
	{
		m_neurons.push_back(Eigen::VectorXf::Zero((int)(data[1])));
		for (int i = 0; i < data.size(); )
		{
			int rowCount = (int)data[i];
			int colCount = (int)data[i + 1];

			m_neurons.push_back(Eigen::VectorXf::Zero(rowCount));
			m_weights.emplace_back(Eigen::MatrixXf(rowCount, colCount));
			m_biases.emplace_back(Eigen::VectorXf(rowCount));

			for (int row = 0; row < rowCount; row++)
			{
				int biasIndex = i + 2 + rowCount * colCount + row;
				m_biases.back()(row) = data[biasIndex];

				for (int col = 0; col < colCount; col++)
				{
					int weightIndex = i + 2 + row * colCount + col;
					m_weights.back()(row, col) = data[weightIndex];
				}
			}
			i += 2 + (colCount + 1) * rowCount;
		}

		m_activation = MathUtils::Sigmoid;
		m_activationDerivative = MathUtils::SigmoidDerivativeFromSigmoidInputVec;
	}

#pragma endregion

#pragma region Properties

	void NeuralNetwork::SetActivation(float (*activation)(float), Eigen::VectorXf(*activationDerivative)(const Eigen::VectorXf&))
	{
		m_activation = activation;
		m_activationDerivative = activationDerivative;
	}
	void NeuralNetwork::SetWeight(int layer, int row, int col, float newValue)
	{
		m_weights[layer](row, col) = newValue;
	}
	void NeuralNetwork::SetBias(int layer, int row, float newValue)
	{
		m_biases[layer](row) = newValue;
	}
	std::vector<int> NeuralNetwork::GetLayers()const
	{
		//TODO: no inner logic should use this
		std::vector<int> layers;
		for (auto& neuronLayer : m_neurons)
			layers.push_back((int)neuronLayer.size());
		return layers;
	}
	int NeuralNetwork::GetLayerCount()const
	{
		return (int)m_neurons.size();
	}

	int NeuralNetwork::GetLayerSize(int layer)const
	{
		if (layer == 0)
			return m_weights[0].cols();
		return m_biases[layer - 1].size();
	}
	std::vector<float> NeuralNetwork::GetLayer(size_t i)const
	{
		return std::vector<float>(m_neurons[i].data(), m_neurons[i].data() + m_neurons[i].size());
	}
	float NeuralNetwork::GetWeight(int layer, int row, int col)const
	{
		return m_weights[layer](row, col);
	}
	float NeuralNetwork::GetBias(int layer, int row)const
	{
		return m_biases[layer](row);
	}

#pragma endregion

#pragma region Operators
	bool NeuralNetwork::operator==(const NeuralNetwork& other)const
	{
		if (GetLayerCount() != other.GetLayerCount())
			return false;

		for (int i = 0; i < GetLayerCount(); i++)
			if (GetLayerSize(i) != other.GetLayerSize(i))
				return  false;

		for (int i = 0; i < GetLayerCount() - 1; i++)
			if (!m_biases[i].isApprox(other.m_biases[i]) || !m_weights[i].isApprox(other.m_weights[i]))
				return false;

		return true;
	}
	bool NeuralNetwork::operator!=(const NeuralNetwork& other)const
	{
		return !(*this == other);
	}

#pragma endregion

	std::vector<float> NeuralNetwork::FeedForward(const std::vector<float>& inputs)
	{
		std::copy(inputs.begin(), inputs.end(), m_neurons[0].begin());

		for (int i = 0; i < m_weights.size(); i++)
		{
			m_neurons[i + 1] = m_weights[i] * m_neurons[i] + m_biases[i];
			for (float& value : m_neurons[i + 1])
				value = m_activation(value);
		}

		return GetLayer(GetLayerCount() - 1);
	}

	void NeuralNetwork::BackPropagate(const TrainData& data, float learnRate)
	{
		std::vector<Eigen::VectorXf> errors(GetLayerCount() - 1);
		errors.back() = Eigen::VectorXf::Zero(m_neurons.back().size());

		std::vector<float> output = FeedForward(data.input);
		MathUtils::SubstractVec(output, data.target, errors.back());
		errors.back() = errors.back().cwiseProduct(m_activationDerivative(m_neurons.back()));

		for (int i = GetLayerCount() - 2; i > 0; i--)
		{
			errors[i - 1] = (m_weights[i].transpose() * errors[i]).cwiseProduct(m_activationDerivative(m_neurons[i]));
		}

		for (int i = GetLayerCount() - 2; i >= 0; i--)
		{
			m_weights[i] -= learnRate * errors[i] * m_neurons[i].transpose();
			m_biases[i] -= learnRate * errors[i];
		}
	}
	std::vector<NeuralNetwork> NeuralNetwork::Duplicate(int count) const
	{
		std::vector<NeuralNetwork> copies;
		copies.reserve(count);

		for (int i = 0; i < count; ++i)
			copies.emplace_back(*this);

		return copies;
	}

	void NeuralNetwork::Mutate(float changeProb, float maxChange, std::mt19937& randGen)
	{
		std::uniform_real_distribution<float> isMutationDist(0, 1);
		std::uniform_real_distribution<float> changeDist(-maxChange, maxChange);

		for (int i = 0; i < GetLayerCount() - 1; i++)
		{
			for (int row = 0; row < m_weights[i].rows(); row++)
				for (int col = 0; col < m_weights[i].cols(); col++)
				{
					if (isMutationDist(randGen) < changeProb)
					{
						float deltaChange = changeDist(randGen);
						m_weights[i](row, col) += deltaChange;
					}
				}
			for (int j = 0; j < m_biases[i].size(); j++)
			{
				if (isMutationDist(randGen) < changeProb)
				{
					float deltaChange = changeDist(randGen);
					m_biases[i](j) += deltaChange;
				}
			}
		}
	}
	NeuralNetwork NeuralNetwork::CrossOver(NeuralNetwork& a, NeuralNetwork& b, std::mt19937& randGen)
	{
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);

		NeuralNetwork newNN = a;
		for (int layer = 0; layer < newNN.GetLayerCount() - 1; layer++)
		{
			for (int row = 0; row < newNN.m_weights[layer].rows(); row++)
			{
				for (int col = 0; col < newNN.m_weights[layer].cols(); col++)
				{
					if (dist(randGen) < 0.5)
						newNN.SetWeight(layer, row, col, b.GetWeight(layer, row, col));
				}
				if (dist(randGen) < 0.5)
					newNN.SetBias(layer, row, b.GetBias(layer, row));
			}
		}
		return newNN;
	}

	std::vector<float> NeuralNetwork::ExportData()const
	{
		std::vector<float> data;

		for (int i = 0; i < GetLayerCount() - 1; i++)
		{
			data.push_back((float)(m_weights[i].rows()));
			data.push_back((float)(m_weights[i].cols()));

			for (int row = 0; row < m_weights[i].rows(); row++)
				for (int col = 0; col < m_weights[i].cols(); col++)
					data.push_back(m_weights[i](row, col));

			for (float bias : m_biases[i])
				data.push_back(bias);
		}

		return data;
	}

	std::string NeuralNetwork::ToString()const
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

	void NeuralNetwork::Create(const std::vector<int>& layers)
	{
		for (int i = 0; i < layers.size() - 1; i++)
		{
			m_weights.emplace_back(Eigen::MatrixXf(layers[i + 1], layers[i]));
			m_biases.emplace_back(Eigen::VectorXf(layers[i + 1]));
			m_neurons.emplace_back(Eigen::VectorXf::Zero(layers[i]));
		}
		m_neurons.emplace_back(Eigen::VectorXf::Zero(layers[layers.size() - 1]));
	}

	void NeuralNetwork::InitWeights(std::mt19937& gen)
	{
		for (int i = 0; i < m_weights.size(); i++)
		{
			float rangeMax = 1 / std::sqrt((float)m_weights[i].cols());
			std::uniform_real_distribution<float> dist(-rangeMax, rangeMax);

			for (int row = 0; row < m_weights[i].rows(); row++)
				for (int col = 0; col < m_weights[i].cols(); col++)
					m_weights[i](row, col) = dist(gen);
		}
	}
	void NeuralNetwork::InitWeights(const std::vector<std::vector<std::vector<float>>>& weights)
	{
		for (int i = 0; i < m_weights.size(); i++)
			for (int row = 0; row < m_weights[i].rows(); row++)
				for (int col = 0; col < m_weights[i].cols(); col++)
					m_weights[i](row, col) = weights[i][row][col];
	}

	void NeuralNetwork::InitBiases(std::mt19937& gen)
	{
		for (int i = 0; i < m_biases.size(); i++)
		{
			float rangeMax = 1.0f / std::sqrt((float)m_weights[i].cols());
			std::uniform_real_distribution<float> dist(-rangeMax, rangeMax);

			for (int j = 0; j < m_biases[i].size(); j++)
				m_biases[i][j] = dist(gen);
		}

	}
	void NeuralNetwork::InitBiases(const std::vector<std::vector<float>>& biases)
	{
		for (int i = 0; i < m_biases.size(); i++)
			for (int j = 0; j < m_biases[i].size(); j++)
				m_biases[i][j] = biases[i][j];
	}

	//void NeuralNetwork::SetConnections(const std::vector<std::vector<std::vector<float>>>& connLayers)
	//{
	//	for (int i = 0; i < connLayers.size(); i++)
	//		for (int row = 0; row < connLayers[i].size(); row++)
	//			for (int col = 0; col < connLayers[i][row].size(); col++)
	//				m_weights[i](row, col) = connLayers[i][row][col];
	//}
	//void NeuralNetwork::SetBiases(const std::vector<std::vector<float>>& biases)
	//{
	//	for (int i = 0; i < biases.size(); i++)
	//		for (int j = 0; j < biases[i].size(); j++)
	//			m_biases[i][j] = biases[i][j];
	//}


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

}