#pragma once

#include "NeuralNetwork.h"
#include "Core.h"

#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <thread>

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

	class NEURAL_API NeuralManager
	{
	public:
		NeuralManager();
		~NeuralManager();

		std::vector<int> CreateNNs(const std::vector<int>& layers, int count);
		std::vector<int> CreateNNsFromData(const std::vector<float>& data, int count);
		std::vector<int> DuplicateNN(int id, int count);

		void DestroyNNs(const std::vector<int>& ids);

		std::vector<std::vector<float>> GetNNDatas(const std::vector<int>& ids);

		void Mutate(const std::vector<int>& ids, float maxChange, float chance);
		std::vector<int> Cross(int idA, int idB, int count);
		NeuralResult* Calculate(NeuralRequest* requests,size_t requestsSize);

		void AddRequest(const NeuralRequest& request);
		std::vector<NeuralResult> PopResults();


	private:
		void Run();
		int GetValidKey();
		void RemoveKey(int key);


		std::thread m_runThread;
		std::mt19937 m_randGen;
		std::vector<std::unique_ptr<NeuralNetwork>> m_allNNs;
		std::vector<int> m_availableIndices;

		std::queue<NeuralRequest> m_requests;
		std::queue<NeuralResult> m_results;
		std::mutex m_mutex;
		std::condition_variable m_conditionVar;
	};
}
