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

	class NeuralManager
	{
	public:
		NeuralManager();
		~NeuralManager();

		std::vector<int> CreateNNs(const std::vector<int>& layers, int count);
		std::vector<int> CreateNNsFromData(const std::vector<float>& data, int count);
		std::vector<int> DuplicateNN(int id, int count);

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
