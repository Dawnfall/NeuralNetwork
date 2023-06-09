#pragma once

#include "NeuralNetwork.h"

#include <vector>
#include <memory>



namespace dawn
{
	struct NeuralRequest
	{
		int Id;
		float* Inputs;
	};

	struct NeuralResult
	{
		int Id;
		float* Outputs;
	};

	class NeuralManager
	{
	public:
		NeuralManager();

		std::vector<int> CreateNNs(const std::vector<int>& layers, int count);
		std::vector<int> CreateNNsFromData(const std::vector<float>& data, int count);
		std::vector<int> DuplicateNN(int id, int count);

		void DestroyNNs(const std::vector<int>& ids);

		std::vector<std::vector<float>> GetNNDatas(const std::vector<int>& ids);

		void Mutate(const std::vector<int>& ids, float maxChange, float chance);
		std::vector<int> Cross(int idA, int idB, int count);
		NeuralResult* Calculate(NeuralRequest* requests,size_t requestsSize);

	private:
		int GetValidKey();
		void RemoveKey(int key);


		std::mt19937 m_randGen;
		std::vector<std::unique_ptr<NeuralNetwork>> m_allNNs;
		std::vector<int> m_availableIndices;
	};
}
